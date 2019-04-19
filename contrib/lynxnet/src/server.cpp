#include "server.h"

// Constructors & Destructors

Server::Server() {
	FD_ZERO(&sockets);
}

Server::~Server() {
	if (running) {
		running = false;

		conditional.notify_one();
	}
}

// Member Functions

int Server::bind(const std::string& port) {
	const char* port_c = port.c_str();

	// Get Linked-List Of Address Structures
		
	struct addrinfo hints = { 0 };

   	hints.ai_family = AF_INET;
   	hints.ai_socktype = SOCK_STREAM;
   	hints.ai_flags = AI_PASSIVE;

	struct addrinfo* results = nullptr;

   	if (getaddrinfo(NULL, port_c, &hints, &results) != 0) {
	   	// Failed To Get Local AddrInfo
			
		return -1;
   	}

	// Loop Through Address Structures Until An Appropriate One Is Found

	int sock = 0;

	struct addrinfo* inf = nullptr;
	
   	for(inf = results; inf != NULL; inf = inf->ai_next) {
	   	sock = socket(inf->ai_family, inf->ai_socktype, inf->ai_protocol);

	   	if (sock < 0) {
			// Local Socket Invalid, Skipping

	   		continue;
	   	}

		// Set/Get Socket Options - Address Reuseable

		int yes = 1;
		
	   	setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes));

		// Bind The Socket

	   	if (::bind(sock, inf->ai_addr, inf->ai_addrlen) < 0) {
			// Failed To Bind, Skipping

		   	continue;
	   	}

	   	break;
   	}

	// Check Bind Valid

	freeaddrinfo(results);

	if (inf == NULL) {
		// Returned Local Socket Invalid

		return -2;
	}

	// Try Listen

   	if (::listen(sock, 10) == -1) {
	   	// Failed To Listen

	   	return -3;
   	}

	// Add Server Socket To FileDescriptor Set

	FD_SET(sock, &sockets);

	if (sock > sockmax) sockmax = sock;

	self = sock;

	return 1;
}

int Server::connect(const std::string& addr, const std::string& port) {
	const char* addr_c = addr.c_str();
	const char* port_c = port.c_str();

	// Get Linked-List Of Address Structures
		
	struct addrinfo hints = { 0 };

   	hints.ai_family = AF_INET;
   	hints.ai_socktype = SOCK_STREAM;

	struct addrinfo* results;

	if (getaddrinfo(addr_c, port_c, &hints, &results) != 0) {
	   	// Failed To Get Host AddrInfo
			
		return -1;
   	}

	// Loop Through Address Structures Until An Appropriate One Is Found

	int sock = 0;

	struct addrinfo* inf = nullptr;

	for(inf = results; inf != NULL; inf = inf->ai_next) {
	   	sock = ::socket(inf->ai_family, inf->ai_socktype, inf->ai_protocol);

	   	if (sock < 0) {
			// Host Socket Invalid, Skipping

	   		continue;
	   	}

		// Connect To Socket

		if (::connect(sock, inf->ai_addr, inf->ai_addrlen) < 0) {
			// Failed To Connect To Host, Skipping

			::close(sock);

			continue;
		}

	   	break;
   	}

	// Check Connect Valid

	freeaddrinfo(results);

	if (inf == NULL) {
		// Returned Host Socket Invalid

		return -2;
	}

	// Add Host Socket To FileDescriptor Set

	FD_SET(sock, &sockets);

	if (sock > sockmax) sockmax = sock;

	return 1;
}

bool Server::start() {
	// Start Threads If Not Running

	if (!running) {
		running = true;

		listener = std::thread(&Server::listen, this);
		listener.detach();

		broadcaster = std::thread(&Server::broadcast, this);
		broadcaster.detach();

		return true;
	}

	return false;
}

void Server::listen() {
	// Listen For New Connections & Data

	char buf[1024];
   	int nbytes;

	while (running) {
		// Get All FileDescriptors That Return Instead Of Blocking

		fd_set tmp = sockets;

		if (::select(sockmax + 1, &tmp, NULL, NULL, NULL) == -1) continue;

		//if (::poll(&tmp, sockmax + 1, NULL) == -1) continue;

		for (int i = 0; i <= sockmax; i++) {
			// Check If Part Of Set

			if (FD_ISSET(i, &tmp)) {
				// If Current Socket Is Self, Listen For New Connections

			   	if (i == self) {
					// Peer Possibly Requested Connection

					nbytes = ::accept(self, NULL, NULL);

				   	if (nbytes != -1) {
						// Add To FileDescriptor Set

						FD_SET(nbytes, &sockets);

						if (nbytes > sockmax) sockmax = nbytes;

						// Send Peer Connect Information To Peers

						Packet message(nbytes, "client connected");

						send(message, true);
				   	}
			   	}
				else {
					// Parse Received Data

					nbytes = ::recv(i, buf, sizeof buf, 0);

				   	if (nbytes <= 0) {
						// Peer Disconnected Or Error Occurred

						if (nbytes == 0) {
							// Remove From FileDescriptor Set

							FD_CLR(i, &sockets);

							::close(i);

							// Send Peer Disconnect Information To Peers

							Packet message(i, "client disconnected");

							send(message, true);
						}
						else {
							// Unknown Error
						}
				   	}
					else {
						// Packet Received From Peer

						std::string text = "";

						for (int c = 0; c < nbytes; c++) text += buf[c];

						// Send Received Data To Peers
						
						Packet message(i, nbytes, &text[0]);

						send(message, true);
				   	}
			   	}
		   	}
	   	}
	}
}

void Server::broadcast() {
	for (;;) {
		while (sendbuffer.size() > 0) {
			// Send All Queued Messages To Each Socket

			sendmut.lock();
			Packet message = sendbuffer.front();
			sendbuffer.pop();
			sendmut.unlock();
				
			for(int i = 0; i <= sockmax; i++) {
	   			if (i != message.socket && i != self && FD_ISSET(i, &sockets)) {
					// Ensure No Data Is Omitted
					
					int len = message.text.length();
					const char* txt = message.text.c_str();

					int total = 0;

					while (total < len) {
						int ret = ::send(i, txt + total, len - total, 0);
						
						if (ret < 0) break;
						
						total += ret;
					}
	   			}
			}
		}

		// Exit If Program No Longer Running, Wait For Conditional Otherwise

		if (!running) return;

		std::unique_lock<std::mutex> lk(condmut);

		conditional.wait(lk);
	}
}

void Server::send(const Packet& message, bool includeLocal) {
	// Add To Send Queue In A Thread-Safe Manner

	sendmut.lock();
	sendbuffer.push(message);
	sendmut.unlock();

	if (includeLocal) {
		recvmut.lock();
		recvbuffer.push(message);
		recvmut.unlock();
	}

	conditional.notify_one();
}

bool Server::recv(Packet& message) {
	// Get From Receive Queue In A Thread-Safe Manner

	if (recvbuffer.size() > 0) {
		recvmut.lock();
		message = recvbuffer.front();
		recvbuffer.pop();
		recvmut.unlock();

		return true;
	}

	return false;
}

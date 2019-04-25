#include "server.h"

#include <iostream>

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

	host = sock;

	return 1;
}

int Server::disconnect(bool reconnect) {
	// Reset Everything Except Network Image

	return 0;
}

bool Server::start() {
	// Start Threads If Not Running

	if (!running) {
		running = true;

		broadcaster = std::thread(&Server::broadcast, this);
		broadcaster.detach();

		listener = std::thread(&Server::listen, this);
		listener.detach();

		// Setup If No Host

		if (!host) network.self = network.add_peer(000);

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

					struct sockaddr_in clientaddr;
					socklen_t clientaddr_size = sizeof(clientaddr);

					nbytes = ::accept(self, (struct sockaddr*) &clientaddr, &clientaddr_size);

				   	if (nbytes != -1) {
						// Add To FileDescriptor Set

						FD_SET(nbytes, &sockets);

						if (nbytes > sockmax) sockmax = nbytes;

						// Send Network Image Serialised

						std::string net = network.serialise();

						if (net != "") send_to(Packet(nbytes, "16" + net + '\n'));		

						// Set Socket On Peer

						send_to(Packet(nbytes, "13" + int_to_str(network.self->uuid, 3) + '\n'));
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

							// Get UUID Of Peer

							Peer* peer = network.from_sock(i);

							std::string uuid = peer ? int_to_str(peer->uuid, 3) : "999";

							// Forward Disconnect NetEvent

							parse(Packet(i, "02" + uuid + '\n'));
						}
						else {
							// Unknown Error
						}
				   	}
					else {
						// Packet Received From Peer

						std::string msg = "";

						for (int c = 0; c < nbytes; c++) msg += buf[c];

						// Parse & Forward

						parse(Packet(i, msg));
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
			
			// Figure Out If Inclusive Or Exclusive

			bool inclusive = true;
			
			if (message.socket < 0) {
				inclusive = false;

				message.socket = -message.socket;
			}

			// Send
				
			for(int i = 0; i <= sockmax; i++) {
				if (i == self || !FD_ISSET(i, &sockets)) continue;

				if (inclusive && i != message.socket) continue;

				if (!inclusive && i == message.socket) continue;
				
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

		// Exit If Program No Longer Running, Wait For Conditional Otherwise

		if (!running) return;

		std::unique_lock<std::mutex> lk(condmut);

		conditional.wait(lk);
	}
}

void Server::send_to(const Packet& message) {
	// Add To Send Queue In A Thread-Safe Manner

	sendmut.lock();
	sendbuffer.push(message);
	sendmut.unlock();

	conditional.notify_one();
}

void Server::send(const std::string& text) {
	// Send To All Peers

	send_to(Packet(-self, text));
}

bool Server::recv(std::string& text) {
	// Get From Receive Queue In A Thread-Safe Manner

	if (recvbuffer.size() > 0) {
		recvmut.lock();
		Packet message = recvbuffer.front();
		recvbuffer.pop();
		recvmut.unlock();

		text = message.text;

		return true;
	}

	return false;
}

void Server::parse(const Packet& message) {
	// Split Merged Messages

	std::vector<Packet> packets = message.split();
	
	if (packets.size() > 0) {
		for (Packet& packet : packets) parse(packet);

		return;
	}

	// Extract Type Data From Text

	std::string text = message.text;

	BroadcastType target = (BroadcastType) (text[0] - '0');

	text.erase(text.begin());

	PacketType type = (PacketType) (text[0] - '0');

	text.erase(text.begin());

	// Debug Log
	
	std::string target_str = target == 0 ? "GLBL" : "MSSG";
	std::string type_str = type == 0 ? "GAMEDAT" : type == 1 ? "NEWCONN" : type == 2 ? "REMCONN" : type == 3 ? "SETSOCK" : type == 4 ? "SETNAME" : type == 5 ? "SETIDIP" : "NETSTAT";

	recvmut.lock();
	recvbuffer.push(Packet(self, "[" + target_str + ":" + type_str + "]" + text));
	recvmut.unlock();

	// If Broadcast, Forward Message To Peers
	
	if (target == 0) send_to(Packet(-message.socket, message.text));

	// GAMEDAT

	if (type == 0) {
		// Add To Receive Queue

		recvmut.lock();
		recvbuffer.push(Packet(self, text));
		recvmut.unlock();
	}

	// NEWCONN

	if (type == 1) {
		// Add To Network Image

		std::string uuid_str = text.substr(0, 3);
		std::string host_str = text.substr(3, 6);

		if (uuid_str == "" || host_str == "") return;

		int uuid = stoi(uuid_str);
		int host = stoi(host_str);

		Peer* peer = network.add_peer(uuid, host);

		// Set Self

		if (message.socket == self) network.self = peer;
	}

	// REMCONN

	if (type == 2) {
		// Remove From Network Image

		std::string uuid_str = text.substr(0, 3);

		if (uuid_str == "") return;

		int uuid = stoi(uuid_str);

		network.clr_peer(uuid);
	}

	// SETSOCK

	if (type == 3) {
		// Assign Socket Number To Peer

		std::string uuid_str = text.substr(0, 3);

		if (uuid_str == "") return;

		int uuid = stoi(uuid_str);

		Peer* peer = network.from_uuid(uuid);

		if (peer) peer->socket = message.socket;
	}

	// SETNAME

	if (type == 4) {
		// Assign Name To Peer
		
		std::string uuid_str = text.substr(0, 3);

		if (uuid_str == "") return;

		int uuid = stoi(uuid_str);

		Peer* peer = network.from_uuid(uuid);

		if (peer) peer->name = text.substr(3, -1);
	}

	// SETIDIP

	if (type == 5) {
		// Assign IP To Peer

		std::string uuid_str = text.substr(0, 3);

		if (uuid_str == "") return;

		int uuid = stoi(uuid_str);

		Peer* peer = network.from_uuid(uuid);

		if (peer) peer->address = text.substr(3, -1);
	}

	// NETSTAT

	if (type == 6) {
		// Parse Received Network Image

		network.parse(text.substr(0, -1));

		// Identify Host

		int host_uuid = network.sock_to_uuid(message.socket);

		// Add Self To Network

		int uuid = network.new_uuid();

		parse(Packet(self, "01" + int_to_str(uuid, 3) + int_to_str(host_uuid, 3) + '\n'));

		// Set Sock On Host

		send_to(Packet(host, "13" + int_to_str(uuid, 3) + '\n'));
	}
}

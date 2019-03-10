#include "server.h"

// Constructors & Destructors

Server::Server() {
	
}

Server::~Server() {
	if (running) {
		running = false;

		conditional.notify_one();

		//listener->join();
		//broadcaster->join();
	}
	
	for(int i = 0; i <= maxsock; i++) if (FD_ISSET(i, &sockets)) close(i);
}

// Member Functions

int Server::bind(const std::string& port) {
	const char* port_c = port.c_str();

	// Get Linked-List Of Address Structures
		
	struct addrinfo hints = { 0 };

   	hints.ai_family = AF_INET;
   	hints.ai_socktype = SOCK_STREAM;
   	hints.ai_flags = AI_PASSIVE;

	struct addrinfo* results;

   	if (getaddrinfo(NULL, port_c, &hints, &results) != 0) {
       	// Failed To Get Local AddrInfo

		freeaddrinfo(results);
			
		return -1;
   	}

	// Loop Through Address Structures Until An Appropriate One Is Found
    
   	for(selfinf = results; selfinf != NULL; selfinf = selfinf->ai_next) {
       	self = socket(selfinf->ai_family, selfinf->ai_socktype, selfinf->ai_protocol);

       	if (self < 0) {
			// Local Socket Invalid, Skipping

       		continue;
       	}

		// Set/Get Socket Options - Address Reuseable

		int yes = 1;
        
       	setsockopt(self, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes));

		// Bind The Socket

       	if (::bind(self, selfinf->ai_addr, selfinf->ai_addrlen) < 0) {
			// Failed To Bind, Skipping

           	continue;
       	}

       	break;
   	}

	// Check Bind Valid

	freeaddrinfo(results);

	if (selfinf == NULL) {
		// Returned Local Socket Invalid

		return -2;
	}

	// Try Listen

   	if (::listen(self, 10) == -1) {
       	// Failed To Listen

       	return -3;
   	}

	// Add Server Socket To FileDescriptor Set

	FD_SET(self, &sockets);

	if (self > maxsock) maxsock = self;

	bound = true;

	if (!running) {
		running = true;

		listener = std::thread(&Server::listen, this);
		listener.detach();

		broadcaster = std::thread(&Server::broadcast, this);
		broadcaster.detach();
	}

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

		freeaddrinfo(results);
			
		return -1;
   	}

	// Loop Through Address Structures Until An Appropriate One Is Found

	for(hostinf = results; hostinf != NULL; hostinf = hostinf->ai_next) {
       	host = socket(hostinf->ai_family, hostinf->ai_socktype, hostinf->ai_protocol);

       	if (host < 0) {
			// Host Socket Invalid, Skipping

       		continue;
       	}

		// Connect To Socket

		if (::connect(host, hostinf->ai_addr, hostinf->ai_addrlen) < 0) {
			// Failed To Connect To Host, Skipping

			continue;
		}

       	break;
   	}

	// Check Connect Valid

	freeaddrinfo(results);

	if (hostinf == NULL) {
		// Returned Host Socket Invalid

		return -2;
	}

	// Add Host Socket To FileDescriptor Set

	FD_SET(host, &sockets);

	if (host > maxsock) maxsock = host;

	connected = true;

	if (!running) {
		running = true;

		listener = std::thread(&Server::listen, this);
		listener.detach();

		broadcaster = std::thread(&Server::broadcast, this);
		broadcaster.detach();
	}

	return 1;
}

void Server::listen() {
	// Listen For New Connections & Data

	char buf[1024];
   	int nbytes;

	while (running) {
		// Get All FileDescriptors That Return Instead Of Blocking

		fd_set tmp = sockets;
        	
		if (select(maxsock + 1, &tmp, NULL, NULL, NULL) == -1) continue;

		// Loop Through FileDescriptors
        	
       	for(int i = 0; i <= maxsock; i++) {
			// Check If Part Of Set

           	if (FD_ISSET(i, &tmp)) {
				// If Current Socket Is This Client Listen For New Connections

               	if (self != -1 && i == self) {
					struct sockaddr_storage remoteaddr;

                   	socklen_t addrlen = sizeof remoteaddr;
                    	
					int newsock = accept(self, (struct sockaddr*) &remoteaddr, &addrlen);

                   	if (newsock != -1) {
						// Add To FileDescriptor Set

                   	    FD_SET(newsock, &sockets);

                   	    if (newsock > maxsock) maxsock = newsock;

						// Get Socket Info

						char ipstr[INET6_ADDRSTRLEN];
						int port;

						struct sockaddr_in* s = (struct sockaddr_in*) &remoteaddr;
    					
						inet_ntop(AF_INET, &s->sin_addr, ipstr, sizeof ipstr);
						port = ntohs(s->sin_port);

						Message msg(newsock, "Client connected " + std::string(ipstr) + ":" + std::to_string(port));

						send_loc(msg);

						send(msg);
                   	}
               	}
				else {
					// Parse Received Data

                   	if ((nbytes = ::recv(i, buf, sizeof buf, 0)) <= 0) {
                      	// Error Received Or Connection Closed By Client

                       	FD_CLR(i, &sockets);

						close(i);

						if (nbytes == 0) {
							Message msg(i, "Client disconnected");
							
							send_loc(msg);

							send(msg);
						}
                   	}
					else {
						Message msg(i, "");

						for (int c = 0; c < nbytes; c++) msg.text += buf[c];

						send_loc(msg);

						send(msg);
                   	}
               	}
           	}
       	}
	}
}

void Server::broadcast() {
	for (;;) {
		while (sendbuffer.size() > 0) {
			sendmut.lock();

			Message msg = sendbuffer.front();

			sendbuffer.pop();

			sendmut.unlock();
				
			for(int i = 0; i <= maxsock; i++) {
       			if (i != self && i != msg.sock && FD_ISSET(i, &sockets)) {
					const char* txt = msg.text.c_str();
					int len = msg.text.size();

					int total = 0;

					while (total < len) {
						int ret = ::send(i, txt + total, len - total, 0);
						
						if (ret < 0) break;
						
						total += ret;
					}
       	    	}
       		}
		}

		if (!running) return;

		std::unique_lock<std::mutex> lk(condmut);

		conditional.wait(lk);
	}
}

void Server::send_loc(const Message& message) {
	recvmut.lock();

	recvbuffer.push(message);

	recvmut.unlock();
}

void Server::send(const Message& message) {
	sendmut.lock();

	sendbuffer.push(message);
		
	sendmut.unlock();

	conditional.notify_one();
}

bool Server::recv(Message& message) {
	if (recvbuffer.size() > 0) {
		recvmut.lock();

		message = recvbuffer.front();

		recvbuffer.pop();

		recvmut.unlock();

		return true;
	}

	return false;
}

void Server::this_addr(std::string& addr, std::string& port) {

}

void Server::host_addr(std::string& addr, std::string& port) {

}

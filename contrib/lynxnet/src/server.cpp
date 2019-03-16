#include "server.h"

#include <iostream>

// Constructors & Destructors

Server::Server() {
	
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

		freeaddrinfo(results);
			
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

	mesh.add(sock);

	mesh.peers = new Socket(sock);

	// Get Local IP Addr

	std::string locAddr = "";

	struct ifaddrs* ifAddrStruct = NULL;
	struct ifaddrs* ifa = NULL;
	void* tmpAddrPtr = NULL;

	getifaddrs(&ifAddrStruct);

	for (ifa = ifAddrStruct; ifa != NULL; ifa = ifa->ifa_next) {
		if (!ifa->ifa_addr) continue;

		tmpAddrPtr = &((struct sockaddr_in *) ifa->ifa_addr)->sin_addr;
		
		char addressBuffer[INET_ADDRSTRLEN];

		inet_ntop(AF_INET, tmpAddrPtr, addressBuffer, INET_ADDRSTRLEN);

		locAddr = addressBuffer;

		std::cout << ifa->ifa_name << " : " << locAddr << std::endl;
	}

	if (ifAddrStruct!=NULL) freeifaddrs(ifAddrStruct);

	mesh.peers->addr = locAddr;
	mesh.peers->port = port;

	std::cout << "addr:port=" << locAddr << ":" << port << std::endl;

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

	int sock = 0;

	struct addrinfo* inf = nullptr;

	for(inf = results; inf != NULL; inf = inf->ai_next) {
       	sock = socket(inf->ai_family, inf->ai_socktype, inf->ai_protocol);

       	if (sock < 0) {
			// Host Socket Invalid, Skipping

       		continue;
       	}

		// Connect To Socket

		if (::connect(sock, inf->ai_addr, inf->ai_addrlen) < 0) {
			// Failed To Connect To Host, Skipping

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

	mesh.add(sock);

	mesh.peers->set_host(sock);

	return 1;
}

void Server::start() {
	// Start Threads If Not Running

	if (!running) {
		running = true;

		listener = std::thread(&Server::listen, this);
		listener.detach();

		broadcaster = std::thread(&Server::broadcast, this);
		broadcaster.detach();
	}
}

void Server::listen() {
	// Listen For New Connections & Data

	char buf[1024];
   	int nbytes;

	while (running) {
		// Get All FileDescriptors That Return Instead Of Blocking

		fd_set tmp;

		if (!mesh.select(&tmp)) continue;

		// Loop Through FileDescriptors
        	
       	for(int i = 0; i <= mesh.count; i++) {
			// Check If Part Of Set

           	if (mesh.is_set(i)) {
				// If Current Socket Is This Client Listen For New Connections

               	if (mesh.is_self(i)) {
					int sock = accept(mesh.peers->sock, NULL, NULL);

                   	if (sock != -1) {
						// Add To FileDescriptor Set

                   	    mesh.add(sock);

						mesh.peers->add_child(sock);

						// Report

						Message msg(sock, "Client connected");

						send_loc(msg);

						send(msg);
                   	}
               	}
				else {
					// Parse Received Data

					std::string text = "";

					for (int c = 0; c < nbytes; c++) text += buf[c];

                   	if ((nbytes = ::recv(i, buf, sizeof buf, 0)) <= 0) {
						bool ishost = mesh.is_host(i);

						if (nbytes == 0) {
							// Remove From FileDescriptor Set

                    		mesh.remove(i);

							ishost ? mesh.peers->clear_host() : mesh.peers->remove_child(i);

							// Report

							Message msg(i, ishost ? "Host Disconnected" : "Client disconnected");
							
							send_loc(msg);

							send(msg);
						}
						else {
							// Unknown Error
						}
                   	}
					else {
						// Report

						Message msg(i, text);

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
		Message message;

		while (sendbuffer.size() > 0) {
			sendmut.lock();

			message = sendbuffer.front();
			sendbuffer.pop();

			sendmut.unlock();
				
			for(int i = 0; i <= mesh.count; i++) {
       			if (i != message.sock && !mesh.is_self(i) && mesh.is_set(i)) {
					const char* txt = message.text.c_str();
					int len = message.text.size();

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

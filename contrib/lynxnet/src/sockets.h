#ifndef sockets_h
#define sockets_h

#include <string>
#include <vector>
#include <queue>
#include <algorithm>

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>

class Socket {
private:
	// Member Variables

	Socket* host = nullptr;
	std::vector<Socket*> children = std::vector<Socket*>();
public:
	// Member Variables

	int sock = -1;

	std::string addr = "";
	std::string port = "";

	// Constructors & Destructors

	Socket(int sock);

	// Member Functions

	Socket* get_host();
	void set_host(int sock);
	void clear_host();

	Socket* get_child(int sock);
	void add_child(int sock);
	void remove_child(int sock);
};

class MeshNet {
private:
	// Member Variables

	fd_set set;
public:
	// Member Variables

	Socket* peers = nullptr;

	int count = 0;

	// Member Functions

	bool is_set(int sock);
	bool is_self(int sock);
	bool is_host(int sock);

	bool select(fd_set* sel);

	void add(int sock);
	void remove(int sock);
	
	// Remove Somehow
};

#endif

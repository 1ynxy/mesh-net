#ifndef sockets_h
#define sockets_h

#include <string>
#include <vector>
#include <queue>

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

enum SockType {
	SOCK_SELF,
	SOCK_HOST,
	SOCK_PEER
};

struct Socket {
	// Member Variables

	SockType type = SOCK_PEER;

	int sock = -1;

	struct addrinfo* inf = { 0 };

	std::string addr = "";
	std::string port = "";

	// Constructors & Destructors

	Socket();
	Socket(int sock, SockType type);

	~Socket();
};

class Mesh {
private:
	// Member Variables

	std::vector<Socket> peers = std::vector<Socket>();

	fd_set set;
	int count;
public:
	// Constructors & Destructors

	Mesh();

	~Mesh();

	// Member Functions

	void add(int sock, struct addrinfo* inf);
	
	// Remove Somehow
};

#endif

#ifndef peer_h
#define peer_h

#include <string>
#include <vector>

enum PortStat {
	PORT_FORWARD,
	PORT_BLOCK,
	PORT_UNDEF
};

struct Peer {
	// Member Variables

	std::string global;
	std::string local;

	int socket;

	PortStat stat;

	Peer* host;
	std::vector<Peer*> children;

	// Member Functions



	// Constructors & Destructors

	
};

#endif

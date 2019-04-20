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

	int uuid = 0;

	int socket = 0;

	std::string global = "";
	std::string local = "";

	PortStat stat = PORT_UNDEF;

	Peer* host = nullptr;
	std::vector<Peer*> children = std::vector<Peer*>();

	// Member Functions



	// Constructors & Destructors


};

#endif

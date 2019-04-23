#ifndef peer_h
#define peer_h

#include <string>
#include <vector>

struct Peer {
	// Member Variables

	int uuid = 0;

	int socket = 0;

	std::string global = "";
	std::string local = "";

	// Network Tree

	Peer* host = nullptr;
	std::vector<Peer*> children = std::vector<Peer*>();

	// Member Functions



	// Constructors & Destructors


};

#endif

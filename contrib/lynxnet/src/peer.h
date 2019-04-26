#ifndef peer_h
#define peer_h

#include <string>
#include <vector>
#include <algorithm>

struct Peer {
	// Member Variables

	int uuid = -1;
	int socket = -1;

	std::string name = "unnamed";

	std::string address = "1.0.0.1";

	// Network Tree

	Peer* host = nullptr;
	std::vector<Peer*> children = std::vector<Peer*>();

	// Constructors & Destructors

	Peer(int uuid);

	// Member Functions

	std::string serialise();

	void remove();
};

// Global Functions

std::string int_to_str(int in, int length);

#endif

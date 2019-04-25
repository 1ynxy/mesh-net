#ifndef network_h
#define network_h

#include <vector>
#include <algorithm>

#include <../src/peer.h>

class Network {
private:
	// Member Variables

	std::vector<Peer*> peers;

public:
	// Member Variables

	Peer* self;

	// Member Functions

	int new_uuid();

	Peer* get_peer(int uuid);

	void add_peer(int uuid, int host);
	void clr_peer(int uuid);
};

#endif

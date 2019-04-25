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

	Peer* from_uuid(int uuid);
	Peer* from_sock(int sock);

	Peer* add_peer(int uuid, int host = -1);
	void clr_peer(int uuid);

	int sock_to_uuid(int sock);
	int uuid_to_sock(int uuid);

	std::string serialise();
	void parse(const std::string& in);
};

#endif

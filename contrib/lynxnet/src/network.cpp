#include "network.h"

// Member Functions

int Network::new_uuid() {
	// Generate A New UUID

	int uuid = 0;

	Peer* peer = get_peer(uuid);

	while (peer) {
		uuid ++;

		peer = get_peer(uuid);
	}

	return uuid;
}

Peer* Network::get_peer(int uuid) {
	// Return First Peer With UUID

	for (Peer* peer : peers) if (peer->uuid == uuid) return peer;

	return nullptr;
}

void Network::add_peer(int uuid, int host) {
	// Create New Peer

	Peer* newPeer = new Peer(uuid);

	// Set Host To Host Peer

	Peer* hostPeer = get_peer(host);

	if (hostPeer) newPeer->host = hostPeer;

	// Add To Peers List

	peers.push_back(newPeer);
}

void Network::clr_peer(int uuid) {
	// Recursively Remove Peer & Children From List & Destroy

	Peer* peer = get_peer(uuid);

	while (peer) {
		if (peer->children.size() > 0) peer = peer->children[0];
		else {
			// Remove From List

			std::vector<Peer*>::iterator iter = std::find(peers.begin(), peers.end(), peer);

			if (iter != peers.end()) peers.erase(iter);

			// Destroy Peer

			Peer* host = peer->host;

			delete(peer);

			peer = host;
		}
	}
}

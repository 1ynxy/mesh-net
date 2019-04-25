#include "network.h"

// Member Functions

int Network::new_uuid() {
	// Generate A New UUID

	int uuid = 0;

	Peer* peer = get_peer_by_uuid(uuid);

	while (peer) {
		uuid ++;

		peer = get_peer_by_uuid(uuid);
	}

	return uuid;
}

Peer* Network::get_peer_by_uuid(int uuid) {
	// Return First Peer With UUID

	for (Peer* peer : peers) if (peer->uuid == uuid) return peer;

	return nullptr;
}

Peer* Network::get_peer_by_sock(int sock) {
	// Return First Peer With Sock

	for (Peer* peer : peers) if (peer->socket == sock) return peer;

	return nullptr;
}

Peer* Network::add_peer(int uuid, int host) {
	// Find Host Peer

	Peer* hostPeer = get_peer_by_uuid(host);
	
	// Create New Peer & Add To Peers

	Peer* peer = new Peer(uuid, hostPeer);

	if (hostPeer) hostPeer->children.push_back(peer);

	peers.push_back(peer);

	return peer;
}

void Network::clr_peer(int uuid) {
	// Recursively Remove Peer & Children From List & Destroy

	Peer* peer = get_peer_by_uuid(uuid);

	Peer* host = peer ? peer->host : nullptr;

	while (peer && peer != host) {
		if (peer->children.size() > 0) peer = peer->children[0];
		else {
			// Remove From List

			std::vector<Peer*>::iterator iter = std::find(peers.begin(), peers.end(), peer);

			if (iter != peers.end()) peers.erase(iter);

			// Destroy Peer

			Peer* host = peer->host;

			if (host) {
				std::vector<Peer*>::iterator iter = std::find(host->children.begin(), host->children.end(), peer);

				host->children.erase(iter);
			}

			delete(peer);

			peer = host;
		}
	}


}

int Network::sock_to_uuid(int sock) {
	Peer* peer = get_peer_by_sock(sock);

	return peer ? peer->uuid : -1;
}

int Network::uuid_to_sock(int uuid) {
	Peer* peer = get_peer_by_uuid(uuid);

	return peer ? peer->socket : -1;
}

std::string Network::serialise() {
	if (peers.size() == 0) return "";

	// Find Root Node

	Peer* peer = peers[0];

	while (peer->host) peer = peer->host;

	// Recursively Serialise Peers Individually

	return peer->serialise();
}

void Network::parse(const std::string& in) {
	// Parse Received Data

	std::string buffer = "";

	int step = 0;

	Peer* peer = nullptr;

	int uuid = -1;
	int host = -1;

	for (unsigned char chr : in) {
		if (chr == ':' || chr == ' ') {
			if (step == 0) {
				if (buffer != "") uuid = stoi(buffer);
			}

			if (step == 1) {
				if (buffer != "") host = stoi(buffer);

				peer = add_peer(uuid, host);
			}

			if (step == 2) peer->name = buffer;

			if (step == 3) peer->address = buffer;

			buffer = "";

			step = chr == ' ' ? 0 : step + 1;

			continue;
		}

		buffer += chr;
	}
}

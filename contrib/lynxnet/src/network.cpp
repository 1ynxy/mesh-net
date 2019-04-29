#include "network.h"

// Member Functions

int Network::new_uuid() {
	// Generate A New UUID

	int uuid = 1;

	Peer* peer = from_uuid(uuid);

	while (peer) {
		uuid ++;

		peer = from_uuid(uuid);
	}

	return uuid;
}

Peer* Network::from_uuid(int uuid) {
	// Return First Peer With UUID

	for (Peer* peer : peers) if (peer->uuid == uuid) return peer;

	return nullptr;
}

Peer* Network::from_sock(int sock) {
	// Return First Peer With Sock

	for (Peer* peer : peers) if (peer->socket == sock) return peer;

	return nullptr;
}

Peer* Network::add_peer(int uuid, bool is_self) {
	// Create New Peer & Add To Peers

	Peer* peer = new Peer(uuid);

	if (is_self) self = peer;

	peers.push_back(peer);

	return peer;
}

void Network::clr_peer(int uuid) {
	// Recursively Remove Peer & Children From List & Destroy

	Peer* peer = from_uuid(uuid);

	if (peer) peer->remove();

	for (unsigned int i = 0; i < peers.size(); i++) {
		Peer* rem = peers[i];

		if (rem->dead) {
			peers.erase(peers.begin() + i);

			delete(rem);

			i--;
		}
	}
}

int Network::sock_to_uuid(int sock) {
	Peer* peer = from_sock(sock);

	return peer ? peer->uuid : 0;
}

int Network::uuid_to_sock(int uuid) {
	Peer* peer = from_uuid(uuid);

	return peer ? peer->socket : 0;
}

std::string Network::serialise() {
	if (peers.size() == 0) return "";

	// Find Root Node

	Peer* peer = peers[0];

	while (peer->host) peer = peer->host;

	// Recursively Serialise Peers

	return peer->serialise();
}

void Network::parse(const std::string& in) {
	// Parse Received Data

	std::string buffer = "";

	int step = 0;

	Peer* peer = nullptr;
	Peer* host = nullptr;

	for (unsigned char chr : in) {
		if (chr == ':' || chr == ' ') {
			// If Step One Add New Peer To Network

			if (step == 0) {
				if (buffer != "") {
					int peer_uuid = stoi(buffer);

					peer = add_peer(peer_uuid);
				}
			}

			// If Step Two Set New Peer Host

			if (step == 1) {
				if (buffer != "") {
					int host_uuid = stoi(buffer);

					host = from_uuid(host_uuid);

					if (peer && host) {
						peer->host = host;

						host->children.push_back(peer);
					}
				}
			}

			// Fill In Other Peer Information
			
			if (peer) {
				if (step == 2) peer->name = buffer;
				if (step == 3) peer->address = buffer;
			}

			buffer = "";

			step++;

			if (chr == ' ') {
				peer = nullptr;
				host = nullptr;

				step = 0;
			}

			continue;
		}

		buffer += chr;
	}
}

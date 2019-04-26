#include "network.h"

// Member Functions

int Network::new_uuid() {
	// Generate A New UUID

	int uuid = 0;

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

			if (peer) {
				if (step == 2) peer->name = buffer;
				if (step == 3) peer->address = buffer;
			}

			buffer = "";

			step++;

			if (chr == ' ') {
				peer = nullptr;

				uuid = -1;
				host = -1;

				step = 0;
			}

			continue;
		}

		buffer += chr;
	}
}

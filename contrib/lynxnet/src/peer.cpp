#include "peer.h"

// Contructors & Destructors

Peer::Peer(int uuid, Peer* host) : uuid(uuid), host(host) {
	
}

// Member Functions

std::string Peer::serialise() {
	std::string result = "";

	// Serialise Peer

	result += int_to_str(uuid, 3);

	result += ":";

	if (host) result += int_to_str(host->uuid, 3);

	result += ":";

	result += name;

	result += ":";

	result += address;

	result += " ";

	// Recursively Serialise Children

	for (Peer* child : children) result += child->serialise();

	return result;
}

// Global Functions

std::string int_to_str(int in, int length) {
	std::string out = std::to_string(in);

	while (out.length() < length) out = "0" + out;

	return out;
}

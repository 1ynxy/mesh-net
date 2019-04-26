#include "packet.h"

// Constructors & Destructors

Packet::Packet() {

}

Packet::Packet(int socket, int len, const char* text) : socket(socket) {
	for (unsigned int i = 0; i < len; i++) this->text += text[i];
}

Packet::Packet(int socket, const std::string& text) : socket(socket) {
	this->text = text;
}

Packet::Packet(const Packet& packet) : socket(packet.socket) {
	text = packet.text;
}

// Member Functions

std::vector<Packet> Packet::split() const {
	std::vector<Packet> packets = std::vector<Packet>();

	std::string msg = text;

	std::string buffer = "";

	for (unsigned char chr : msg) {
		if (chr == ';') {
			packets.push_back(Packet(socket, buffer));

			buffer = "";
		}
		else buffer += chr;
	}

	return packets;
}

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



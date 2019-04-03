#include "packet.h"

// Constructors & Destructors

Packet::Packet() : socket(0), len(0) {
	this->text = new char;
}

Packet::Packet(int socket, int len, const char* text) : socket(socket), len(len) {
	this->text = new char;

	for (unsigned int i = 0; i < len; i++) this->text[i] = text[i];
}

// Member Functions


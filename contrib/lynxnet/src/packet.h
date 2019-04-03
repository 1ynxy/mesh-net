#ifndef packet_h
#define packet_h

#include <string>

struct Packet {
	// Member Variables

	int socket = 0;

	std::string text = "";

	// Constructors & Destructors

	Packet();
	Packet(int socket, int len, const char* text);
	Packet(int socket, const std::string& text);
	Packet(const Packet& packet);

	// Member Functions

	
};

#endif

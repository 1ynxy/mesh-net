#ifndef packet_h
#define packet_h

struct Packet {
	// Member Variables

	int socket;

	int len;
	char* text;

	// Constructors & Destructors

	Packet();
	Packet(int socket, int len, const char* text);
};

#endif

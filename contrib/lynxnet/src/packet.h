#ifndef packet_h
#define packet_h

#include <string>

enum BroadcastType {
	BROADCAST_GLBL = 0,
	BROADCAST_MSSG = 1
};

enum PacketType {
	PCKT_GAMEDAT = 0,
	PCKT_NEWCONN = 1,
	PCKT_REMCONN = 2,
	PCKT_SETNAME = 3,
	PCKT_SETIDIP = 4,
	PCKT_NETSTAT = 5
};

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

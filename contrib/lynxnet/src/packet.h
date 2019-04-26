#ifndef packet_h
#define packet_h

#include <string>
#include <vector>

enum BroadcastType {
	BROADCAST_GLBL = 0,
	BROADCAST_MSSG = 1
};

enum PacketType {
	PCKT_GAMEDAT = 0,
	PCKT_NEWCONN = 1,
	PCKT_REMCONN = 2,
	PCKT_SETSOCK = 3,
	PCKT_SETHOST = 4,
	PCKT_SETNAME = 5,
	PCKT_SETIDIP = 6,
	PCKT_NETSTAT = 7
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

	std::vector<Packet> split() const;
};

#endif

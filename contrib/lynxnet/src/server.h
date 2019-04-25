#ifndef server_h
#define server_h

#include <vector>
#include <queue>

#include <thread>
#include <atomic>
#include <mutex>
#include <condition_variable>

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <ifaddrs.h>
#include <netdb.h>
#include <unistd.h>
#include <poll.h>

#include <../src/peer.h>
#include <../src/network.h>
#include <../src/packet.h>

class Server {
private:
	// Member Variables

	fd_set sockets;
	int sockmax = 0;

	int self = 0;
	int host = 0;

	Network network;

	std::atomic<bool> running;
	std::thread listener;
	std::thread broadcaster;

	std::condition_variable conditional;
	std::mutex condmut;

	std::queue<Packet> sendbuffer = std::queue<Packet>();
	std::mutex sendmut;

	std::queue<Packet> recvbuffer = std::queue<Packet>();
	std::mutex recvmut;

	// Member Functions

	void listen();
	void broadcast();
public:
	// Constructors & Destructors

	Server();
	~Server();

	// Member Functions

	int bind(const std::string& port);
	int connect(const std::string& addr, const std::string& port);

	bool start();

	void send_to(const Packet& message);

	void send(const std::string& text);
	bool recv(std::string& text);

	void parse(const Packet& message);

	std::string int_to_str(int in, int length);
};

#endif

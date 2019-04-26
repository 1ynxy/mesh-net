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

	std::string name = "";

	fd_set sockets;
	int sockmax = 0;

	int self_sock = 0;
	int host_sock = 0;

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
	// Member Variables

	Network network;

	// Constructors & Destructors

	Server();
	~Server();

	// Member Functions

	void set_name(const std::string& name);

	int bind(const std::string& port);
	int connect(const std::string& addr, const std::string& port);
	int disconnect(bool reconnect = false);

	bool start();

	void send_to(const Packet& message);

	void send(const std::string& text);
	bool recv(std::string& text);

	void parse(const Packet& message);
};

#endif

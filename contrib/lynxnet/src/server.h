#ifndef server_h
#define server_h

#include <thread>
#include <atomic>
#include <mutex>
#include <condition_variable>

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>

struct Message {
	// Member Variables

	int sock = -1;

	std::string text = "";

	// Constructors & Destructors

	Message() {}
	Message(int sock, const std::string& text) : sock(sock), text(text) {}
};

class Server {
private:
	// Member Variables

	std::atomic<bool> running;
	std::thread listener;
	std::thread broadcaster;

	std::condition_variable conditional;
	std::mutex condmut;

	Sockets sockets;

	std::queue<Message> sendbuffer = std::queue<Message>();
	std::mutex sendmut;

	std::queue<Message> recvbuffer = std::queue<Message>();
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

	void send_loc(const Message& message);
	void send(const Message& message);
	bool recv(Message& message);

	void this_addr(std::string& addr, std::string& port);
	void host_addr(std::string& addr, std::string& port);
};

#endif

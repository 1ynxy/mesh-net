#ifndef debug_h
#define debug_h

#include <iostream>
#include <string>
#include <queue>

#include <thread>
#include <atomic>
#include <mutex>
#include <condition_variable>

class Debug {
private:
	// Member Variables

	std::atomic<bool> running;
	std::thread loop;

	std::condition_variable conditional;
	std::mutex condmut;

	std::atomic<int> verbosity;

	std::queue<int> levels = std::queue<int>();
	std::queue<std::string> messages = std::queue<std::string>();
	std::mutex msgmut;

	// Member Functions

	void print();
	
	void add(int level, const std::string& message);
public:
	// Constructors & Destructors

	Debug(int level = 0);
	Debug(const Debug& debugger);

	~Debug();

	// Member Functions

	void set_verbosity(int level);

	void info(const std::string& message);
	void warn(const std::string& message);
	void error(const std::string& message);

	// Static Functions

	static std::string dltostr(int verbosity);
};

#endif

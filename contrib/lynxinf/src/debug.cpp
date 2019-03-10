#include "debug.h"

// Constructors & Destructors

Debug::Debug(int level) {
	running = true;

	verbosity = level;

	loop = std::thread(&Debug::print, this);
}

Debug::Debug(const Debug& debugger) {
	running = true;

	levels = debugger.levels;
	messages = debugger.messages;

	verbosity.store(debugger.verbosity);

	loop = std::thread(&Debug::print, this);
}

Debug::~Debug() {
	running = false;

	conditional.notify_one();

	loop.join();
}

// Member Functions

void Debug::print() {
	for (;;) {
		while (messages.size() > 0) {
			msgmut.lock();

			int level = levels.front();
			std::string message = messages.front();

			levels.pop();
			messages.pop();

			msgmut.unlock();

			if (level >= verbosity) {
				std::string dlstr = Debug::dltostr(level);

				std::cout << "[" << dlstr << "] " << message << std::endl;
			}
			
		}

		if (!running) return;

		std::unique_lock<std::mutex> lk(condmut);

		conditional.wait(lk);
	}
}

void Debug::add(int level, const std::string& message) {
	msgmut.lock();

	levels.push(level);
	messages.push(message);

	msgmut.unlock();

	conditional.notify_one();
}

void Debug::set_verbosity(int level) {
	verbosity = level;
}

void Debug::info(const std::string& message) {
	add(0, message);
}

void Debug::warn(const std::string& message) {
	add(1, message);
}

void Debug::error(const std::string& message) {
	add(2, message);
}

// Static Functions

std::string Debug::dltostr(int verbosity) {
	switch (verbosity) {
		case 0 : return "INFO";
		case 1 : return "WARN";
		case 2 : return "ERROR";
		default : return "NONE";
	}
}

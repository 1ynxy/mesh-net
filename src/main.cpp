#include <iostream>

#include <lynxinf.h>
#include <lynxengine.h>
#include <lynxnet.h>

void init();
void update();

// Program

Debug debug;

Config conf;

Core core;

Server server;

int main(int argc, char* argv[]) {
	int stat = 0;

	if (!conf.load("config")) {
		debug.error("failed to load configuration file");

		return 0;
	}

	core.set_callbacks(&init, &update, nullptr, nullptr, nullptr, nullptr);
	
	core.init();
}

void init() {
	core.time.set_limit(60);

	//Display::Open(glm::vec2(600, 300), glm::vec2(500, 500), "window1", Colour(40, 40, 40));

	std::string addr = conf.get_string("addr");
	std::string port = conf.get_string("port");

	int stat = 0;

	stat = server.bind(port);

	if (stat != 1) debug.error("failed to bind : " + std::to_string(stat));
	else {
		debug.info("bound");
	}

	stat = server.connect(addr, port);

	if (stat != 1) debug.error("failed to connect : " + std::to_string(stat));
	else {
		debug.info("connected");
	}

	stat = server.start();

	if (stat = 0) debug.error("failed to start server");
	else debug.info("started server");
}

void update() {
	//debug.info("delta : " + std::to_string(core.time.delta));

	Packet result;

	while (server.recv(result)) debug.info(result.text);
}

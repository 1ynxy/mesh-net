#include <iostream>

#include <lynxinf.h>
#include <lynxnet.h>
//#include <lynxengine.h>

void init();
void update();

// Program

Debug debug;

Config conf;

Server server;

int main(int argc, char* argv[]) {
	int stat = 0;

	if (!conf.load("config")) {
		debug.error("failed to load configuration file");

		return 0;
	}

	init();

	//Core::SetCallbacks(&Init, &Update, nullptr, nullptr, &OnGUI, nullptr);
	
	//Core::Init();
}

void init() {
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

	for (;;) update();
}

void update() {
	Packet result;

	while (server.recv(result)) debug.info(result.text);
}

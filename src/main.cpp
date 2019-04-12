#include <iostream>

#include <lynxengine.h>
#include <lynxnet.h>

void init();
void update();

// Program

Server server;

int main(int argc, char* argv[]) {
	int stat = 0;

	if (!core.conf.load("config")) {
		debug.error("failed to load configuration file");

		return 0;
	}

	core.set_callbacks(&init, &update, nullptr, nullptr, nullptr, nullptr);

	core.time.set_limit(60);
	
	core.init();
}

void init() {
	core.display.open(glm::vec2(600, 300), glm::vec2(500, 500), "window1", Colour(40, 40, 40));

	std::string addr = core.conf.get_string("addr");
	std::string port = core.conf.get_string("port");

	int stat = 0;

	stat = server.bind(port);
	if (stat != 1) debug.error("failed to bind listener : " + std::to_string(stat));
	else debug.info("bound listener");

	stat = server.connect(addr, port);
	if (stat != 1) debug.error("failed to connect to host : " + std::to_string(stat));
	else debug.info("connected to host");

	stat = server.start();
	if (stat != 1) debug.error("failed to start server");
	else debug.info("started server");
}

void update() {
	//debug.info("delta : " + std::to_string(core.time.delta));

	//if (core.time.tick == 500) server.send(Packet(-1, "TEST"));

	Packet result;

	while (server.recv(result)) debug.info(result.text);
}

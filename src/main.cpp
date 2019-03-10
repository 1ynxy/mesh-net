#include <lynxinf.h>
#include <lynxnet.h>
//#include <lynxengine.h>

void init();
void update();

// Program

Debug debug;

IO file;

Config conf;

Server server;

int main(int argc, char* argv[]) {
	int stat = 0;

	if (!conf.load("config")) {
		debug.error("Failed to load configuration file");

		return 0;
	}

	init();

	//Core::SetCallbacks(&Init, &Update, nullptr, nullptr, &OnGUI, nullptr);
	
	//Core::Init();
}

void init() {
	//Display::Open(glm::vec2(600, 300), glm::vec2(500, 500), "window1", Colour(40, 40, 40));

	std::string addr = conf.get_string("ip-addr");
	std::string port = conf.get_string("port");

	int stat = 0;

	stat = server.bind(port);

	if (stat != 1) debug.error("Failed to bind port : " + std::to_string(stat));
	else {
		server.this_addr(addr, port);

		debug.info("Bound to " + addr + ":" + port);
	}

	stat = server.connect(addr, port);

	if (stat != 1) debug.error("Failed to connect to host : " + std::to_string(stat));
	else {
		server.host_addr(addr, port);

		debug.info("Connected to " + addr + ":" + port);
	}

	for (;;) update();
}

void update() {
	Message result;

	while (server.recv(result)) debug.info(result.text);
}

#include <lynxengine.h>
#include <lynxnet.h>

void init();
void update();

// Program

Server server;

int main(int argc, char* argv[]) {
	// Initialise Core, Debug & Timer

	core.set_callbacks(&init, &update, nullptr, nullptr, nullptr, nullptr);

	debug.set_verbosity(core.conf.get_int("verbosity"));

	timer.set_limit(core.conf.get_int("fps-limit"));
	
	core.init();
}

void init() {
	// Start Up Server & Connect
	
	std::string addr = core.conf.get_string("addr");
	std::string port = core.conf.get_string("port");

	int stat = 0;

	if ((stat = server.bind(port)) != 1) debug.warn("failed to bind listener : " + std::to_string(stat));
	else debug.info("bound listener");

	if ((stat = server.connect(addr, port)) != 1) debug.warn("failed to connect to host : " + std::to_string(stat));
	else debug.info("connected to host");

	if ((stat = server.start()) != 1) debug.error("failed to start server");
	else debug.info("started server");

	// Open Window

	core.display.open(glm::vec2(600, 300), glm::vec2(500, 500), "window", Colour(40, 40, 40));

	// Test Resource Loader

	Shared<Sprite> sprite = resource.load<Sprite>("sprites/test");
}

void update() {
	//debug.info("delta : " + std::to_string(timer.delta));

	//if (timer.tick == 500) server.send(Packet(-1, "TEST"));

	//if (input.keyboard.key_down(GLFW_KEY_SPACE)) debug.info("space key depressed");
	//if (input.keyboard.key_up(GLFW_KEY_SPACE)) debug.info("space key released");

	Packet result;

	while (server.recv(result)) debug.info(result.text);
}

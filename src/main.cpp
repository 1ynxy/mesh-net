#include <lynxengine.h>
#include <lynxnet.h>

void init();
void update();

// Program

Server server;

Shared<Entity> cament;
Shared<Entity> objent;

int main(int argc, char* argv[]) {
	// Initialise Core

	core.load_conf("config");

	core.set_callbacks(&init, &update, nullptr, nullptr, nullptr, nullptr);

	// Initialise Timer & Debugger

	debug.set_verbosity(core.conf.get_int("verbosity"));

	timer.set_limit(core.conf.get_int("fps-limit"));

	// Start Game
	
	core.init();
}

void init() {
	// Open Window

	core.display.open(glm::vec2(600, 300), glm::vec2(500, 500), "window", Colour(40, 40, 40));

	// Setup Server

	std::string name = core.conf.get_string("name");

	server.set_name(name != "" ? name : "unnamed");

	// Start Up Server & Connect
	
	std::string addr = core.conf.get_string("addr");
	std::string port = core.conf.get_string("port");

	int stat = 0;

	if ((stat = server.bind(port)) != 1) {
		debug.error("failed to bind listener : " + std::to_string(stat));

		core.exit();

		return;
	}
	else debug.info("bound listener");

	if ((stat = server.connect(addr, port)) != 1) debug.warn("failed to connect to host : " + std::to_string(stat));
	else debug.info("connected to host");

	if ((stat = server.start()) != 1) {
		debug.error("failed to start server");

		core.exit();

		return;
	}
	else debug.info("started server");

	// Test Network

	server.send("00This is a Test Packet;");

	// Set Up Camera & Default Object

	cament = scene.instantiate("camera", glm::vec3(), glm::vec3(0, 0, 0));
	cament->add(Camera(45.0f, 0.01f, 100.0f));
	cament->add(Controller(4.0f));

	compositor.add_camera(cament->get<Camera>());

	objent = scene.instantiate("object", glm::vec3(0, 0, -5), glm::vec3());
	objent->add(MeshRenderer("shader/default", "mesh/island", "sprite/island"));
}

void update() {
	//debug.info("delta : " + std::to_string(timer.delta) + " - " + std::to_string(timer.fps));

	// Print Received Packets

	std::string result;

	while (server.recv(result)) debug.info(result);

	// Rotate Object

	Shared<Transform> transform = objent->get<Transform>();

	if (transform) transform->rotate(timer.delta * 50, transform->up());

	if (input.keyboard.key_up(GLFW_KEY_P)) debug.info(server.network.serialise());
}


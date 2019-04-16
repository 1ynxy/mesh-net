#include <lynxengine.h>
#include <lynxnet.h>

void init();
void update();

// Program

Server server;

Shared<Sprite> sprite;
Shared<Shader> shader;
Shared<Mesh> mesh;

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

	// Load Shaders

	sprite = resource.load<Sprite>("sprite/island");
	mesh = resource.load<Mesh>("mesh/island");
	shader = resource.load<Shader>("shader/default");

	// Test Cameras

	cament = scene.instantiate("camera", glm::vec3(), glm::vec3(0, 0, 0));
	cament->add(Camera(45.0f, 0.01f, 100.0f));
	cament->add(Controller(4.0f));

	compositor.add_camera(cament->get<Camera>());

	objent = scene.instantiate("object", glm::vec3(0, 0, -5), glm::vec3());
	objent->add(MeshRenderer("shader/default", "mesh/island", "sprite/island"));
}

void update() {
	//debug.info("delta : " + std::to_string(timer.delta) + " - " + std::to_string(timer.fps));

	//if (input.keyboard.key_down(GLFW_KEY_SPACE)) debug.info("space key depressed");
	//if (input.keyboard.key_up(GLFW_KEY_SPACE)) debug.info("space key released");

	//if (timer.tick == 500) server.send(Packet(-1, "TEST"));

	Packet result;

	while (server.recv(result)) debug.info(result.text);

	// Rotate Object

	Shared<Transform> transform = objent->get<Transform>();

	transform->rotate(timer.delta * 5, transform->up());
}


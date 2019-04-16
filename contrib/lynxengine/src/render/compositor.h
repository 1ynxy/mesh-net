#ifndef compositor_h
#define compositor_h

#include <vector>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>

#include <../src/ces/component/camera.h>

struct Compositor {
	// Member Variables

	std::vector<Shared<Camera>> cameras = std::vector<Shared<Camera>>();

	// Member Functions

	void add_camera(Shared<Camera> camera);
	void clr_camera(Shared<Camera> camera);

	void composite();
};

extern Compositor compositor;

#endif

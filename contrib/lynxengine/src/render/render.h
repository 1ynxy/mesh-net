#ifndef render_h
#define render_h

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>

#include <../src/resource/asset/sprite.h>
#include <../src/resource/asset/shader.h>
#include <../src/resource/asset/mesh.h>

class Render {
private:
	// Member Variables


public:
	// Member Functions

	bool sprite(glm::vec2 position, glm::vec2 size, Shared<Sprite> sprite, Shared<Shader> shader);
	bool mesh(glm::vec3 position, glm::vec3 rotation, Shared<Mesh> mesh, Shared<Shader> shader, Shared<Sprite> sprite);
};

extern Render render;

#endif

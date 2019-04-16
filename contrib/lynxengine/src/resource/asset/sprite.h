#ifndef sprite_h
#define sprite_h

#include <string>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>

#include <../src/data/colour.h>

#include <../src/resource/asset.h>

class Sprite : public Asset {
private:
	// Member Variables

	GLuint type = GL_RGBA;

	unsigned int width = 0;
	unsigned int height = 0;

	std::vector<unsigned char> image;

public:
	// Member Variables

	GLuint pos = 0;

	// Constructors & Destructors

	Sprite(const std::string& name);
	Sprite(glm::vec2 size, const Colour& fill, GLuint type = GL_RGBA);
	Sprite(const std::string& name, glm::vec2 size, const Colour& fill, GLuint type = GL_RGBA);

	~Sprite();

	// Member Functions

	void init() override;
	void load() override;
	void create(glm::vec2 size, const Colour& fill = Colour());
	void resize(glm::vec2 size, const Colour& fill = Colour());
	void upload() override;
	void unload() override;
};

#endif

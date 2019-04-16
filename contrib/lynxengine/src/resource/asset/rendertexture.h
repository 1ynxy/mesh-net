#ifndef rendertexture
#define rendertexture

#include <string>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>

#include <../src/resource/asset.h>
#include <../src/resource/asset/sprite.h>

class RenderTexture : public Asset {
public:
	// Member Variables

	Shared<Sprite> colour = nullptr;
	Shared<Sprite> depth = nullptr;

	GLuint buffer = 0;

	// Constructors & Destructors

	RenderTexture(const std::string& name);
	RenderTexture(glm::vec2 size);
	RenderTexture(const std::string& name, glm::vec2 size);

	~RenderTexture();

	// Member Functions

	void init() override;
	void load() override;
	void create(glm::vec2 size);
	void resize(glm::vec2 size);
	void upload() override;
	void unload() override;
};

#endif

#ifndef rendertexture
#define rendertexture

#include <shared.h>

#include <resource/asset.h>

#include <resource/asset/sprite.h>

class RenderTexture : public Asset {
public:
	// Member Variables

	Shared<Sprite> colour = nullptr;
	Shared<Sprite> depth = nullptr;

	glm::vec2 size;

	GLuint buffer = 0;

	// Constructors & Destructors

	RenderTexture() {}
	RenderTexture(glm::vec2 size) : size(size) {}

	// Member Functions

	void Clear(const Colour& colour);
	void Resize(glm::vec2 size);

	// Gameloop Functions

	void Init() override;
	void Generate() override;
	void Upload() override;
	void Unload() override;
};

#endif
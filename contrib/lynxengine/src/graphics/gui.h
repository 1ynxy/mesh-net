#ifndef gui_h
#define gui_h

#include <shared.h>

#include <resource/asset/shader.h>
#include <resource/asset/sprite.h>
#include <resource/asset/mesh.h>

class GUI {
public:
	// Member Variables

	static Shared<Shader> shader;
	static Shared<Mesh> mesh;

	static glm::mat4 orthographic;
	
	// Member Functions

	static void Init();

	static void Draw(glm::vec2 position, glm::vec2 size, float rotation, Shared<Shader> shader, Shared<Sprite> sprite, const Colour& colour);

	static void Draw(glm::vec2 position, glm::vec2 size, float rotation, Shared<Sprite> sprite, const Colour& colour = Colour(255));
	static bool Button(glm::vec2 position, glm::vec2 size, float rotation, Shared<Sprite> normalSprite, Shared<Sprite> hoverSprite, Shared<Sprite> activeSprite);
};

#endif
#ifndef camera_h
#define camera_h

#include <shared.h>

#include <resource/asset/rendertexture.h>

#include <ces/component.h>

#include <ces/component/transform.h>

class Camera : public Component {
private:
	// Member Variables

	float fov;
	float min;
	float max;

public:
	// Member Variables

	glm::mat4 projection = glm::mat4(1.0f);

	Shared<Transform> transform = nullptr;
	Shared<RenderTexture> target = nullptr;

	Colour colour;

	glm::vec2 position;
	glm::vec2 size;

	// Constructors & Destructors

	Camera(float fov, float min, float max, const Colour colour = Colour(0, 0, 0, 0));

	// Member Functions

	void SetFOV(float fov);
	void SetDistance(float min, float max);

	void Resize(glm::vec2 size);

	// Gameloop Functions

	void Init() override;
	void LateUpdate() override;	
};

#endif

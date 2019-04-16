#ifndef camera_h
#define camera_h

#include <glm/glm.hpp>

#include <../src/resource/asset/rendertexture.h>

#include <../src/ces/component/transform.h>

enum ProjectionType {
	PROJ_ORTHO,
	PROJ_PERSP
};

class Camera : public Component {
public:
	// Member Variables

	float fov;
	float min;
	float max;

	ProjectionType proj;

	Shared<Transform> transform = nullptr;

	Shared<RenderTexture> target = nullptr;

	// Constructors & Destructors

	Camera(float fov, float min, float max, ProjectionType proj = PROJ_PERSP);
	Camera(Shared<Entity> entity, const Camera& camera);

	~Camera();

	// Member Functions

	void init() override;
	void term() override;

	void resize(glm::vec2 size);

	glm::mat4 projection() const;
};

#endif

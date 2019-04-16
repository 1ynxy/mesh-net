#include "camera.h"

#include <../src/ces/entity.h>

// Constructors & Destructors

Camera::Camera(float fov, float min, float max, ProjectionType projection) : fov(fov), min(min), max(max), projection(projection) {
	
}

Camera::Camera(Shared<Entity> entity, const Camera& camera) : fov(camera.fov), min(camera.min), max(camera.max), projection(camera.projection) {
	this->entity = entity;

	init();
}

Camera::~Camera() {
	term();
}

// Member Functions

void Camera::init() {
	//transform = entity->get<Transform>();

	//target = new RenderTexture("camera_target", glm::vec2(1, 1));
}

void Camera::term() {
	if (target) target->destroy();
}

void Camera::resize(glm::vec2 size) {
	if (target) target->resize(size);
}

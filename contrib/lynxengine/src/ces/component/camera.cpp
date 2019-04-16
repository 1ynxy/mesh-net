#include "camera.h"

#include <../src/core/core.h>

#include <../src/ces/entity.h>

// Constructors & Destructors

Camera::Camera(float fov, float min, float max, ProjectionType proj) : fov(fov), min(min), max(max), proj(proj) {
	
}

Camera::Camera(Shared<Entity> entity, const Camera& camera) : fov(camera.fov), min(camera.min), max(camera.max), proj(camera.proj) {
	this->entity = entity;

	init();
}

Camera::~Camera() {
	term();
}

// Member Functions

void Camera::init() {
	if (entity) transform = entity->get<Transform>();

	//target = new RenderTexture("camera_target", glm::vec2(1, 1));
}

void Camera::term() {
	if (target) target->destroy();
}

void Camera::resize(glm::vec2 size) {
	if (target) target->resize(size);
}

glm::mat4 Camera::projection() const {
	glm::vec2 size = target ? target->colour->get_size() : core.display.get_size();

	if (proj == PROJ_ORTHO) return glm::ortho(0, (int) size.x, 0, (int) size.y);
	if (proj == PROJ_PERSP) return glm::perspective(45.0f, size.x / size.y, 0.01f, 100.0f);

	return glm::mat4(1.0f);
}

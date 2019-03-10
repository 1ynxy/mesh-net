#include "camera.h"

#include <graphics/display.h>

#include <ces/entity.h>

#include <resource/resource.h>

// Constructors & Destructors

Camera::Camera(float fov, float min, float max, const Colour colour) : fov(fov), min(min), max(max), colour(colour) {

}

// Member Functions

void Camera::SetFOV(float fov) {
	this->fov = fov;
}

void Camera::SetDistance(float min, float max) {
	this->min = min;
	this->max = max;
}

void Camera::Resize(glm::vec2 size) {
	if (target) {
		target->Resize(size);

		this->size = size;
	}
}

// Gameloop Functions

void Camera::Init() {
	if (entity) transform = entity->Get<Transform>();

	target = Resource::Create<RenderTexture>("target", RenderTexture(Display::Size()));
}

void Camera::LateUpdate() {
	// Calculate Matrix

	glm::vec2 size = (target && target->colour) ? target->colour->size : Display::Size();

	projection = glm::perspective(glm::radians(fov), size.x / size.y, min, max);

	projection = glm::scale(projection, glm::vec3(1, -1, 1));

	// Clear Target

	if (target) target->Clear(colour);
}
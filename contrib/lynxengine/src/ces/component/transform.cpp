#include "transform.h"

#include <core/time.h>

// Constructors & Destructors

Transform::Transform(const glm::vec3& position, const glm::vec3& rotation) : position(position), rotation(rotation) {
	
}

// Generic Gameloop Functions

void Transform::LateUpdate() {
	// Calculate Matrix
	
	matrix = glm::mat4(1.0f);
	
	matrix = glm::translate(matrix, position);
	
	matrix = glm::rotate(matrix, glm::radians(rotation.z), glm::vec3(0, 0, 1));
	matrix = glm::rotate(matrix, glm::radians(rotation.y), glm::vec3(0, 1, 0));
	matrix = glm::rotate(matrix, glm::radians(rotation.x), glm::vec3(1, 0, 0));
}

// Member Functions

void Transform::Posite(const glm::vec3& position) {
	this->position += position;
}

void Transform::Rotate(float rotation, const glm::vec3& axis) {
	this->rotation += axis * rotation;
}

glm::vec3 Transform::Up() const {
	glm::vec3 up = glm::vec3(0, 1, 0);
	
	up = glm::rotateX(up, glm::radians(rotation.x));
	up = glm::rotateY(up, glm::radians(rotation.y));
	up = glm::rotateZ(up, glm::radians(rotation.z));
	
	return up;
}

glm::vec3 Transform::Down() const {
	return -Up();
}

glm::vec3 Transform::Left() const {
	glm::vec3 left = glm::vec3(-1, 0, 0);
	
	left = glm::rotateX(left, glm::radians(rotation.x));
	left = glm::rotateY(left, glm::radians(rotation.y));
	left = glm::rotateZ(left, glm::radians(rotation.z));
	
	return left;
}

glm::vec3 Transform::Right() const {
	return -Left();
}

glm::vec3 Transform::Forward() const {
	glm::vec3 forward = glm::vec3(0, 0, -1);
	
	forward = glm::rotateX(forward, glm::radians(rotation.x));
	forward = glm::rotateY(forward, glm::radians(rotation.y));
	forward = glm::rotateZ(forward, glm::radians(rotation.z));
	
	return forward;
}

glm::vec3 Transform::Backward() const {
	return -Forward();
}

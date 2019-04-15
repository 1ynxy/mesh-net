#include "transform.h"

// Constructors & Destructors

Transform::Transform(const glm::vec3& position, const glm::vec3& rotation) : position(position), rotation(rotation) {
	init();
}

Transform::~Transform() {
	term();
}

// Member Functions

void Transform::posite(const glm::vec3& position) {
	this->position += position;
}

void Transform::rotate(float rotation, const glm::vec3& axis) {
	this->rotation += axis * rotation;
}

glm::vec3 Transform::up() const {
	glm::vec3 up = glm::vec3(0, 1, 0);
	
	up = glm::rotateX(up, glm::radians(rotation.x));
	up = glm::rotateY(up, glm::radians(rotation.y));
	up = glm::rotateZ(up, glm::radians(rotation.z));
	
	return up;
}

glm::vec3 Transform::down() const {
	return -up();
}

glm::vec3 Transform::left() const {
	glm::vec3 left = glm::vec3(-1, 0, 0);
	
	left = glm::rotateX(left, glm::radians(rotation.x));
	left = glm::rotateY(left, glm::radians(rotation.y));
	left = glm::rotateZ(left, glm::radians(rotation.z));
	
	return left;
}

glm::vec3 Transform::right() const {
	return -left();
}

glm::vec3 Transform::forward() const {
	glm::vec3 forward = glm::vec3(0, 0, -1);
	
	forward = glm::rotateX(forward, glm::radians(rotation.x));
	forward = glm::rotateY(forward, glm::radians(rotation.y));
	forward = glm::rotateZ(forward, glm::radians(rotation.z));
	
	return forward;
}

glm::vec3 Transform::backward() const {
	return -forward();
}

#include "transform.h"

#include <../src/ces/entity.h>

// Constructors & Destructors

Transform::Transform(const glm::vec3& position, const glm::vec3& rotation) : position(position), rotation(rotation) {
	this->rotation.x = glm::radians(rotation.x);
	this->rotation.y = glm::radians(rotation.y);
	this->rotation.z = glm::radians(rotation.z);
}

Transform::Transform(Shared<Entity> entity, const Transform& transform) : position(transform.position), rotation(transform.rotation) {
	this->entity = entity;
	
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
	this->rotation += axis * glm::radians(rotation);
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
	glm::vec3 forward = glm::vec3(0, 0, 1);
	
	forward = glm::rotateX(forward, glm::radians(rotation.x));
	forward = glm::rotateY(forward, glm::radians(rotation.y));
	forward = glm::rotateZ(forward, glm::radians(rotation.z));
	
	return forward;
}

glm::vec3 Transform::backward() const {
	return -forward();
}

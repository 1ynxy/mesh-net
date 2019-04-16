#include "controller.h"

#include <../src/core/time.h>
#include <../src/core/input.h>

#include <../src/ces/entity.h>

// Constructors & Destructors

Controller::Controller(float speed) : speed(speed) {

}

Controller::Controller(Shared<Entity> entity, const Controller& controller) : speed(controller.speed) {
	this->entity = entity;

	init();
}

Controller::~Controller() {
	term();
}

// Member Functions

void Controller::init() {
	if (entity) transform = entity->get<Transform>();
}

void Controller::update() {
	if (!transform) return;

	/**/

	// Relative Movement

	if (input.keyboard.key(GLFW_KEY_A)) transform->posite(transform->left() * (timer.delta * speed));
	if (input.keyboard.key(GLFW_KEY_D)) transform->posite(transform->right() * (timer.delta * speed));

	if (input.keyboard.key(GLFW_KEY_SPACE)) transform->posite(transform->up() * (timer.delta * speed));
	if (input.keyboard.key(GLFW_KEY_LEFT_SHIFT)) transform->posite(transform->down() * (timer.delta * speed));

	if (input.keyboard.key(GLFW_KEY_W)) transform->posite(transform->forward() * (timer.delta * speed));
	if (input.keyboard.key(GLFW_KEY_S)) transform->posite(transform->backward() * (timer.delta * speed));

	/**/

	/*

	// Absolute Movement

	if (input.keyboard.key(GLFW_KEY_A)) transform->posite(glm::vec3(timer.delta * speed * -1, 0, 0));
	if (input.keyboard.key(GLFW_KEY_D)) transform->posite(glm::vec3(timer.delta * speed, 0, 0));

	if (input.keyboard.key(GLFW_KEY_SPACE)) transform->posite(glm::vec3(0, timer.delta * speed, 0));
	if (input.keyboard.key(GLFW_KEY_LEFT_SHIFT)) transform->posite(glm::vec3(0, timer.delta * speed * -1, 0));

	if (input.keyboard.key(GLFW_KEY_W)) transform->posite(glm::vec3(0, 0, timer.delta * speed * -1));
	if (input.keyboard.key(GLFW_KEY_S)) transform->posite(glm::vec3(0, 0, timer.delta * speed));

	*/
}

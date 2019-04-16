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
	transform = entity->get<Transform>();
}

void Controller::update() {
	if (!transform) return;

	if (input.keyboard.key(GLFW_KEY_A)) transform->posite(transform->right() * (timer.delta * speed));
	if (input.keyboard.key(GLFW_KEY_D)) transform->posite(transform->left() * (timer.delta * speed));

	if (input.keyboard.key(GLFW_KEY_SPACE)) transform->posite(transform->up() * (timer.delta * speed));
	if (input.keyboard.key(GLFW_KEY_LEFT_SHIFT)) transform->posite(transform->down() * (timer.delta * speed));

	if (input.keyboard.key(GLFW_KEY_W)) transform->posite(transform->forward() * (timer.delta * speed));
	if (input.keyboard.key(GLFW_KEY_S)) transform->posite(transform->backward() * (timer.delta * speed));
}

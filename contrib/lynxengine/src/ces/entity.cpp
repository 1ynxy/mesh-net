#include "entity.h"

#include <../src/ces/scene.h>

// Constructors & Destructors

Entity::Entity(const std::string& name) : name(name) {
	init();
}

Entity::~Entity() {
	term();
}

// Member Functions



// Gameloop Functions

void Entity::init() {
	// Init
}

void Entity::update() {
	cleanup();

	for (Shared<Component>& component : components) if (component) component->update();
}

void Entity::lateupdate() {
	for (Shared<Component>& component : components) if (component) component->lateupdate();
}

void Entity::display() {
	for (Shared<Component>& component : components) if (component) component->display();
}

void Entity::ongui() {
	for (Shared<Component>& component : components) if (component) component->ongui();
}

void Entity::term() {
	for (Shared<Component>& component : components) if (component) component->destroy();
}

void Entity::cleanup() {
	// Cleanup Deleted Components

	for (unsigned int i = 0; i < components.size(); i++) {
		if (!components[i]) {
			components.erase(components.begin() + i);

			i--;
		}
	}
}

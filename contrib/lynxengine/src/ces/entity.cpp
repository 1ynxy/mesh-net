#include "entity.h"

// Constructors & Destructors

Entity::Entity(const std::string& name) : name(name) {

}

Entity::~Entity() {
	for (Shared<Component>& component : components) if (component) component->Destroy();
}

// Member Functions



// Gameloop Functions

void Entity::Update() {
	for (Shared<Component>& component : components) if (component) component->Update();

	// Cleanup Deleted Components

	for (unsigned int i = 0; i < components.size(); i++) {
		if (!components[i]) {
			components.erase(components.begin() + i);

			i--;
		}
	}
}

void Entity::LateUpdate() {
	for (Shared<Component>& component : components) if (component) component->LateUpdate();
}

void Entity::Display() {
	for (Shared<Component>& component : components) if (component) component->Display();
}

void Entity::OnGUI() {
	for (Shared<Component>& component : components) if (component) component->OnGUI();
}

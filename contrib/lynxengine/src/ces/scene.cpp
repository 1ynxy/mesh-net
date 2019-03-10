#include "scene.h"

// Member Variables

std::vector<Shared<Entity>> Scene::entities;

std::vector<Shared<Camera>> Scene::cameras;

// Member Functions

Shared<Entity> Scene::Instantiate(const std::string& name, const glm::vec3& position, const glm::vec3& rotation) {
	Shared<Entity> entity = Shared<Entity>(new Entity(name));

	entity->Add<Transform>(Transform(position, rotation));

	entities.push_back(entity);

	return entities.back();
}

Shared<Entity> Scene::Find(const std::string& name) {
	for (Shared<Entity>& entity : entities) if (entity && entity->name == name) return entity;

	return nullptr;
}

std::vector<Shared<Entity>> Scene::FindAll(const std::string& name) {
	std::vector<Shared<Entity>> results;

	for (Shared<Entity>& entity : entities) if (entity && entity->name == name) results.push_back(entity);

	return results;
}

// Gameloop Functions

void Scene::Init() {
	
}

void Scene::Update() {
	for (Shared<Entity>& entity : entities) if (entity) entity->Update();

	// Cleanup Deleted Entities

	for (unsigned int i = 0; i < entities.size(); i++) {
		if (!entities[i]) {
			entities.erase(entities.begin() + i);

			i--;
		}
	}
}

void Scene::LateUpdate() {
	for (Shared<Entity>& entity : entities) if (entity) entity->LateUpdate();

	// Find Cameras

	cameras = GetAll<Camera>();
}

void Scene::Display() {
	for (Shared<Entity>& entity : entities) if (entity) entity->Display();
}

void Scene::OnGUI() {
	for (Shared<Entity>& entity : entities) if (entity) entity->OnGUI();
}

void Scene::Term() {
	for (Shared<Entity>& entity : entities) if (entity) entity->Destroy();
}

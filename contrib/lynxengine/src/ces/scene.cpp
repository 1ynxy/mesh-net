#include "scene.h"

Scene scene;

// Constructors & Destructors

Scene::Scene(const std::string& name) : name(name) {
	init();
}

Scene::~Scene() {
	term();
}

// Member Functions

Shared<Entity> Scene::instantiate(const std::string& name, const glm::vec3& position, const glm::vec3& rotation) {
	Shared<Entity> entity = Shared<Entity>(new Entity(name));

	entity->scene = this;

	//entity->Add<Transform>(Transform(position, rotation));

	entities.push_back(entity);

	return entities.back();
}

Shared<Entity> Scene::find(const std::string& name) {
	for (Shared<Entity>& entity : entities) if (entity && entity->name == name) return entity;

	return nullptr;
}

std::vector<Shared<Entity>> Scene::find_all(const std::string& name) {
	std::vector<Shared<Entity>> results;

	for (Shared<Entity>& entity : entities) if (entity && entity->name == name) results.push_back(entity);

	return results;
}

// Gameloop Functions

void Scene::init() {
	// Init
}

void Scene::update() {
	cleanup();

	for (Shared<Entity>& entity : entities) if (entity) entity->update();
}

void Scene::lateupdate() {
	for (Shared<Entity>& entity : entities) if (entity) entity->lateupdate();
}

void Scene::display() {
	for (Shared<Entity>& entity : entities) if (entity) entity->display();
}

void Scene::ongui() {
	for (Shared<Entity>& entity : entities) if (entity) entity->ongui();
}

void Scene::term() {
	for (Shared<Entity>& entity : entities) if (entity) entity->destroy();
}

void Scene::cleanup() {
	// Cleanup Deleted Entities

	for (unsigned int i = 0; i < entities.size(); i++) {
		if (!entities[i]) {
			entities.erase(entities.begin() + i);

			i--;
		}
	}
}

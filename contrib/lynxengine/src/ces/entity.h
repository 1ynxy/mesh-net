#ifndef entity_h
#define entity_h

#include <string>
#include <vector>
#include <algorithm>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>

#include <../src/ces/component.h>

class Scene;

class Entity : public Sharable {
private:
	// Member Variables

	std::vector<Shared<Component>> components;

public:
	// Member Variables

	Shared<Scene> scene = nullptr;

	std::string name = "";

	// Constructors & Destructors

	Entity(const std::string& name = "entity");

	~Entity();

	// Member Functions

	template <typename T>
	Shared<T> add(const T& src = T()) {
		Shared<T> component = Shared<T>(new T(this, src));

		components.push_back(component);

		return components.back();
	}

	template <typename T>
	Shared<T> get() {
		for (Shared<Component>& component : components) {
			Shared<T> result = component;
			
			if (result) return result;
		}

		return nullptr;
	}

	template <typename T>
	std::vector<Shared<T>> get_all() {
		std::vector<Shared<T>> results;

		for (Shared<Component>& component : components) {
			Shared<T> result = component;

			if (result) results.push_back(result);
		}

		return results;
	}

	// Gameloop Functions

	void init();
	void update();
	void lateupdate();
	void display();
	void ongui();
	void term();

	void cleanup();
};

#endif

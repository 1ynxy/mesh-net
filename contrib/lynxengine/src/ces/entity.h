#ifndef entity_h
#define entity_h

#include <shared.h>

#include <ces/component.h>

class Entity : public Sharable {
private:
	// Member Variables

	std::vector<Shared<Component>> components;

public:
	// Member Variables

	std::string name = "";

	// Constructors & Destructors

	Entity(const std::string& name = "entity");

	~Entity();

	// Member Functions

	template <typename T>
	Shared<T> Add(const T& src = T()) {
		Shared<T> component = Shared<T>(new T(src));

		component->entity = this;

		component->Init();

		components.push_back(component);

		return components.back();
	}

	template <typename T>
	Shared<T> Get() {
		for (Shared<Component>& component : components) {
			Shared<T> result = component;
			
			if (result) return result;
		}

		return nullptr;
	}

	template <typename T>
	std::vector<Shared<T>> GetAll() {
		std::vector<Shared<T>> results;

		for (Shared<Component>& component : components) {
			Shared<T> result = component;

			if (result) results.push_back(result);
		}

		return results;
	}

	// Gameloop Functions

	void Update();
	void LateUpdate();
	void Display();
	void OnGUI();
};

#endif
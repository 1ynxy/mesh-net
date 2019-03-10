#ifndef scene_h
#define scene_h

#include <shared.h>

#include <ces/entity.h>

#include <ces/component/transform.h>
#include <ces/component/camera.h>

class Scene {
private:
	// Member Variables

	static std::vector<Shared<Entity>> entities;

public:
	// Member Variables

	static std::vector<Shared<Camera>> cameras;

	// Member Functions

	static Shared<Entity> Instantiate(const std::string& name, const glm::vec3& position = glm::vec3(), const glm::vec3& rotation = glm::vec3());
	static Shared<Entity> Find(const std::string& name);
	static std::vector<Shared<Entity>> FindAll(const std::string& name);

	template <typename T>
	static Shared<T> Get() {
		for (Shared<Entity>& entity : entities) {
			Shared<T> component = entity->Get<T>();

			if (component) return component;
		}
	}

	template <typename T>
	static std::vector<Shared<T>> GetAll() {
		std::vector<Shared<T>> results;

		for (Shared<Entity>& entity : entities) {
			std::vector<Shared<T>> components = entity->GetAll<T>();
			
			results.insert(results.end(), components.begin(), components.end());
		}
		
		return results;
	}

	// Gameloop Functions

	static void Init();
	static void Update();
	static void LateUpdate();
	static void Display();
	static void OnGUI();
	static void Term();
};

#endif
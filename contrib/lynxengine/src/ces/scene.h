#ifndef scene_h
#define scene_h

#include <string>
#include <vector>
#include <algorithm>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>

#include <../src/ces/entity.h>

class Scene : public Sharable {
private:
	// Member Variables

	std::vector<Shared<Entity>> entities;

public:
	// Member Variables

	std::string name = "";

	// Constructors & Destructors

	Scene(const std::string& name = "scene");

	~Scene();

	// Member Functions

	Shared<Entity> instantiate(const std::string& name, const glm::vec3& position = glm::vec3(), const glm::vec3& rotation = glm::vec3());
	
	Shared<Entity> find(const std::string& name);
	
	std::vector<Shared<Entity>> find_all(const std::string& name);

	// Gameloop Functions

	void init();
	void update();
	void lateupdate();
	void display();
	void ongui();
	void term();

	void cleanup();
};

extern Scene scene;

#endif

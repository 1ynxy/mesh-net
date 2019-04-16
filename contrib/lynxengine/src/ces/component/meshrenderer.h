#ifndef meshrenderer_h
#define meshrenderer_h

#include <string>

#include <glm/glm.hpp>

#include <../src/resource/asset/sprite.h>
#include <../src/resource/asset/mesh.h>
#include <../src/resource/asset/shader.h>	

#include <../src/ces/component/transform.h>

class MeshRenderer : public Component {
public:
	// Member Variables

	std::string meshName = "";
	std::string shaderName = "";
	std::string spriteName = "";

	Shared<Mesh> mesh = nullptr;
	Shared<Shader> shader = nullptr;
	Shared<Sprite> sprite = nullptr;

	Shared<Transform> transform = nullptr;

	// Constructors & Destructors

	MeshRenderer(const std::string& shaderName, const std::string& meshName, const std::string& spriteName = "");
	MeshRenderer(Shared<Entity> entity, const MeshRenderer& meshrenderer);

	~MeshRenderer();

	// Gameloop Functions

	void init() override;
	void display() override;
};

#endif

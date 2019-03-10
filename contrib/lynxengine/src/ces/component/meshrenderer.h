#ifndef meshrenderer_h
#define meshrenderer_h

#include <shared.h>

#include <resource/resource.h>

#include <resource/asset/shader.h>
#include <resource/asset/sprite.h>
#include <resource/asset/mesh.h>

#include <ces/component.h>

#include <ces/component/transform.h>
#include <ces/component/camera.h>

class MeshRenderer : public Component {
private:
	// Member Variables

	std::string shaderName = "";
	std::string spriteName = "";
	std::string meshName = "";

public:
	// Member Variables

	Shared<Shader> shader = nullptr;
	Shared<Sprite> sprite = nullptr;
	Shared<Mesh> mesh = nullptr;

	Shared<Transform> transform = nullptr;

	// Constructors & Destructors

	MeshRenderer(const std::string& shaderName, const std::string& meshName, const std::string& spriteName = "");

	// Gameloop Functions

	void Init() override;
	void Display() override;
};

#endif

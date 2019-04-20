#include "meshrenderer.h"

#include <../src/resource/resource.h>

#include <../src/render/render.h>
#include <../src/render/compositor.h>

#include <../src/ces/entity.h>

// Constructors & Destructors

MeshRenderer::MeshRenderer(const std::string& shaderName, const std::string& meshName, const std::string& spriteName) : shaderName(shaderName), meshName(meshName), spriteName(spriteName) {

}

MeshRenderer::MeshRenderer(Shared<Entity> entity, const MeshRenderer& meshrenderer) : shaderName(meshrenderer.shaderName), meshName(meshrenderer.meshName), spriteName(meshrenderer.spriteName) {
	this->entity = entity;

	init();
}

MeshRenderer::~MeshRenderer() {
	term();
}

// Gameloop Functions

void MeshRenderer::init() {
	if (entity) transform = entity->get<Transform>();

	if (spriteName != "") sprite = resource.load<Sprite>(spriteName);
	if(meshName != "") mesh = resource.load<Mesh>(meshName);
	if (shaderName != "") shader = resource.load<Shader>(shaderName);
}

void MeshRenderer::display() {
	for (Shared<Camera> camera : compositor.cameras) {
		render.set_camera(camera);

		render.mesh(transform, mesh, shader, sprite);
	}
}

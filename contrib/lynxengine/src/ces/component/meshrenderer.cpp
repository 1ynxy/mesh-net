#include "meshrenderer.h"

#include <graphics/display.h>

#include <ces/scene.h>
#include <ces/entity.h>

// Constructors & Destructors

MeshRenderer::MeshRenderer(const std::string& shaderName, const std::string& meshName, const std::string& spriteName) : shaderName(shaderName), meshName(meshName), spriteName(spriteName) {

}

// Gameloop Functions

void MeshRenderer::Init() {
	shader = Resource::Load<Shader>(shaderName);
	sprite = Resource::Load<Sprite>(spriteName);
	mesh = Resource::Load<Mesh>(meshName);

	if (entity) transform = entity->Get<Transform>();
}

void MeshRenderer::Display() {
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);

	if (!shader || !shader->Ready()) return;
	glUseProgram(shader->program);
	
	if (!mesh || !mesh->Ready()) return;
	glBindVertexArray(mesh->vao);
	
	if (sprite && sprite->Ready()) {
		glActiveTexture(GL_TEXTURE0 + sprite->pos);
		glBindTexture(GL_TEXTURE_2D, sprite->pos);
		
		shader->Set("diffuseSampler", sprite->pos);
	}

	if (transform) shader->Set("modelMat", transform->matrix);

	for (Shared<Camera>& camera : Scene::cameras) {
		if (!camera) continue;

		if (camera->target) {
			glBindFramebuffer(GL_FRAMEBUFFER, camera->target->buffer);

			glViewport(0, 0, (int) camera->size.x, (int) camera->size.y);
		}
		else {
			glBindFramebuffer(GL_FRAMEBUFFER, 0);

			glViewport(0, 0, (int) Display::Size().x, (int) Display::Size().y);
		}

		if (camera->transform) shader->Set("viewMat", glm::inverse(camera->transform->matrix));
		
		shader->Set("projMat", camera->projection);

		glDrawArrays(GL_TRIANGLES, 0, mesh->size);
	}

	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
}
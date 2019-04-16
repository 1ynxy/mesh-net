#include "render.h"

#include <../src/core/core.h>

Render render;

// Member Functions

void Render::camera(Shared<Camera> camera) {
	glm::vec2 size;

	if (camera->target && camera->target->ready()) {
		glBindFramebuffer(GL_FRAMEBUFFER, camera->target->buffer);

		size = camera->target->colour->get_size();
	}
	else {
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		size = core.display.get_size();
	}

	glViewport(0, 0, (int) size.x, (int) size.y);

	if (camera->projection == PROJ_ORTHO) projection = glm::ortho(0, (int) size.x, 0, (int) size.y);
	else projection = glm::perspective(45.0f, core.display.aspect_ratio(), 0.01f, 100.0f);

	if (camera->transform) {
		glm::vec3 position = camera->transform->position;
		glm::vec3 rotation = camera->transform->rotation;

		view = glm::translate(glm::mat4(1.0f), position);

		view = glm::rotate(view, rotation.x, glm::vec3(0, 0, 1));
		view = glm::rotate(view, rotation.y, glm::vec3(0, 1, 0));
		view = glm::rotate(view, rotation.z, glm::vec3(1, 0, 0));

		view = glm::inverse(view);
	}
}

bool Render::sprite(glm::vec2 position, glm::vec2 size, Shared<Sprite> sprite, Shared<Shader> shader) {


	return true;
}

bool Render::mesh(glm::vec3 position, glm::vec3 rotation, Shared<Mesh> mesh, Shared<Shader> shader, Shared<Sprite> sprite) {
	glm::vec2 screenSize = core.display.get_size();
	
	// Generate Matrix

	glm::mat4 transform = glm::mat4(1.0f);
	
	transform = glm::translate(transform, position);
	
	transform = glm::rotate(transform, glm::radians(rotation.z), glm::vec3(0, 0, 1));
	transform = glm::rotate(transform, glm::radians(rotation.y), glm::vec3(0, 1, 0));
	transform = glm::rotate(transform, glm::radians(rotation.x), glm::vec3(1, 0, 0));

	// Draw

	glCullFace(GL_BACK);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	if (!mesh || !mesh->ready()) return false;

	if (mesh != meshCache) {
		glBindVertexArray(mesh->vao);

		meshCache = mesh;
	}

	if (!shader || !shader->ready()) return false;

	if (shader != shaderCache) {
		glUseProgram(shader->program);

		shaderCache = shader;
	}
	
	if (sprite && sprite->ready()) {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, sprite->pos);
		
		shader->set("diffuseSampler", sprite->pos);
	}

	shader->set("modelMat", transform);
	shader->set("viewMat", view);
	shader->set("projMat", projection);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glViewport(0, 0, (int) screenSize.x, (int) screenSize.y);

	glDrawArrays(GL_TRIANGLES, 0, mesh->size);

	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
	
	return true;
}

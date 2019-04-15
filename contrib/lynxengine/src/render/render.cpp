#include "render.h"

#include <../src/core/core.h>

Render render;

// Member Functions

/*

bool Render::camera(Shared<Camera> camera) {
	if (camera->target) {
		glBindFramebuffer(GL_FRAMEBUFFER, camera->target->pos);

		glViewport(0, 0, (int) camera->target->size.x, (int) camera->target->size.y);
	}
	else {
		glm::vec2 screenSize = core.display.get_size();

		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		glViewport(0, 0, (int) screenSize.x, (int) screenSize.y);
	}
}

*/

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

	glm::mat4 view = glm::mat4(1.0f);

	view = glm::translate(view, glm::vec3(0, 0, 0));

	view = glm::rotate(view, glm::radians(0.0f), glm::vec3(0, 0, 1));
	view = glm::rotate(view, glm::radians(0.0f), glm::vec3(0, 1, 0));
	view = glm::rotate(view, glm::radians(0.0f), glm::vec3(1, 0, 0));
	
	glm::mat4 projection = glm::perspective(45.0f, core.display.aspect_ratio(), 0.01f, 100.0f);

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
	shader->set("viewMat", glm::inverse(view));
	shader->set("projMat", projection);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glViewport(0, 0, (int) screenSize.x, (int) screenSize.y);

	glDrawArrays(GL_TRIANGLES, 0, mesh->size);

	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
	
	return true;
}

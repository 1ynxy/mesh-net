#include "gui.h"

#include <core/input.h>

#include <graphics/display.h>

#include <resource/resource.h>

// Member Variables

Shared<Shader> GUI::shader = nullptr;
Shared<Mesh> GUI::mesh = nullptr;

glm::mat4 GUI::orthographic = glm::mat4(1.0f);

// Member Functions

void GUI::Init() {
	shader = Resource::Load<Shader>("gui/default");
	
	#include <data/plane.h>
	
	mesh = Resource::Create<Mesh>("plane", Mesh(vertices, normals, uvcoords));
}

void GUI::Draw(glm::vec2 position, glm::vec2 size, float rotation, Shared<Shader> shader, Shared<Sprite> sprite, const Colour& colour) {
	// Setup Display

	glm::vec2 displaySize = Display::Size();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glViewport(0, 0, (int) displaySize.x, (int) displaySize.y);

	orthographic = glm::ortho(0.0f, (float) displaySize.x, (float) displaySize.y, 0.0f, -1.0f, 1.0f);

	// Bind Shader, Mesh & Sprite

	if (!shader || !shader->Ready()) return;
	glUseProgram(shader->program);

	if (!mesh || !mesh->Ready()) return;
	glBindVertexArray(mesh->vao);

	if (sprite && sprite->Ready()) {
		glActiveTexture(GL_TEXTURE0 + sprite->pos);
		glBindTexture(GL_TEXTURE_2D, sprite->pos);

		shader->Set("diffuse", sprite->pos);
		shader->Set("colour", colour.Vec4());
	}

	// Generate Model Matrix
	
	glm::mat4 matrix = glm::mat4(1.0f);
	
	matrix = glm::translate(matrix, glm::vec3(position, 0.0f));
	
	matrix = glm::translate(matrix, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f));
	matrix = glm::rotate(matrix, glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f));
	matrix = glm::translate(matrix, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f));
	
	matrix = glm::scale(matrix, glm::vec3(size, 1.0f));

	shader->Set("modelMat", matrix);
	shader->Set("projMat", orthographic);

	// Render

	glDrawArrays(GL_TRIANGLES, 0, mesh->size);
}

void GUI::Draw(glm::vec2 position, glm::vec2 size, float rotation, Shared<Sprite> sprite, const Colour& colour) {
	Draw(position, size, rotation, shader, sprite, colour);
}

bool GUI::Button(glm::vec2 position, glm::vec2 size, float rotation, Shared<Sprite> normalSprite, Shared<Sprite> hoverSprite, Shared<Sprite> activeSprite) {
	bool hover = true, active = false;
	
	glm::vec2 mouse = Mouse::position;

	// Rotate Mouse Position Around Centre

	if (rotation != 0) {
		glm::vec2 centre = position + glm::vec2(size.x / 2, size.y / 2);

		mouse -= centre;

		float rad = glm::radians(-rotation);

		float cs = cos(rad);
		float sn = sin(rad);

		mouse = glm::vec2(mouse.x * cs - mouse.y * sn, mouse.x * sn + mouse.y * cs);

		mouse += centre;
	}
	
	// Check For Collision

	if (mouse.x < position.x || mouse.x > position.x + size.x) hover = false;
	if (mouse.y < position.y || mouse.y > position.y + size.y) hover = false;

	if (hover) active = Mouse::Button(GLFW_MOUSE_BUTTON_1);

	Draw(position, size, rotation, active ? activeSprite : hover ? hoverSprite : normalSprite);

	return active;
}
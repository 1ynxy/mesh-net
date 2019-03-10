#include "rendertexture.h"

#include <resource/resource.h>

// Member Functions

void RenderTexture::Clear(const Colour& colour) {
	glBindFramebuffer(GL_FRAMEBUFFER, buffer);

	glClearColor(colour.r / 255.0f, colour.g / 255.0f, colour.b / 255.0f, colour.a / 255.0f);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void RenderTexture::Resize(glm::vec2 size) {
	if (colour) {
		colour->Resize(size);
		colour->Update();
	}
	if (depth) {
		depth->Resize(size);
		depth->Update();
	}
}

// Gameloop Functions

void RenderTexture::Init() {
	state = RES_INITIALISING;

	colour = Resource::Create<Sprite>("colour", Sprite(size, GL_RGBA));
	depth = Resource::Create<Sprite>("depth", Sprite(size, GL_DEPTH_COMPONENT));

	glGenFramebuffers(1, &buffer);

	if (!buffer) {
		Debug::Error("RESOURCE", "failed to generate framebuffer for \"" + name + "\"");

		state = RES_INVALID;

		return;
	}

	state = RES_INITIALISED;
}

void RenderTexture::Generate() {
	state = RES_LOADING;

	if (!colour || !depth) {
		Debug::Error("RESOURCE", "failed to create either colour or depth texture for framebuffer");

		state = RES_INVALID;

		return;
	}

	while (!colour->Ready() || !depth->Ready());

	state = RES_LOADED;
}

void RenderTexture::Upload() {
	state = RES_UPLOADING;

	// Upload Data To GPU

	glBindFramebuffer(GL_FRAMEBUFFER, buffer);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colour->pos, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depth->pos, 0);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		Debug::Error("RESOURCE", "framebuffer status check returned incomplete for \"" + name + "\"");

		state = RES_INVALID;

		return;
	}

	state = RES_READY;
}

void RenderTexture::Unload() {
	state = RES_UNLOADING;

	// Do Unload Data Here

	glDeleteFramebuffers(1, &buffer);

	state = RES_INVALID;
}

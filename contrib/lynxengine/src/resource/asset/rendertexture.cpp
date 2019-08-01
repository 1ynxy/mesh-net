#include "rendertexture.h"

#include <../src/core/core.h>
#include <../src/core/debug.h>
#include <../src/core/file.h>
#include <../src/core/config.h>

// Constructors & Destructors

RenderTexture::RenderTexture(const std::string& name) {
	this->name = name;

	init();
}

RenderTexture::RenderTexture(glm::vec2 size) {
	this->name = "created_rendertexture";

	init();

	create(size);
}

RenderTexture::RenderTexture(const std::string& name, glm::vec2 size) {
	this->name = name;

	init();

	create(size);
}

RenderTexture::~RenderTexture() {
	unload();
}

// Member Functions

void RenderTexture::init() {
	state = ASSET_WAITING;

	// Init Asset Data

	glGenFramebuffers(1, &buffer);

	if (!buffer) {
		debug.error("failed to generate framebuffer for \"" + name + "\"");

		state = ASSET_INVALID;

		return;
	}

	state = ASSET_INIT;
}

void RenderTexture::load() {
	debug.warn("cannot load resource of type framebuffer : can only be created");

	state = ASSET_INVALID;
}

void RenderTexture::create(glm::vec2 size) {
	state = ASSET_WAITING;

	created = true;

	// Create Asset Data

	colour = new Sprite("rendertexture_colour", size, Colour(), GL_RGBA);
	depth = new Sprite("rendertexture_depth", size, Colour(), GL_DEPTH_COMPONENT);

	if (!colour || !colour->ready() || !depth || !depth->ready()) {
		debug.error("failed to create either colour or depth texture for framebuffer");

		state = ASSET_INVALID;

		return;
	}

	state = ASSET_LOADED;

	upload();
}

void RenderTexture::resize(glm::vec2 size) {
	if (colour) colour->resize(size);
	if (depth) depth->resize(size);
}

void RenderTexture::upload() {
	state = ASSET_WAITING;

	// Upload Asset To GPU

	glBindFramebuffer(GL_FRAMEBUFFER, buffer);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colour->pos, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depth->pos, 0);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		debug.error("framebuffer status check returned incomplete for \"" + name + "\"");

		state = ASSET_INVALID;

		return;
	}

	state = ASSET_READY;

	debug.info(std::string(created ? "created" : "loaded") + " rendertexture \"" + name + "\"");
}

void RenderTexture::unload() {
	state = ASSET_WAITING;

	// Unload Asset Data

	glDeleteFramebuffers(1, &buffer);

	if (colour) colour->destroy();
	if (depth) depth->destroy();

	state = ASSET_INVALID;
}


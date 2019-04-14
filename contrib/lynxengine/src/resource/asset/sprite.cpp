#include "sprite.h"

#include <../src/resource/asset/req/lodepng.h>

#include <../src/core/core.h>
#include <../src/core/debug.h>
#include <../src/core/file.h>
#include <../src/core/config.h>

// Constructors & Destructors

Sprite::Sprite(const std::string& name) {
	this->name = name;

	init();
}

Sprite::~Sprite() {
	unload();
}

// Member Functions

void Sprite::init() {
	state = ASSET_WAITING;

	// Init Asset Data

	glGenTextures(1, &pos);

	if (!pos) {
		debug.error("failed to generate texture for \"" + name + "\"");

		state = ASSET_INVALID;

		return;
	}

	state = ASSET_INIT;
}

void Sprite::load() {
	state = ASSET_WAITING;

	// Load Asset Data

	std::string path = core.conf.get_string("resource-loc") + name + ".png";

	if (!file.read(path, data)) {
		debug.error("failed to read data from \"" + name + "\"");

       	state = ASSET_INVALID;
		
		return;
   	}

	std::vector<unsigned char> png;	

	for (unsigned char chr : data) png.push_back(chr);

	// Do Parse Data Here

	unsigned int width = 0, height = 0;

	unsigned int error = lodepng::decode(image, width, height, png);

	size = glm::vec2(width, height);

	if (error) {
		debug.error("failed to parse data from \"" + name + "\"");

		state = ASSET_INVALID;

		return;
	}

	state = ASSET_LOADED;
}

void Sprite::upload() {
	state = ASSET_WAITING;

	// Upload Asset To GPU

	glBindTexture(GL_TEXTURE_2D, pos);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, (int) size.x, (int) size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, &image.front());

	glGenerateMipmap(GL_TEXTURE_2D);

	state = ASSET_READY;
}

void Sprite::unload() {
	state = ASSET_WAITING;

	// Unload Asset Data

	glDeleteTextures(1, &pos);

	state = ASSET_INVALID;
}

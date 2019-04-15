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
		debug.error("failed to read data from \"" + path + "\"");

	   	state = ASSET_INVALID;
		
		return;
   	}
	
	// Decode Data

	int error = lodepng::decode(image, width, height, file.split(data));

	if (error) {
		debug.error("failed to parse data from \"" + path + "\" : " + std::to_string(error));

		state = ASSET_INVALID;

		return;
	}

	state = ASSET_LOADED;
}

void Sprite::upload() {
	state = ASSET_WAITING;

	// Upload Asset To GPU

	//glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, pos);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &image.front());

	glGenerateMipmap(GL_TEXTURE_2D);

	state = ASSET_READY;

	debug.info("loaded sprite \"" + name + "\"");
}

void Sprite::unload() {
	state = ASSET_WAITING;

	// Unload Asset Data

	glDeleteTextures(1, &pos);

	state = ASSET_INVALID;
}

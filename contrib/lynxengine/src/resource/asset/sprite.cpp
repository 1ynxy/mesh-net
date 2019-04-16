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

Sprite::Sprite(glm::vec2 size, const Colour& fill, GLuint type) : type(type) {
	this->name = "created_sprite";

	init();

	create(size, fill);
}

Sprite::Sprite(const std::string& name, glm::vec2 size, const Colour& fill, GLuint type) : type(type) {
	this->name = name;

	init();

	create(size, fill);
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

	unsigned int width;
	unsigned int height;

	int error = lodepng::decode(image, width, height, file.split(data));

	if (error) {
		debug.error("failed to parse data from \"" + path + "\" : " + std::to_string(error));

		state = ASSET_INVALID;

		return;
	}

	size.x = width;
	size.y = height;

	state = ASSET_LOADED;
}

void Sprite::create(glm::vec2 size, const Colour& fill) {
	state = ASSET_WAITING;

	created = true;

	// Create Asset Data

	image.clear();

	this->size = size;

	for (unsigned int i = 0; i < size.x * size.y; i++) {
		image.push_back(fill.r);
		image.push_back(fill.g);
		image.push_back(fill.b);
		image.push_back(fill.a);
	}

	state = ASSET_LOADED;

	upload();
}

void Sprite::resize(glm::vec2 size, const Colour& fill) {
	if (this->size.x == size.x && this->size.y == size.y) return;

	create(size, fill);
}

void Sprite::upload() {
	state = ASSET_WAITING;

	// Upload Asset To GPU

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, pos);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glTexImage2D(GL_TEXTURE_2D, 0, type, (int) size.x, (int) size.y, 0, type, GL_UNSIGNED_BYTE, &image.front());

	glGenerateMipmap(GL_TEXTURE_2D);

	state = ASSET_READY;

	debug.info(std::string(created ? "created" : "loaded") + " sprite \"" + name + "\"");
}

void Sprite::unload() {
	state = ASSET_WAITING;

	// Unload Asset Data

	glDeleteTextures(1, &pos);

	state = ASSET_INVALID;
}

glm::vec2 Sprite::get_size() {
	return size;
}

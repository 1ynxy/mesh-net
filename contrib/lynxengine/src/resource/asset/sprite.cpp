#include "sprite.h"

#include <lodepng/lodepng.h>

// Member Functions

void Sprite::Clear(Colour colour) {
	for (unsigned int i = 0; i < image.size(); i += 4) {
		image[i + 0] = colour.r;
		image[i + 1] = colour.g;
		image[i + 2] = colour.b;
		image[i + 2] = colour.a;
	}
}

void Sprite::Add(glm::vec2 size, const std::vector<float>& src) {
	image.insert(image.end(), src.begin(), src.end());

	this->size = size;
}

void Sprite::Resize(glm::vec2 size) {
	if (this->size == size) return;

	this->size = size;

	image.resize(4 * (int) size.x * (int) size.y, 255);
}

// Gameloop Functions

void Sprite::Init() {
	state = RES_INITIALISING;

	glGenTextures(1, &pos);

	if (!pos) {
		Debug::Error("RESOURCE", "failed to generate texture for \"" + name + "\"");

		state = RES_INVALID;

		return;
	}

	state = RES_INITIALISED;
}

void Sprite::Load() {
	state = RES_LOADING;

	// Do Load Data Here

	std::string path = Config::GetS("sprite-assets") + name + ".png";

	if (!File::Read(path, data)) {
       	state = RES_INVALID;
		
		return;
   	}

	// Do Parse Data Here

	unsigned int width = 0, height = 0;

	unsigned int error = lodepng::decode(image, width, height, File::CharList(data));

	size = glm::vec2(width, height);

	if (error) {
		state = RES_INVALID;

		return;
	}

	state = RES_LOADED;
}

void Sprite::Generate() {
	state = RES_LOADING;

	// Do Generate Data Here

	image = std::vector<unsigned char>(4 * (int) size.x * (int) size.y, 0);

	state = RES_LOADED;
}

void Sprite::Upload() {
	state = RES_UPLOADING;

	// Upload Data To GPU

	glBindTexture(GL_TEXTURE_2D, pos);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glTexImage2D(GL_TEXTURE_2D, 0, format, (int) size.x, (int) size.y, 0, format, GL_UNSIGNED_BYTE, &image.front());

	glGenerateMipmap(GL_TEXTURE_2D);

	if (!hidden) Debug::Info("RESOURCE", "loaded sprite \"" + name + "\"");

	state = RES_READY;
}

void Sprite::Update() {
	state = RES_UPLOADING;

	// Upload Data To GPU
	
	glBindTexture(GL_TEXTURE_2D, pos);

	glTexImage2D(GL_TEXTURE_2D, 0, format, (int) size.x, (int) size.y, 0, format, GL_UNSIGNED_BYTE, &image.front());

	glGenerateMipmap(GL_TEXTURE_2D);

	state = RES_READY;
}

void Sprite::Unload() {
	state = RES_UNLOADING;

	// Do Unload Data Here

	glDeleteTextures(1, &pos);

	state = RES_INVALID;
}

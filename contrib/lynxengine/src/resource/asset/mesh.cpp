#include "mesh.h"

#include <../src/core/core.h>
#include <../src/core/debug.h>
#include <../src/core/file.h>
#include <../src/core/config.h>

// Constructors & Destructors

Mesh::Mesh(const std::string& name) {
	this->name = name;

	init();
}

Mesh::~Mesh() {
	unload();
}

// Member Functions

void Mesh::init() {
	state = ASSET_WAITING;

	// Init Asset Data

	glGenVertexArrays(1, &vao);

	if (!vao) {
		debug.error("failed to generate VAO for \"" + name + "\"");

		state = ASSET_INVALID;

		return;
	}

	state = ASSET_INIT;
}

void Mesh::load() {
	state = ASSET_WAITING;

	// Load Asset Data

	std::string path = core.conf.get_string("resource-loc") + name + ".obj";

	if (!file.read(path, data)) {
		debug.error("failed to read data from \"" + path + "\"");

	   	state = ASSET_INVALID;
		
		return;
   	}

	std::stringstream stream(data);

	std::string line = "";

	std::vector<float> vert;
	std::vector<float> norm;
	std::vector<float> uvc;

	while (getline(stream, line)) {
		std::string type = "";

		for (std::string word : file.split(line, ' ')) {
			if (type == "") type = word;
			else {
				if (type == "f") {
					std::vector<std::string> vals = file.split(word, '/');

					if (vals.size() != 3) continue;

					if (vals[0] != "") {
						unsigned int index = (std::stoi(vals[0]) - 1) * 3;
						vertices.insert(vertices.end(), { vert[index], vert[index + 1], vert[index + 2] });
					}

					if (vals[1] != "") {
						unsigned int index = (std::stoi(vals[1]) - 1) * 2;
						uvcoords.insert(uvcoords.end(), { uvc[index], uvc[index + 1] });
					}

					if (vals[2] != "") {
						unsigned int index = (std::stoi(vals[2]) - 1) * 3;
						normals.insert(normals.end(), { norm[index], norm[index + 1], norm[index + 2] });
					}
				}
				else if (type == "v") vert.push_back(std::stof(word.c_str(), nullptr));
				else if (type == "vn") norm.push_back(std::stof(word.c_str(), nullptr));
				else if (type == "vt") uvc.push_back(std::stof(word.c_str(), nullptr));
			}
		}
	}

	size = vertices.size();

	state = ASSET_LOADED;
}

void Mesh::upload() {
	state = ASSET_WAITING;

	// Upload Asset To GPU

	glBindVertexArray(vao);

	if (!gen_buffer(vertBuf, 0, 3, &vertices) || !gen_buffer(normBuf, 1, 3, &normals) || !gen_buffer(uvcBuf, 2, 2, &uvcoords)) {
		debug.error("failed to generate one or more buffers for \"" + name + "\"");

		state = ASSET_INVALID;

		return;
	}

	state = ASSET_READY;

	debug.info("loaded mesh \"" + name + "\"");
}

void Mesh::unload() {
	state = ASSET_WAITING;

	// Unload Asset Data

	glDeleteVertexArrays(1, &vao);

	glDeleteBuffers(1, &vertBuf);
	glDeleteBuffers(1, &normBuf);
	glDeleteBuffers(1, &uvcBuf);

	state = ASSET_INVALID;
}

bool Mesh::gen_buffer(GLuint& buffer, unsigned int position, unsigned int count, std::vector<float>* dat) {
	bool exists = buffer ? true : false;
	
	if (!exists) glGenBuffers(1, &buffer);

	if (!buffer) {
		debug.error("failed to generate VBO for \"" + name + "\"");

		return false;
	}

	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * dat->size(), &dat->front(), GL_DYNAMIC_DRAW);
	
	if (!exists) {
		glVertexAttribPointer(position, count, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(position);
	}

	return true;
}

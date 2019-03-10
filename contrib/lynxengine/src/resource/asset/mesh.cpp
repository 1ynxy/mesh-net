#include "mesh.h"

// Member Functions

bool Mesh::GenBuffer(GLuint& buffer, unsigned int position, unsigned int count, std::vector<float>* dat) {
	bool exists = buffer ? true : false;
	
	if (!exists) glGenBuffers(1, &buffer);

	if (!buffer) {
		Debug::Error("RESOURCE", "failed to generate VBO for \"" + name + "\"");

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

void Mesh::Clear() {
	state = RES_LOADING;

	vertices.clear();
	normals.clear();
	uvcoords.clear();

	size = 0;

	state = RES_LOADED;
}

void Mesh::Add(std::vector<float> vertices, std::vector<float> normals, std::vector<float> uvcoords) {
	state = RES_LOADING;
	
	this->vertices.insert(this->vertices.end(), vertices.begin(), vertices.end());
	this->normals.insert(this->normals.end(), normals.begin(), normals.end());
	this->uvcoords.insert(this->uvcoords.end(), uvcoords.begin(), uvcoords.end());

	size += vertices.size();

	state = RES_LOADED;
}

// Gameloop Functions

void Mesh::Init() {
	state = RES_INITIALISING;

	glGenVertexArrays(1, &vao);

	if (!vao) {
		Debug::Error("RESOURCE", "failed to generate VAO for \"" + name + "\"");

		state = RES_INVALID;

		return;
	}

	state = RES_INITIALISED;
}

void Mesh::Load() {
	state = RES_LOADING;

	// Do Load Data Here

	std::string path = Config::GetS("mesh-assets") + name + ".obj";

    if (!File::Read(path, data)) {
        state = RES_INVALID;

        return;
    }

	// Do Parse Data Here

	std::stringstream stream(data);

	std::string line = "";

	std::vector<float> vert;
    std::vector<float> norm;
    std::vector<float> uvc;

	while (getline(stream, line)) {
		std::string type = "";

		for (std::string word : File::Split(line, ' ')) {
			if (type == "") type = word;
			else {
				if (type == "f") {
					std::vector<std::string> vals = File::Split(word, '/');

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

	state = RES_LOADED;
}

void Mesh::Generate() {
	state = RES_LOADING;

	size = vertices.size();
	
	state = RES_LOADED;
}

void Mesh::Upload() {
	state = RES_UPLOADING;

	// Upload Data To GPU

	glBindVertexArray(vao);

	if (!GenBuffer(vertBuf, 0, 3, &vertices) || !GenBuffer(normBuf, 1, 3, &normals) || !GenBuffer(uvcBuf, 2, 2, &uvcoords)) {
		state = RES_INVALID;

		return;
	}

	if (!hidden) Debug::Info("RESOURCE", "loaded mesh \"" + name + "\"");

	state = RES_READY;
}

void Mesh::Update() {
	state = RES_UPLOADING;

	// Upload Data To GPU

	glBindVertexArray(vao);

	if (!GenBuffer(vertBuf, 0, 3, &vertices) || !GenBuffer(normBuf, 1, 3, &normals) || !GenBuffer(uvcBuf, 2, 2, &uvcoords)) {
		state = RES_INVALID;

		return;
	}

	state = RES_READY;
}

void Mesh::Unload() {
	state = RES_UNLOADING;

	// Do Unload Data Here

	glDeleteVertexArrays(1, &vao);

	glDeleteBuffers(1, &vertBuf);
	glDeleteBuffers(1, &normBuf);
	glDeleteBuffers(1, &uvcBuf);

	state = RES_INVALID;
}

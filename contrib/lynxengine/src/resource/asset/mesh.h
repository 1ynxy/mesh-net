#ifndef mesh_h
#define mesh_h

#include <string>
#include <sstream>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GLFW/glfw3.h>

#include <../src/resource/asset.h>

class Mesh : public Asset {
private:
	// Member Variables

	std::vector<float> vertices;
	std::vector<float> normals;
	std::vector<float> uvcoords;

	GLuint vertBuf = 0;
	GLuint normBuf = 0;
	GLuint uvcBuf = 0;

public:
	// Member Variables

	unsigned int size = 0;

	GLuint vao = 0;

	// Constructors

	Mesh(const std::string& name);

	~Mesh();

	// Member Functions

	void init() override;
	void load() override;
	void upload() override;
	void unload() override;

	bool gen_buffer(GLuint& buffer, unsigned int position, unsigned int count, std::vector<float>* bufferData);
};

#endif

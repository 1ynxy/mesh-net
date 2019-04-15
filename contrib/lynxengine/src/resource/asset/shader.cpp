#include "shader.h"

#include <../src/core/core.h>
#include <../src/core/debug.h>
#include <../src/core/file.h>
#include <../src/core/config.h>

// Constructors & Destructors

Shader::Shader(const std::string& name) {
	this->name = name;
	
	init();
}

Shader::~Shader() {
	unload();
}

// Member Functions

void Shader::init() {
	state = ASSET_WAITING;

	// Init Asset Data

	program = glCreateProgram();

	if (!program) {
		debug.error("failed to create program for shader \"" + name + "\"");

		state = ASSET_INVALID;

		return;
	}

	state = ASSET_INIT;
}

void Shader::load() {
	state = ASSET_WAITING;
	
   // Load Asset Data

	std::string path = core.conf.get_string("resource-loc") + name + ".shader";

	if (!file.read(path, data)) {
		debug.error("failed to read data from \"" + path + "\"");

	   	state = ASSET_INVALID;
		
		return;
   	}

	state = ASSET_LOADED;
}

void Shader::upload() {
	state = ASSET_WAITING;

	// Upload Asset To GPU

	if (!load_pass(data, GL_VERTEX_SHADER) || !load_pass(data, GL_FRAGMENT_SHADER)) {
		debug.error("failed to load one or both passes for \"" + name + "\"");
		
		state = ASSET_INVALID;

		return;
	}

	glLinkProgram(program);

	state = ASSET_READY;

	debug.info("loaded shader \"" + name + "\"");
}

void Shader::unload() {
	state = ASSET_WAITING;

	// Unload Asset Data

	glDeleteProgram(program);

	state = ASSET_INVALID;
}

bool Shader::load_pass(std::string data, GLenum type) {
	std::string define = type == GL_VERTEX_SHADER ? "COMP_VERT" : "COMP_FRAG";
	data = "#version 450 core\n\n#define " + define + "\n\n" + data;

	// Compile Shader Pass

	GLuint pass = glCreateShader(type);

	if (pass == 0) {
		debug.error("failed to create shader pass " + define);

		return false;
	}

	const char* cData = data.c_str();
	glShaderSource(pass, 1, &cData, NULL);

	glCompileShader(pass);

	// Check Compiled

	GLint compiled = 1;
	glGetShaderiv(pass, GL_COMPILE_STATUS, &compiled);

	if (compiled == GL_FALSE) {
		GLint length = 0;
		glGetShaderiv(pass, GL_INFO_LOG_LENGTH, &length);
		GLchar error[length];
		glGetShaderInfoLog(pass, length, &length, &error[0]);

		glDeleteShader(pass);

		debug.error(error);

		return false;
	}
	
	glAttachShader(program, pass);

	glDeleteShader(pass);

	return true;
}

int Shader::get_uniform(const std::string& name) {
	if (!uniforms.count(name)) uniforms[name] = glGetUniformLocation(program, name.c_str());

	return uniforms[name];
}

void Shader::set(const std::string& name, float val) {
	if (ready()) glUniform1fv(get_uniform(name), 1, &val);
}

void Shader::set(const std::string& name, glm::vec2 val) {
	if (ready()) glUniform2fv(get_uniform(name), 1, glm::value_ptr(val));
}

void Shader::set(const std::string& name, glm::vec3 val) {
	if (ready()) glUniform3fv(get_uniform(name), 1, glm::value_ptr(val));
}

void Shader::set(const std::string& name, glm::vec4 val) {
	if (ready()) glUniform4fv(get_uniform(name), 1, glm::value_ptr(val));
}

void Shader::set(const std::string& name, glm::mat4 val) {
	if (ready()) glUniformMatrix4fv(get_uniform(name), 1, GL_FALSE, glm::value_ptr(val));
}

void Shader::set(const std::string& name, GLuint val) {
	if (ready()) glUniform1i(get_uniform(name), val);
}

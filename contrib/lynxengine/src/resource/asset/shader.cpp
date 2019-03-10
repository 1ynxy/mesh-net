#include "shader.h"

// Member Functions

bool Shader::LoadPass(std::string data, GLenum type) {
    std::string define = type == GL_VERTEX_SHADER ? "COMP_VERT" : "COMP_FRAG";
    data = "#version 450 core\n\n#define " + define + "\n\n" + data;

    // Compile Shader Pass

    GLuint pass = glCreateShader(type);

    if (pass == 0) {
        Debug::Error("RESOURCE", "failed to create shader " + define);

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

        Debug::Error("RESOURCE", error);

        return false;
    }
    
    glAttachShader(program, pass);

    glDeleteShader(pass);

    Debug::Log("RESOURCE", "compiled shader " + define + " for \"" + name + "\"");

    return true;
}

int Shader::GetUniform(const std::string& name) {
    if (!uniforms.count(name)) uniforms[name] = glGetUniformLocation(program, name.c_str());

    return uniforms[name];
}

void Shader::Set(const std::string& name, float val) {
    if (!Ready()) return;

    glUniform1fv(GetUniform(name), 1, &val);
}

void Shader::Set(const std::string& name, glm::vec2 val) {
    if (!Ready()) return;

    glUniform2fv(GetUniform(name), 1, glm::value_ptr(val));
}

void Shader::Set(const std::string& name, glm::vec3 val) {
    if (!Ready()) return;

    glUniform3fv(GetUniform(name), 1, glm::value_ptr(val));
}

void Shader::Set(const std::string& name, glm::vec4 val) {
    if (!Ready()) return;

    glUniform4fv(GetUniform(name), 1, glm::value_ptr(val));
}

void Shader::Set(const std::string& name, glm::mat4 val) {
    if (!Ready()) return;

    glUniformMatrix4fv(GetUniform(name), 1, GL_FALSE, glm::value_ptr(val));
}

void Shader::Set(const std::string& name, GLuint val) {
    if (!Ready()) return;

    glUniform1i(GetUniform(name), val);
}

// Gameloop Functions

void Shader::Init() {
    state = RES_INITIALISING;

    program = glCreateProgram();

    if (!program) {
        Debug::Error("RESOURCE", "failed to create program for shader \"" + name + "\"");

        state = RES_INVALID;

        return;
    }

    state = RES_INITIALISED;
}

void Shader::Load() {
    state = RES_LOADING;
    
    // Do Load Data Here

    std::string path = Config::GetS("shader-assets") + name + ".shader";

    if (!File::Read(path, data)) {
        state = RES_INVALID;

        return;
    }

    state = RES_LOADED;
}

void Shader::Upload() {
    state = RES_UPLOADING;

    // Upload Data To GPU

    if (!LoadPass(data, GL_VERTEX_SHADER) || !LoadPass(data, GL_FRAGMENT_SHADER)) {
        state = RES_INVALID;

        return;
    }

    glLinkProgram(program);

    Debug::Info("RESOURCE", "loaded shader \"" + name + "\"");

    state = RES_READY;
}

void Shader::Unload() {
    state = RES_UNLOADING;

    // Do Unload Data Here

    glDeleteProgram(program);

    state = RES_INVALID;
}

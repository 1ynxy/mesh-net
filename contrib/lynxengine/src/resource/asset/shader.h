#ifndef shader_h
#define shader_h

#include <string>
#include <map>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GLFW/glfw3.h>

#include <../src/resource/asset.h>

class Shader : public Asset {
private:
    // Member Variables

    std::map<std::string, int> uniforms = std::map<std::string, int>();
    
public:
    // Member Variables

    GLuint program = 0;

    // Constructors & Destructors

    Shader(const std::string& name);

	~Shader();

    // Member Functions

	void init() override;
    void load() override;
    void upload() override;
    void unload() override;

    bool load_pass(std::string data, GLenum type);

    int get_uniform(const std::string& name);

    void set(const std::string& name, float val);
	void set(const std::string& name, glm::vec2 val);
	void set(const std::string& name, glm::vec3 val);
	void set(const std::string& name, glm::vec4 val);
	void set(const std::string& name, glm::mat4 val);
	void set(const std::string& name, GLuint val);
};

#endif

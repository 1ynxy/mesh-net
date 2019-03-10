#ifndef shader_h
#define shader_h

#include <shared.h>

#include <resource/asset.h>

class Shader : public Asset {
private:
    // Member Variables

    std::map<std::string, int> uniforms = std::map<std::string, int>();
    
public:
    // Member Variables

    GLuint program = 0;

    // Constructors & Destructors

    Shader() {}

    // Member Functions

    bool LoadPass(std::string data, GLenum type);

    int GetUniform(const std::string& name);

    void Set(const std::string& name, float val);
	void Set(const std::string& name, glm::vec2 val);
	void Set(const std::string& name, glm::vec3 val);
	void Set(const std::string& name, glm::vec4 val);
	void Set(const std::string& name, glm::mat4 val);
	void Set(const std::string& name, GLuint val);

    // Gameloop Functions

    void Init() override;
    void Load() override;
    void Upload() override;
    void Unload() override;
};

#endif
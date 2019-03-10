#ifndef sprite_h
#define sprite_h

#include <shared.h>

#include <graphics/colour.h>

#include <resource/asset.h>

class Sprite : public Asset {
private:
    // Member Variables

    std::vector<unsigned char> image;

public:
    // Member Variables

    GLenum format = GL_RGBA;

    glm::vec2 size;

    GLuint pos = 0;

    // Constructors & Destructors

    Sprite() {}
    Sprite(glm::vec2 size, GLenum format = GL_RGBA) : size(size), format(format) {}

    // Member Functions

    void Clear(Colour colour);
    void Add(glm::vec2 size, const std::vector<float>& src);
    void Resize(glm::vec2 size);

    // Gameloop Functions

    void Init() override;
    void Load() override;
    void Generate() override;
    void Upload() override;
    void Update() override;
    void Unload() override;
};

#endif
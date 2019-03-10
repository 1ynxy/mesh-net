#ifndef mesh_h
#define mesh_h

#include <shared.h>

#include <resource/asset.h>

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

    Mesh() {}
    Mesh(const std::vector<float>& vertices, const std::vector<float>& normals, const std::vector<float>& uvcoords) : vertices(vertices), normals(normals), uvcoords(uvcoords) {}
        
    // Member Functions

    bool GenBuffer(GLuint& buffer, unsigned int position, unsigned int count, std::vector<float>* bufferData);

    void Clear();
    void Add(std::vector<float> vertices, std::vector<float> normals, std::vector<float> uvcoords);

    // Gameloop Functions

    void Init() override;
    void Load() override;
    void Generate() override;
    void Upload() override;
    void Update() override;
    void Unload() override;
};

#endif
#pragma once

#include <Engine/AssetManager/Asset.hpp>
#include <Engine/AssetManager/Model/BVH.hpp>

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <vector>
namespace uge{
    struct Model : public Asset{
    private:
        struct VertexData{
            glm::vec3 position;
            glm::vec3 normal;
            glm::vec2 uv;
        };

        std::vector<VertexData> data;

        GLuint VAO, VBO, EBO;
    public:
        std::vector<glm::vec3> vertices;
        std::vector<glm::vec3> normals;
        std::vector<glm::vec2> uv;

        std::vector<unsigned int> indices;

        BVH bvh;

        Model();
        ~Model();

        void Recalculate();
        void RecalculateNormals();
        void Apply();
        void Sync();

        const GLuint getVAO() {return VAO;}
    };
}
#include "assimp/mesh.h"
#include <Engine/AssetManager/Model/ModelLoader.hpp>
#include <Engine/AssetManager/Model/Model.hpp>
#include <Engine/AssetManager/Asset.hpp>

#include <glm/glm.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <memory>
#include <string>
#include <queue>
#include <filesystem>
#include <stdexcept>

namespace uge{
    std::shared_ptr<Asset> ModelLoader::Load(const std::filesystem::path &filePath){
        Assimp::Importer importer;

        importer.SetPropertyBool(AI_CONFIG_PP_PTV_NORMALIZE, true);

        const aiScene* scene = importer.ReadFile(filePath, 
            aiProcess_Triangulate | aiProcess_GenUVCoords |
             aiProcess_PreTransformVertices | aiProcess_FixInfacingNormals | 
            aiProcess_FlipWindingOrder);
        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) throw std::runtime_error("falha ao carregar o modelo: " + std::string(importer.GetErrorString()));
        
        std::shared_ptr<Model> model = std::make_shared<Model>();

        aiNode* node = scene->mRootNode;
        for (unsigned int i = 0; i < node->mNumMeshes; i++) {
            int baseIdx = model->vertices.size();
            aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
            for (unsigned int j = 0; j < mesh->mNumVertices; j++) {
                model->vertices.push_back(glm::vec3(
                    mesh->mVertices[j].x,
                    mesh->mVertices[j].y,
                    mesh->mVertices[j].z
                ));

                if (mesh->mTextureCoords[0]) {
                    model->uv.push_back(glm::vec2(
                        mesh->mTextureCoords[0][j].x,
                        mesh->mTextureCoords[0][j].y
                    ));
                } else {
                    model->uv.push_back(glm::vec2(0.0f));
                }
            }

            for (unsigned int j = 0; j < mesh->mNumFaces; j++){
                aiFace face = mesh->mFaces[j];
                model->indices.push_back(face.mIndices[0] + baseIdx);
                model->indices.push_back(face.mIndices[1] + baseIdx);
                model->indices.push_back(face.mIndices[2] + baseIdx);
            }
        }
        

        model->Recalculate();
        model->Apply();

        return model;
    }
}
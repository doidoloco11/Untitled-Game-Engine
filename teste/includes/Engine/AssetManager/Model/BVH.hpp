#pragma once

#include <glm/glm.hpp>

#include <vector>

namespace uge{
    struct BVH{
    public:
        struct Node{
        public:
            alignas(16) glm::vec3 min = glm::vec3(FLT_MAX);
            alignas(4) unsigned int trianglesindexCount = 0;
            alignas(16) glm::vec3 max = glm::vec3(FLT_MIN);
            alignas(4) unsigned int childrenIndex;

            bool isLeafNode() {return trianglesindexCount > 0;}
        };

        void build(const std::vector<glm::vec3>&, std::vector<unsigned int>&);

        std::vector<Node> nodes = {};
    };
}
#include <Engine/AssetManager/Model/Model.hpp>
#include <Engine/AssetManager/Model/BVH.hpp>

#include <glm/glm.hpp>
#include <glad/glad.h>

#include <cstddef>
#include <iterator>
#include <queue>
#include <utility>
#include <vector>
#include <chrono>
#include <algorithm>
#include <iostream>

namespace uge{
    //BVH Implementation

    struct Triangle {
        unsigned int idx0;
        unsigned int idx1;
        unsigned int idx2; 
        glm::vec3 centroid;
    };

    inline float getSurfaceMean(glm::vec3 size){
        return (size.x * size.y + size.z * size.x + size.z * size.y) / 3;
    }

    struct Bin {
        unsigned int triangleCount = 0;
        glm::vec3 min = glm::vec3(FLT_MAX);
        glm::vec3 max = glm::vec3(FLT_MIN);
    };

    float getBestSplitPointInAxis(int axis, unsigned int first_tri, unsigned int tri_count, const BVH::Node& node, const std::vector<Triangle>& triangles, float& minCost){
        minCost = FLT_MAX;
        const int binCount = 5;
        Bin bins[binCount];

        float boundsMin = node.min[axis];
        float boundsMax = node.max[axis];
        float scale = binCount / (boundsMax - boundsMin);

        for (unsigned int i = first_tri; i < first_tri + tri_count; i++) {
            int binIdx = static_cast<int>((triangles[i].centroid[axis] - boundsMin) * scale);
            if (binIdx < 0) binIdx = 0;
            if (binIdx >= binCount) binIdx = binCount - 1;

            bins[binIdx].triangleCount++;
            
            bins[binIdx].min = glm::min(bins[binIdx].min, triangles[i].centroid);
            bins[binIdx].max = glm::max(bins[binIdx].max, triangles[i].centroid);
        }

        int bestSplitBin = 0;

        for (int i = 0; i < binCount - 1; i++) {
            glm::vec3 leftMin{std::numeric_limits<float>::max()}, leftMax{-std::numeric_limits<float>::max()};
            glm::vec3 rightMin{std::numeric_limits<float>::max()}, rightMax{-std::numeric_limits<float>::max()};
            unsigned int leftCount = 0;
            unsigned int rightCount = 0;

            for (int j = 0; j <= i; j++) {
                if (bins[j].triangleCount == 0) continue;
                leftCount += bins[j].triangleCount;
                leftMin = glm::min(leftMin, bins[j].min);
                leftMax = glm::max(leftMax, bins[j].max);
            }

            for (int j = i + 1; j < binCount; j++) {
                if (bins[j].triangleCount == 0) continue;
                rightCount += bins[j].triangleCount;
                rightMin = glm::min(rightMin, bins[j].min);
                rightMax = glm::max(rightMax, bins[j].max);
            }

            if (leftCount == 0 || rightCount == 0) continue;

    
            float cost = getSurfaceMean(leftMax - leftMin) * leftCount +
                        getSurfaceMean(rightMax - rightMin) * rightCount;

            if (cost < minCost) {
                minCost = cost;
                bestSplitBin = i;
            }
        }

        return boundsMin + (bestSplitBin + 1) * (1.0f / scale);
    }

    float getBestSplitPoint(unsigned int first_tri, unsigned int tri_count, const BVH::Node& node, const std::vector<Triangle>& triangles, int& axis){
        float costA, costB, costC;
        
        float splitA = getBestSplitPointInAxis(0, first_tri, tri_count, node, triangles, costA);
        float splitB = getBestSplitPointInAxis(1, first_tri, tri_count, node, triangles, costB);
        float splitC = getBestSplitPointInAxis(2, first_tri, tri_count, node, triangles, costC);

        if (costA < costB && costA < costC){
            axis = 0;
            return splitA;
        }else if (costB < costC){
            axis = 1;
            return splitB;
        }else {
            axis = 2;
            return splitC;
        }
    }

    

    void BVH::build(const std::vector<glm::vec3>& vertices, std::vector<unsigned int>& indices) {
        unsigned int totalTriangles = indices.size() / 3;
        
        std::vector<Triangle> triangles(totalTriangles);
        for (unsigned int i = 0; i < totalTriangles; i++) {
            unsigned int idx = i * 3;
            glm::vec3 v0 = vertices[indices[idx + 0]];
            glm::vec3 v1 = vertices[indices[idx + 1]];
            glm::vec3 v2 = vertices[indices[idx + 2]];
            
            triangles[i].idx0 = indices[idx + 0];
            triangles[i].idx1 = indices[idx + 1];
            triangles[i].idx2 = indices[idx + 2];
            triangles[i].centroid = (v0 + v1 + v2) / 3.f;
        }

        nodes.clear();
        nodes.reserve(totalTriangles * 2); 

        std::queue<std::pair<unsigned int, unsigned int>> nodeBuildInfo;
        nodeBuildInfo.push({0, totalTriangles});

        unsigned int nodesCounter = 1;

        while (!nodeBuildInfo.empty()) {
            unsigned int first_tri = nodeBuildInfo.front().first;
            unsigned int tri_count = nodeBuildInfo.front().second;
            nodeBuildInfo.pop();

            nodes.push_back({}); 
            Node& node = nodes.back();

            for (int i = first_tri; i < first_tri + tri_count - 1; i++) {
                glm::vec3 v0 = vertices[triangles[i].idx0];
                glm::vec3 v1 = vertices[triangles[i].idx1];
                glm::vec3 v2 = vertices[triangles[i].idx2];

                node.min = glm::min(node.min, glm::min(v0, glm::min(v1, v2)));
                node.max = glm::max(node.max, glm::max(v0, glm::max(v1, v2)));
            }
            if (tri_count > 3){
                int axis;
                

                float splitPoint = getBestSplitPoint(first_tri, tri_count, node, triangles, axis);

                auto startIt = triangles.begin() + first_tri;
                auto endIt = startIt + tri_count;

                auto pivotIt = std::partition(startIt, endIt, [axis, splitPoint](const Triangle& triangle) {
                    return triangle.centroid[axis] < splitPoint;
                });

                int leftCount = std::distance(startIt, pivotIt);
                if (leftCount < tri_count && leftCount > 0){
                    node.childrenIndex = nodesCounter;
                    nodesCounter += 2;
                    nodeBuildInfo.push(std::make_pair(first_tri, leftCount));
                    nodeBuildInfo.push(std::make_pair(first_tri + leftCount, tri_count - leftCount));
                    continue;
                }
            }
            node.trianglesindexCount = tri_count * 3;
            node.childrenIndex = first_tri * 3;
        }
        for (int i = 0; i < triangles.size(); i++) {
            indices[i * 3 + 0] = triangles[i].idx0;
            indices[i * 3 + 1] = triangles[i].idx1;
            indices[i * 3 + 2] = triangles[i].idx2;
        }

    }

    //Model Implementation

    Model::Model(){
        glCreateVertexArrays(1, &VAO);
        glCreateBuffers(1, &VBO);
        glCreateBuffers(1, &EBO);

        glBindVertexArray(VAO);
        glVertexArrayVertexBuffer(VAO, 0, VBO, 0, sizeof(VertexData));
        glVertexArrayElementBuffer(VAO, EBO);

        glEnableVertexArrayAttrib(VAO, 0); //in_position
        glEnableVertexArrayAttrib(VAO, 1); //in_normal
        glEnableVertexArrayAttrib(VAO, 2); //in_uv

        glVertexArrayAttribFormat(VAO, 0, 3, GL_FLOAT, GL_FALSE, offsetof(VertexData, position));
        glVertexArrayAttribFormat(VAO, 1, 3, GL_FLOAT, GL_FALSE, offsetof(VertexData, normal));
        glVertexArrayAttribFormat(VAO, 2, 2, GL_FLOAT, GL_FALSE, offsetof(VertexData, uv));

        glVertexArrayAttribBinding(VAO, 0, 0);
        glVertexArrayAttribBinding(VAO, 1, 0);
        glVertexArrayAttribBinding(VAO, 2, 0);
    }

    Model::~Model(){
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
    }

    void Model::Recalculate(){
        auto tempoInicio = std::chrono::steady_clock::now();
        bvh.build(vertices, indices);
        auto tempoFinal = std::chrono::steady_clock::now();

        auto duracaoUs = std::chrono::duration_cast<std::chrono::microseconds>(tempoFinal - tempoInicio).count();

        std::cout << "[PERF] BVH gerada para o modelo "  << duracaoUs << " \u03bcs" << std::endl;

        if (normals.size() != vertices.size()) RecalculateNormals();
        if (uv.size() != vertices.size()) uv.assign(vertices.size(), glm::vec2(0));
    }

    void Model::RecalculateNormals(){
        normals.assign(vertices.size(), glm::vec3(0));
        for (int i = 0; i < indices.size(); i+=3) {
            glm::vec3 v0 = vertices[indices[i + 0]];
            glm::vec3 v1 = vertices[indices[i + 1]];
            glm::vec3 v2 = vertices[indices[i + 2]];

            glm::vec3 e0 = v1 - v0;
            glm::vec3 e1 = v2 - v0;

            glm::vec3 normal = glm::cross(e1, e0);
            normals[indices[i + 0]] += normal;
            normals[indices[i + 1]] += normal;
            normals[indices[i + 2]] += normal;
        }

        for (int i = 0; i < normals.size(); i++) {
            normals[i] = glm::normalize(normals[i]);
        }
    }

    void Model::Apply(){
        std::vector<VertexData> vertex(vertices.size());
        for (int i = 0; i < vertex.size(); i++) {
            vertex[i].position = vertices[i];
            vertex[i].normal = normals[i];
            vertex[i].uv = uv[i];
        }
        glNamedBufferData(VBO, vertex.size() * sizeof(VertexData), vertex.data(), GL_DYNAMIC_DRAW);
        glNamedBufferData(EBO, indices.size() * sizeof(unsigned int), indices.data(), GL_DYNAMIC_DRAW);
    }

    void Model::Sync(){
        std::vector<VertexData> vertex(vertices.size());
        glGetNamedBufferSubData(VBO, 0, vertex.size(), vertex.data());
        glGetNamedBufferSubData(EBO, 0, indices.size(), indices.data());
        
        for (int i = 0; i < vertex.size(); i++) {
            vertices[i] = vertex[i].position;
            normals[i] = vertex[i].normal;
            uv[i] = vertex[i].uv;
        }
    }
}
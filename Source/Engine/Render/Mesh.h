#pragma once

#include <span>

#include "Buffer.h"

class Renderer;

struct Vertex
{
    glm::vec3 mPosition{};
    float mPadding0{};
    glm::vec2 mUv{};
    glm::vec2 mPadding1{};
    glm::vec4 mColor{};
    glm::vec3 mNormal{};
    float mPadding2{};
};

struct Mesh
{
    Buffer mVertexBuffer;
    Buffer mIndexBuffer;

    void initializeBuffers(Renderer& renderer, std::span<Vertex> vertices, std::span<uint32_t> indices);

    void cleanupInitialized();
};

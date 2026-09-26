#pragma once

#include <span>

#include "Buffer.h"

class Renderer;

struct Vertex
{
    glm::vec3 mPosition;
    float padding;
    glm::vec2 mUv;
    glm::vec2 padding2;
};

struct Mesh
{
    Buffer mVertexBuffer;
    Buffer mIndexBuffer;

    void initializeBuffers(Renderer& renderer, std::span<Vertex> vertices, std::span<uint32_t> indices);

    void cleanupInitialized();
};

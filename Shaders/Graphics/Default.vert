#version 450

#extension GL_GOOGLE_include_directive : require
#extension GL_EXT_buffer_reference : require

#include "Data.glsl"

layout (location = 0) out vec2 oUv;
layout (location = 1) out vec3 oColor;
layout (location = 2) out vec3 oNormal;

struct Vertex
{
    vec3 position;
    float padding;
    vec2 uv;
    vec2 padding2;
    vec4 color;
    vec3 normal;
    float padding3;
};

layout (buffer_reference, std430) readonly buffer Vbo
{ 
	Vertex vertices[];
};

layout (push_constant) uniform constants
{
	mat4 transform;
	Vbo vbo;
} uPushConstants;

layout (set = 0, binding = 0, std140) uniform Mvp
{
    mat4 view, projection;
} uMvp;

void main() 
{
	Vertex vertex = uPushConstants.vbo.vertices[gl_VertexIndex];

	gl_Position = uMvp.projection * uMvp.view * uPushConstants.transform * vec4(vertex.position, 1);

	oUv = vertex.uv;
	oColor = vertex.color.xyz * uMaterialConstants.colorFactors.xyz;
    oNormal = (uPushConstants.transform * vec4(vertex.normal, 0)).xyz;
}

#version 450

#extension GL_EXT_buffer_reference : require

layout (location = 0) out vec2 oUv;

struct Vertex
{
	vec3 position;
	float padding;
	vec2 uv;
	vec2 padding2;
};

layout (buffer_reference, std430) readonly buffer Vbo { Vertex vertices[]; };

layout (push_constant) uniform constants
{
	mat4 viewProjection;
	Vbo vbo;
} uPushConstants;

layout (set = 0, binding = 0, std140) uniform Mvp
{
    mat4 model, view, projection;
} uMvp;

void main() 
{
	Vertex vertex = uPushConstants.vbo.vertices[gl_VertexIndex];

	gl_Position = uMvp.projection * uMvp.view * uMvp.model * vec4(vertex.position, 1.0);

	oUv = vertex.uv;
}

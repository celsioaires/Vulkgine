#version 450
#extension GL_EXT_buffer_reference : require

layout (location = 0) out vec3 outColor;

struct Vertex
{
	vec3 position;
	float padding;
};

layout (buffer_reference, std430) readonly buffer Vbo
{ 
	Vertex vertices[];
};

layout (push_constant) uniform constants
{
	mat4 viewProjection;
	Vbo vbo;
} pushConstants;

layout (set = 0, binding = 0, std140) uniform Mvp
{
    mat4 model;
    mat4 view;
    mat4 projection;
} mvp;

void main() 
{
	Vertex vertex = pushConstants.vbo.vertices[gl_VertexIndex];

	gl_Position = mvp.projection * mvp.view * vec4(vertex.position, 1.0);

	outColor = vec3(1.0, 0.0, 0.0);
}

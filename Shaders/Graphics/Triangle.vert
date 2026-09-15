#version 450
#extension GL_EXT_buffer_reference : require

layout (location = 0) out vec3 outColor;

struct Vertex
{
	vec3 position;
	float padding;
};

layout (buffer_reference, std430) readonly buffer VertexBuffer
{ 
	Vertex vertices[];
};

layout (push_constant) uniform constants
{
	mat4 viewProjection;
	VertexBuffer vertexBuffer;
} pushConstants;

void main() 
{
	Vertex vertex = pushConstants.vertexBuffer.vertices[gl_VertexIndex];

	gl_Position = pushConstants.viewProjection * vec4(vertex.position, 1.0);

	outColor = vec3(1.0, 0.0, 0.0);
}

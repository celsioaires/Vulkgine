#version 450

layout (location = 0) in vec3 inColor;

layout (location = 0) out vec4 outFragColor;

layout (set = 1, binding = 0) uniform sampler2D displayTexture;

void main() 
{
	outFragColor = texture(displayTexture, vec2(0.5, 0.5));
}

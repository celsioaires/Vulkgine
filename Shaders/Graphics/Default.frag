#version 450

layout (location = 0) out vec4 oFragment;

layout (location = 0) in vec2 iUv;

layout (set = 1, binding = 0) uniform sampler2D uColorTexture;

void main() 
{
	oFragment = texture(uColorTexture, iUv);
}

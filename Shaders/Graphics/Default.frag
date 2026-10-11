#version 450

layout (location = 0) out vec4 oFragment;

layout (location = 0) in vec2 iUv;
layout (location = 1) in vec4 iColor;

layout (set = 1, binding = 0) uniform MaterialConstants
{
	vec4 colorFactors, metalRoughFactors;
} uMaterialConstants;

layout (set = 1, binding = 1) uniform sampler2D uColorTexture;

void main() 
{
	oFragment = texture(uColorTexture, iUv);
}

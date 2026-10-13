layout (set = 1, binding = 0) uniform MaterialConstants
{
	vec4 colorFactors, metalRoughFactors;
} uMaterialConstants;

layout (set = 1, binding = 1) uniform sampler2D uColorTexture;

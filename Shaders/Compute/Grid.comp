#version 460

layout (local_size_x = 16, local_size_y = 16) in;

layout (rgba16f, set = 0, binding = 0) uniform image2D image;

layout (push_constant) uniform constants { vec4 data1, data2, data3, data4; } pushConstants;

void main()
{
    ivec2 texelCoord = ivec2(gl_GlobalInvocationID.xy);
    ivec2 size = imageSize(image);

    if (texelCoord.x >= size.x || texelCoord.y >= size.y)
        return;

    bool verticalLine = gl_LocalInvocationID.x == 0;
    bool horizontalLine = gl_LocalInvocationID.y == 0;

    vec4 color = vec4(0.0);

    if (verticalLine || horizontalLine)
        color = pushConstants.data1;

    imageStore(image, texelCoord, color);
}

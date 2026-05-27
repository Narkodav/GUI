#version 450

layout(location = 0) in vec2 inPosition;
layout(location = 1) in vec2 inUV;

layout(location = 2) in ivec2 quadPosition;   // pixels
layout(location = 3) in ivec2 quadSize;       // pixels
layout(location = 4) in uint quadTextureId;

layout(location = 0) out vec2 outUV;
layout(location = 1) flat out uint outTextureId;

layout(push_constant) uniform PushConstants {
    uint width;
    uint height;
} dimensions;

void main()
{
    // Local vertex position in pixel space
    vec2 pixelPos = quadPosition + inPosition * quadSize;

    // Convert to normalized [0, 1]
    vec2 normalized = pixelPos / vec2(dimensions.width, dimensions.height);

    // Convert to NDC [-1, 1]
    vec2 ndc = normalized * 2.0 - 1.0;

    // // Flip Y because screen coordinates usually have origin at top-left
    // ndc.y = -ndc.y;

    gl_Position = vec4(ndc, 0.0, 1.0);

    outUV = inUV;
    outTextureId = quadTextureId;
}
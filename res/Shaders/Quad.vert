#version 450
layout(location = 0) in vec2 inPosition;

layout(location = 1) in ivec2 quadPosition;     // pixels
layout(location = 2) in ivec2 quadSize;         // pixels
layout(location = 3) in vec2 uvMin;
layout(location = 4) in vec2 uvMax;
layout(location = 5) in uint quadTextureId;

layout(location = 6) in ivec2 offset;
layout(location = 7) in ivec2 bounds;

layout(location = 0) out vec2 outUV;
layout(location = 1) flat out uint outTextureId;
layout(location = 2) flat out ivec2 outOffset;
layout(location = 3) flat out ivec2 outBounds;

layout(push_constant) uniform PushConstants {
    uint width;
    uint height;
} dimensions;

vec2 getUv() {
    // vec2 uv;
    // switch (gl_VertexIndex) {
    //     case 0: uv = uvMin.xy; break;
    //     case 1: uv = vec2(uvMax.x, uvMin.y); break;
    //     case 2: uv = uvMax.xy; break;
    //     case 3: uv = vec2(uvMin.x, uvMax.y); break;
    // }
    return uvMin + inPosition * (uvMax - uvMin);
}

void main()
{
    // Local vertex position in pixel space
    vec2 pixelPos = quadPosition + offset + inPosition * quadSize;

    // Convert to normalized [0, 1]
    vec2 normalized = pixelPos / vec2(dimensions.width, dimensions.height);

    // Convert to NDC [-1, 1]
    vec2 ndc = normalized * 2.0 - 1.0;

    // // Flip Y because screen coordinates usually have origin at top-left
    // ndc.y = -ndc.y;

    gl_Position = vec4(ndc, 0.0, 1.0);

    outUV = getUv();
    outTextureId = quadTextureId;
    outOffset = offset;
    outBounds = bounds;
}
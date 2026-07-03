#version 450
#extension GL_EXT_nonuniform_qualifier : require

layout(location = 0) in vec2 inUV;
layout(location = 1) flat in uint inTextureId;
layout(location = 2) flat in ivec2 inOffset;
layout(location = 3) flat in ivec2 inBounds;

layout(location = 0) out vec4 outColor;

layout(set = 0, binding = 0) uniform sampler2D textures[];

layout(push_constant) uniform PushConstants {
    uint width;
    uint height;
} dimensions;

void main()
{
    // vec4 fragCoord = gl_FragCoord;
    // fragCoord.y = fragCoord.y - dimensions.height;
    if (gl_FragCoord.x < inOffset.x || gl_FragCoord.x >= inBounds.x + inOffset.x ||
        gl_FragCoord.y < inOffset.y || gl_FragCoord.y >= inBounds.y + inOffset.y) { 
            discard; 
    }

    outColor = texture(textures[inTextureId], inUV);
    if (outColor.a < 0.01) discard;
}
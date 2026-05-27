#version 450
//!#extension GL_KHR_vulkan_glsl : require
#extension GL_EXT_nonuniform_qualifier : require

layout(location = 0) in vec2 inUV;
layout(location = 1) flat in uint inTextureId;

layout(location = 0) out vec4 outColor;

layout(set = 0, binding = 0) uniform sampler2D textures[];

void main()
{
    outColor = texture(textures[inTextureId], inUV);

    if (outColor.a < 0.01)
        discard;
}
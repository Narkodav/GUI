#version 460
#extension GL_KHR_vulkan_glsl : enable

layout (location = 0) in vec2 aPos; //from -1 to 1
layout (location = 1) in vec2 aTexCoord;

layout(set = 3, binding = 0) uniform Config {
    float contrast;         // 0.0 = grayscale, 1.0 = normal, >1.0 = increased contrast
} config;

uniform mat4 uProjection;   // transforms from NDC to window coords
uniform float uZValue;      // from -1 to 1
uniform vec2 uPosition;     // in window pixels
uniform vec2 uSize;         // in window pixels
uniform vec2 uBearing;      // in window pixels
uniform float uScale;       // can be literally anything

out vec2 TexCoord;

void main() {          
    vec2 scaledSize = uSize * uScale;                    //scaled size in pixels
    vec2 pos = vec2(uPosition.x + uBearing.x * uScale, 
        uPosition.y - scaledSize.y + uBearing.y * uScale); //scaled position in pixels
    vec2 screenPos = (aPos + 1) / 2;
    //transforms pixel coords to NDC
    gl_Position = uProjection * vec4(screenPos * scaledSize + pos, uZValue, 1.0);
    TexCoord = aTexCoord;
}
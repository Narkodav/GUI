#version 330 core

out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D uTexture;
uniform vec3 uColor;
uniform bool uIsColor;

void main()
{
    if(!uIsColor)
    {
        vec4 sampled = vec4(1.0, 1.0, 1.0, texture(uTexture, TexCoord).r);
        FragColor = vec4(uColor, 1.0) * sampled;
    }
    else FragColor = texture(uTexture, TexCoord);
}
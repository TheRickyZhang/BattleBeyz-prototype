#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D backgroundTexture;
uniform float time;

void main()
{
    vec2 uv = TexCoords;
    uv.x += time * 0.01; // Example of moving the background
    uv.y += time * 0.01;
    FragColor = texture(backgroundTexture, uv) * 1.25;
}

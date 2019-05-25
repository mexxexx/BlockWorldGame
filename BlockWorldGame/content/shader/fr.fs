#version 330 core
out vec4 FragColor;

in vec2 oTexCoord;

uniform sampler2D terrainTexture;

void main()
{
    FragColor = texture(terrainTexture, oTexCoord);
} 
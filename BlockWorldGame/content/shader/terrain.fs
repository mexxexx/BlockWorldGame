#version 330 core
out vec4 FragColor;

in vec2 textureCoordinateOut;

uniform sampler2D terrainTexture;

void main()
{
    FragColor = texture(terrainTexture, textureCoordinateOut);
} 
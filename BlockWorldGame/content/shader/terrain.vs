#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec2 textureCoordinate;

out vec2 textureCoordinateOut;

uniform mat4 viewProjection;
uniform vec4 lightShades;

void main()
{
    gl_Position = viewProjection * vec4(position.x, position.y, position.z, 1.0);
    textureCoordinateOut = textureCoordinate;
}
#version 460 core

uniform vec3 Color;

out vec4 FinalColor;

void main()
{
    FinalColor = vec4(Color, 1.0f);
}
#version 460 core

// Vertex data interpretation
layout (location = 0) in vec4 Vertex;


// Inputs
uniform mat4 ProjectionMat;

// Outputs to Fragment Shader
out vec2 FragTexCoords;

void main()
{
	gl_Position =ProjectionMat * vec4(Vertex.xy, 0.0f, 1.0f);
	FragTexCoords = Vertex.zw;
}
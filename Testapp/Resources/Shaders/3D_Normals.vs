#version 460 core

// Vertex data interpretation
layout (location = 0) in vec3 Position;
layout (location = 1) in vec2 TexCoords;
layout (location = 2) in vec3 Normal;

// Inputs
uniform mat4 PVMMat;
uniform mat4 Model;


// Outputs to Fragment Shader
out vec2 FragTexCoords;
out vec3 FragNormal;
out vec3 FragPos;

void main()
{
	gl_Position = PVMMat * vec4(Position, 1.0f);
	

	FragTexCoords = TexCoords;
	FragNormal = mat3(transpose(inverse(Model))) * Normal;
	FragPos = vec3(Model * vec4(Position, 1.0f));
}
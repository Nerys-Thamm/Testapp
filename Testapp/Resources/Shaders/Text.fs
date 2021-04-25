#version 460 core

// Vertex Shader Inputs
in vec2 FragTexCoords;

// Uniform Inputs
uniform sampler2D TextTexture;
uniform vec3 TextColor;


// Output
out vec4 FinalColor;



void main()
{
	float Alpha = texture(TextTexture, FragTexCoords).r;
	FinalColor = vec4(TextColor, Alpha); 
}
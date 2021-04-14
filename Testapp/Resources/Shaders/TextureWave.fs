#version 460 core

// Vertex Shader Inputs
in vec3 FragColor;
in vec2 FragTexCoords;

// Uniform Inputs
uniform sampler2D ImageTexture;
uniform float CurrentTime;

// Output
out vec4 FinalColor;



void main()
{
	//Alter the coordinate based on time
	vec2 Distorted = vec2(FragTexCoords.x + (sin((CurrentTime+FragTexCoords.y)*8)/5), FragTexCoords.y);
	FinalColor = texture(ImageTexture, Distorted); 
}
#version 460 core

// Vertex Shader Inputs
in vec2 FragTexCoords;
in vec2 VertexOut;

// Uniform Inputs
uniform sampler2D TextTexture;
uniform vec3 TextColor;
uniform float ScreenWidth;
uniform float CurrentTime;


// Output
out vec4 FinalColor;



void main()
{
	
	float Alpha = texture(TextTexture, FragTexCoords).r;
if(mod(VertexOut.x + (CurrentTime*40), ScreenWidth-48)+48 < 0 || mod(VertexOut.x + (CurrentTime*40), ScreenWidth) > ScreenWidth)
{
	Alpha = 0;
}
	FinalColor = vec4(TextColor, Alpha); 
}
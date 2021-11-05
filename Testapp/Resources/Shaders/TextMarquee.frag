#version 460 core

// Vertex Shader Inputs
in vec2 FragTexCoords;
in vec2 VertexOut;

// Uniform Inputs
uniform sampler2D TextTexture;
uniform vec3 TextColor;
uniform float ScreenWidth;
uniform float LeftBuffer;
uniform float RightBuffer;
uniform float CurrentTime;


// Output
out vec4 FinalColor;



void main()
{
	
	float Alpha;
	if(VertexOut.x < LeftBuffer || VertexOut.x > RightBuffer)
	{
		Alpha = 0;
	}
	else
	{
		Alpha = texture(TextTexture, FragTexCoords).r;
	}
	FinalColor = vec4(TextColor, Alpha); 
}
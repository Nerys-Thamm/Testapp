#version 460 core

// Vertex data interpretation
layout (location = 0) in vec4 Vertex;


// Inputs
uniform mat4 ProjectionMat;
uniform float CurrentTime;
uniform float ScreenWidth;

// Outputs to Fragment Shader
out vec2 FragTexCoords;
out vec2 VertexOut;

void main()
{
	gl_Position = ProjectionMat * vec4(mod(Vertex.x + (CurrentTime*40), ScreenWidth+96)-48, Vertex.y + (sin((Vertex.x + CurrentTime*40)/40)*40), 0.0f, 1.0f);
	FragTexCoords = Vertex.zw;
	VertexOut = Vertex.xy;
}
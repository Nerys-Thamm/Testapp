#version 460 core 

// Vertex Shader Inputs
in vec3 FragColor;
in vec2 FragTexCoords;

// Uniform Inputs
uniform sampler2D ImageTexture;
uniform sampler2D ImageTexture1;
uniform float CurrentTime;

//Output
out vec4 FinalColor;

void main()
{
    FinalColor  = mix(texture(ImageTexture, FragTexCoords + vec2(sin(CurrentTime), cos(CurrentTime))), texture(ImageTexture1, FragTexCoords + vec2(cos(CurrentTime), sin(CurrentTime))), (sin(CurrentTime)+1)/2);
}
#version 460 core

// Vertex Shader Inputs
in vec3 FragColor;
in vec2 FragTexCoords;

// Uniform Inputs
uniform sampler2D ImageTexture;
uniform sampler2D ImageTexture1;
uniform float CurrentTime;
uniform bool IsFading;

//Output
out vec4 FinalColor;

float Currtime = 0;
float CurrFadePercent = 0;

void main()
{
    Currtime = CurrentTime - Currtime;
    if(IsFading){CurrFadePercent += Currtime;}
    else {CurrFadePercent -= Currtime;}
    FinalColor  = mix(texture(ImageTexture, FragTexCoords), texture(ImageTexture1, FragTexCoords), clamp(CurrFadePercent, 0, 1));
}
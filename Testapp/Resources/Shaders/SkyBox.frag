#version 460 core
// Vertex shader inputs
in vec3 FragTexCoords;

//Uniform inputs
uniform samplerCube Texture;

//Output
out vec4 FinalColor;

void main()
{
    FinalColor = texture(Texture, FragTexCoords);
}
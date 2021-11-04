#version 460 core
layout (location = 0) in vec3 Position;
layout (location = 1) in vec2 TexCoords;
layout (location = 2) in vec3 Normal;

out vec2 FragTexCoords;

void main()
{
    const vec3 Vertices[] = vec3[]( vec3(  0.0f, 0.8f, 0.0f),
                                    vec3( -0.5f, 0.0f, 0.0f),
                                    vec3(  0.5f, 0.0f, 0.0f));

    gl_Position = vec4(Vertices[gl_VertexID], 1.0f);
}
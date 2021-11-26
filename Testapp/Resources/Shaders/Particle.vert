#version 460 core
layout (location = 0) in vec3 vertex;
layout (location = 1) in vec4 color;
layout (location = 2) in float size;

out VS_OUT {
    vec4 vertColor;
    float Size;
    } vs_out;

void main(){
    gl_Position = vec4(vertex, 1.0f);
    vs_out.vertColor = color;
    vs_out.Size = size;
}
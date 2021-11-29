#version 460 core
out vec4 color;

in float lifetime;


void main(){
    color = vec4(0.7f, 0.7f, 0.7f, 1-lifetime);
}
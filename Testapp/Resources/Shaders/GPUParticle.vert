#version 460 core
layout (location = 0) in vec4 position;

uniform mat4 vp;
out float lifetime;

void main(){
    gl_Position = vp * vec4(position.xyz, 1.0f);
    lifetime = position.w;
}
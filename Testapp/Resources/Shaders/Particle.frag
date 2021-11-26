#version 460 core
out vec4 color;

in GS_FS_VERTEX{
    vec2 texcoord;
    vec4 outcolor;
}fs_in;

uniform sampler2D Texture;


void main(){
    color = texture(Texture, vec2( fs_in.texcoord.x , 1 - fs_in.texcoord.y)) * fs_in.outcolor;
}
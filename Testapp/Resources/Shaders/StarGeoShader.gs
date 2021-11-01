
#version 460 core
#define M_PI 3.1415926535897932384626433832795

layout (points) in;
layout (triangle_strip, max_vertices = 66) out;

out vec3 outColor;

in VS_GS_VERTEX{
    in vec4 position;
    in vec3 color;
    in mat4 mvp;
    }gs_in[];

void main() 
{
    

    gl_Position = gs_in[0].position + gs_in[0].mvp * vec4(0.0f, 0.0f, 0.0f, 0.0f); EmitVertex();

    for (int i = 0; i < 33; i++)
    {
        float theta = i*2.0f*M_PI / 10.0f + 0.279253f;

        float mag = mod(i, 2) == 0 ? 1.0f : 0.5f;

        vec2 pos = vec2(cos(theta) * mag, sin(theta) * mag);
        float thirdOfCircle = ((2.0f*M_PI)/3);
        outColor = vec3(((sin(theta) + 1.0f) * 0.5f), ((sin(theta + 2.0f) + 0.5f) * 0.5f), ((sin(theta + 4.0f) + 1.0f) * 0.5f));
        
        if(mod(i,3) == 0)
        {
            gl_Position = gs_in[0].position + gs_in[0].mvp * vec4(0.0f, 0.0f, 0.5f, 0.0f); EmitVertex();
            EndPrimitive();
        }
        else
        {
            gl_Position = gs_in[0].position + gs_in[0].mvp * vec4(pos.x, pos.y, 0.0f, 0.0f); EmitVertex();
        }
    }

     gl_Position = gs_in[0].position + gs_in[0].mvp * vec4(0.0f, 0.0f, 0.0f, 0.0f); EmitVertex();

    for (int i = 33; i > 0; i--)
    {
        float theta = i*2.0f*M_PI / 10.0f + 0.279253f;

        float mag = mod(i, 2) == 0 ? 1.0f : 0.5f;

        vec2 pos = vec2(cos(theta) * mag, sin(theta) * mag);
        float thirdOfCircle = ((2.0f*M_PI)/3);
        outColor = vec3(((sin(theta) + 1.0f) * 0.5f), ((sin(theta + 2.0f) + 0.5f) * 0.5f), ((sin(theta + 4.0f) + 1.0f) * 0.5f));
        
        if(mod(i,3) == 0)
        {
            gl_Position = gs_in[0].position + gs_in[0].mvp * vec4(0.0f, 0.0f, -0.5f, 0.0f); EmitVertex();
            EndPrimitive();
        }
        else
        {
            gl_Position = gs_in[0].position + gs_in[0].mvp * vec4(pos.x, pos.y, 0.0f, 0.0f); EmitVertex();
        }
    }
    
    
}
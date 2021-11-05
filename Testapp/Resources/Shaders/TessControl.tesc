
#version 460 core

//size of output patch & no. of times the TCS will be executed
layout (vertices = 4) out;

uniform int InnerLOD;
uniform int OuterLOD;

void main() 
{
	if (gl_InvocationID == 0)
	{
		gl_TessLevelInner[0] = InnerLOD;
		gl_TessLevelInner[1] = InnerLOD;
		gl_TessLevelOuter[0] = OuterLOD;
		gl_TessLevelOuter[1] = OuterLOD;
		gl_TessLevelOuter[2] = OuterLOD;
		gl_TessLevelOuter[3] = OuterLOD;
	}
	gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;
}
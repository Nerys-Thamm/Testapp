#version 460 core


//Vertex shader inputs
in vec2 FragTexCoords;
in vec3 FragNormal;
in vec3 FragPos;

//Uniform inputs
uniform sampler2D ImageTexture;
uniform float CurrentTime;

//Output
out vec4 FinalColor;

void main()
{
   
   // distance from center of image, used to adjust blur
   	vec2 uv = FragTexCoords.xy;
   	float d = length((uv) - vec2(0.5,0.5));
   	
   	// blur amount
   	float blur = 0.0;	
   	blur = (1.0 + sin(CurrentTime*6.0)) * 0.5;
   	blur *= 1.0 + sin(CurrentTime*16.0) * 0.5;
   	blur = pow(blur, 3.0);
   	blur *= 0.05;
   	// reduce blur towards center
   	blur *= d;
   	
   	// final color
       vec3 col;
       col.r = texture( ImageTexture, vec2(uv.x+blur,uv.y) ).r;
       col.g = texture( ImageTexture, uv ).g;
       col.b = texture( ImageTexture, vec2(uv.x-blur,uv.y) ).b;
   	
   	// scanline
   	float scanline = sin(uv.y*1600.0)*0.04;
   	col -= scanline;
   	
   	// vignette
   	col -= pow((d), 3.0);
   	
       
    //Calculate final colour
	FinalColor = vec4(col,1.0);
}
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
   
    float PI = 6.28318530718; //2*PI because thats a circle
    float Directions = 32.0; //Number of directions to sample
    float Quality = 10.0; // Times to sample
    float Size = 0.8; // Radius of circle to sample from surrounding pixels
    vec2 Radius = vec2(Size, Size);

   
   	vec2 uv = FragTexCoords.xy;
   	float d = length((uv) - vec2(0.5,0.5));
   	
   	// Chromatic abberation
   	float Chroma = 0.0;	
   	Chroma = (1.0 + sin(CurrentTime*6.0)) * 0.5;
   	Chroma *= 1.0 + sin(CurrentTime*16.0) * 0.5;
   	Chroma = pow(Chroma, 3.0);
   	Chroma *= 0.01;
   	Chroma *= d;
   	
   	// final color
    vec3 col;
       
   	
    for(float i=0.0; i<PI; i+=PI/Directions)
    {
        for(float j=1.0/Quality; j<=1.0; j+=1.0/Quality)
        {
			
            col.r += texture( ImageTexture, (uv+vec2(cos(i),sin(i))*(Radius* pow((d), 3.0))*j)+vec2(Chroma,0.0)).r;
            col.g += texture( ImageTexture, (uv+vec2(cos(i),sin(i))*(Radius* pow((d), 3.0))*j)+vec2(0.0,Chroma)).g;
            col.b += texture( ImageTexture, (uv+vec2(cos(i),sin(i))*(Radius* pow((d), 3.0))*j)-vec2(Chroma,Chroma)).b;
        }
    }

    col /= Quality * Directions - 15.0;
   	// scanline
    float scanline = sin(uv.y*1600.0)*0.04;
    col -= scanline;
   	
 
    //Calculate final colour
    FinalColor = vec4(col,1.0);
}
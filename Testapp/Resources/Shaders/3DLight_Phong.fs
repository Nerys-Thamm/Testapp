#version 460 core


//Vertex shader inputs
in vec2 FragTexCoords;
in vec3 FragNormal;
in vec3 FragPos;

//Uniform inputs
uniform sampler2D ImageTexture;
uniform vec3 CameraPos;
uniform float AmbientStrength;
uniform vec3 AmbientColor;
uniform vec3 LightColor               = vec3(1.0f, 1.0f, 1.0f);
uniform vec3 LightPos                 = vec3(0.0f, 6.0f, 6.0f);
uniform float LightSpecularStrength   = 1.0f;
uniform float Shininess;

//Output
out vec4 FinalColor;

void main()
{
    //Light direction
    vec3 Normal = normalize(FragNormal);
    vec3 LightDir = normalize(FragPos - LightPos);

    //Ambient component
    vec3 Ambient = AmbientStrength * AmbientColor;

    //Diffuse component
    float DiffuseStrength = max(dot(Normal, -LightDir), 0.0f);
    vec3 Diffuse = DiffuseStrength * LightColor;

    //Specular component
    vec3 ReverseViewDir = normalize(CameraPos - FragPos);
    vec3 ReflectedDir = reflect(LightDir, Normal);
    float SpecularReflectivity = pow(max(dot(ReverseViewDir, ReflectedDir), 0.0f), Shininess);
    vec3 Specular = LightSpecularStrength * SpecularReflectivity * LightColor;

    //Combine the lighting components
    vec4 Light = vec4(Ambient + Diffuse + Specular, 1.0f);

    //Calculate final colour
	FinalColor = Light * texture(ImageTexture, FragTexCoords);
}
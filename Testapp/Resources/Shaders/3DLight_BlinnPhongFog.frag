#version 460 core
#define MAX_POINT_LIGHTS 4
#define MAX_DIRECTIONAL_LIGHTS 1

//Vertex shader inputs
in vec2 FragTexCoords;
in vec3 FragNormal;
in vec3 FragPos;
in vec4 mWorldPos;

struct PointLight
{
    vec3 Position;
    vec3 Color;
    float AmbientStrength;
    float SpecularStrength;

    float AttenuationConstant;
    float AttenuationLinear;
    float AttenuationExponent;
};

struct DirectionalLight
{
	vec3 Direction;
	vec3 Color;
	float AmbientStrength;
	float SpecularStrength;
};

struct Material
{
    float Smoothness;
    float Reflectivity;
};

//Uniform inputs
uniform sampler2D ImageTexture;
uniform vec3 CameraPos;
uniform Material Mat[1];
uniform float AmbientStrength;
uniform vec3 AmbientColor;
uniform vec3 LightColor               = vec3(1.0f, 1.0f, 1.0f);
uniform vec3 LightPos                 = vec3(0.0f, 6.0f, 6.0f);
uniform float LightSpecularStrength   = 1.0f;
uniform float Shininess;
uniform PointLight PointLights[MAX_POINT_LIGHTS];
uniform DirectionalLight DirectionalLights[MAX_DIRECTIONAL_LIGHTS];

//Output
out vec4 FinalColor;



vec3 CalculateLight_Point(PointLight _light)
{
    //Light direction
    vec3 Normal = normalize(FragNormal);
    vec3 LightDir = normalize(FragPos - _light.Position);

    //Ambient component
    vec3 Ambient = _light.AmbientStrength * _light.Color;

    //Diffuse component
    float DiffuseStrength = max(dot(Normal, -LightDir), 0.0f);
    vec3 Diffuse = DiffuseStrength * _light.Color;

    //Specular component
    vec3 ReverseViewDir = normalize(CameraPos - FragPos);
    vec3 HalfwayVector = normalize(-LightDir + ReverseViewDir);
    float SpecularReflectivity = pow(max(dot(Normal, HalfwayVector), 0.0f), Mat[0].Smoothness);
    vec3 Specular = _light.SpecularStrength * SpecularReflectivity * _light.Color;

    //Combine the lighting components
    vec3 CombinedLight = vec3(Ambient + Diffuse + Specular);

    float Distance = length(_light.Position - FragPos);
    float Attenuation = _light.AttenuationConstant + (_light.AttenuationLinear * Distance) + (_light.AttenuationExponent * pow(Distance, 2));
    if(Attenuation < 1f)
    {
        return vec3(0.0f,0.0f,0.0f);
    }
    return CombinedLight / Attenuation;
}

vec3 CalculateLight_Directional(DirectionalLight _light)
{
    //Light direction
    vec3 Normal = normalize(FragNormal);
    vec3 LightDir = _light.Direction;

    //Ambient component
    vec3 Ambient = _light.AmbientStrength * _light.Color;

    //Diffuse component
    float DiffuseStrength = max(dot(Normal, -LightDir), 0.0f);
    vec3 Diffuse = DiffuseStrength * _light.Color;

    //Specular component
    vec3 ReverseViewDir = normalize(CameraPos - FragPos);
    vec3 HalfwayVector = normalize(-LightDir + ReverseViewDir);
    float SpecularReflectivity = pow(max(dot(Normal, HalfwayVector), 0.0f), Mat[0].Smoothness);
    vec3 Specular = _light.SpecularStrength * SpecularReflectivity * _light.Color;

    //Combine the lighting components
    vec3 CombinedLight = vec3(Ambient + Diffuse + Specular);

    
    return CombinedLight;
}



void main()
{
    vec3 LightOutput = vec3(0.0f, 0.0f, 0.0f);
    for(int i = 0; i < MAX_POINT_LIGHTS; i++)
    {
        LightOutput += CalculateLight_Point(PointLights[i]);
    }
    for(int i = 0; i < MAX_DIRECTIONAL_LIGHTS; i++)
    {
        LightOutput += CalculateLight_Directional(DirectionalLights[i]);
    }

    //Calculate colour before fog
    vec4 prefog = vec4(LightOutput, 1.0f) * texture(ImageTexture, FragTexCoords);

    //Add fog effect
    vec4 vFogColor = vec4(0.2f, 0.2f, 0.2f, 1.0f);
    float d = distance(mWorldPos.xyz, CameraPos);
    float lerp = (d - 60.0f)/190.0f;
    lerp = clamp(lerp, 0.0f, 1.0f);

	FinalColor = mix(prefog, vFogColor, lerp);
}


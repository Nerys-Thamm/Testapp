#version 460 core
#define MAX_POINT_LIGHTS 4
#define MAX_DIRECTIONAL_LIGHTS 1

//Vertex shader inputs
in vec2 FragTexCoords;
in vec3 FragNormal;
in vec3 FragPos;

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
uniform samplerCube CubeMap;
uniform vec3 CameraPos;
uniform Material Mat[1];
uniform PointLight PointLights[MAX_POINT_LIGHTS];
uniform DirectionalLight DirectionalLights[MAX_DIRECTIONAL_LIGHTS];
uniform float RimExponent             = 4.0f;
uniform vec3 RimColor                 = vec3(1.0f, 0.0f, 0.0f);

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

    //Rim Component
    float RimFactor = 1.0f - dot(Normal, ReverseViewDir);
    RimFactor = smoothstep(0.0f, 1.0f, RimFactor);
    RimFactor = pow(RimFactor, RimExponent);
    vec3 Rim = RimFactor * RimColor * _light.Color;

    //Combine the lighting components
    vec3 CombinedLight = vec3(Ambient + Diffuse + Specular + Rim);

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

vec3 CalculateReflection()
{
    vec3 Normal = normalize(FragNormal);
    vec3 ViewDir = normalize(FragPos - CameraPos);
    vec3 ReflectDir = reflect(ViewDir, Normal);
    return ReflectDir;
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

    //Calculate final colour
    vec4 TexColor = texture(ImageTexture, FragTexCoords);
    vec4 ReflectColor = texture(CubeMap, CalculateReflection());
    vec4 MixedColor = mix(TexColor, ReflectColor, Mat[0].Reflectivity);
	FinalColor =  vec4(LightOutput, 1.0f) * MixedColor;
}


#pragma once
#include <iostream>
#include <glew.h>
#include <glfw3.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include "Camera.h"
#include <vector>
#include <map>
#define _USE_MATH_DEFINES
#include <cmath>
#include <math.h>

struct Material
{
	float Smoothness;
	float Reflectivity;
};

struct PointLight
{
	glm::vec3 Position;
	glm::vec3 Color;
	float AmbientStrength;
	float SpecularStrength;

	float AttenuationConstant;
	float AttenuationLinear;
	float AttenuationExponent;
};

struct DirectionalLight
{
	glm::vec3 Direction;
	glm::vec3 Color;
	float AmbientStrength;
	float SpecularStrength;
};

struct SpotLight
{

};

class Lighting
{
public:
	static float Global_Illumination_Strength;
	static glm::vec3 Global_Illumination_Color;
	static void AddMaterial(std::string _name);
	static Material* GetMaterial(std::string _name);
	static const int MAX_POINT_LIGHTS = 4;
	static const int MAX_DIRECTIONAL_LIGHTS = 1;
	static PointLight PointLights[MAX_POINT_LIGHTS];
	static DirectionalLight DirectionalLights[MAX_DIRECTIONAL_LIGHTS];
private:
	static std::map<std::string, Material*> m_Materials;
	
};


// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2021 Media Design School
//
// File Name   : Lighting.h
// Description : Declares structs and methods for Lighting
// Author      : Nerys Thamm
// Mail        : nerys.thamm@mds.ac.nz

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

//Struct defining material properties for an object
struct Material
{
	float Smoothness;
	float Reflectivity;
};

//Struct defining properties of a Point light with Attenuation
struct PointLight
{
	//Light properties
	glm::vec3 Position;
	glm::vec3 Color;
	float AmbientStrength;
	float SpecularStrength;

	//Attenuation properties
	float AttenuationConstant;
	float AttenuationLinear;
	float AttenuationExponent;
};

//Struct defining properties of a Directional Light
struct DirectionalLight
{
	//Light properties
	glm::vec3 Direction;
	glm::vec3 Color;
	float AmbientStrength;
	float SpecularStrength;
};

//TODO: Struct defining the properties of a spotlight
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


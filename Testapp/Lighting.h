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
};

struct PointLight
{
	glm::vec3 Position;
	glm::vec3 Color;
	float SpecularStrength;
};

class Lighting
{
public:
	static float Global_Illumination_Strength;
	static glm::vec3 Global_Illumination_Color;
	static void AddMaterial(std::string _name);
	static Material* GetMaterial(std::string _name);
private:
	static std::map<std::string, Material*> m_Materials;
};


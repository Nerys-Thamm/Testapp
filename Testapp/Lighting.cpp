// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
// 
// (c) 2021 Media Design School
//
// File Name   : Lighting.cpp
// Description : Implementation file
// Author      : Nerys Thamm
// Mail        : nerys.thamm@mds.ac.nz
#include "Lighting.h"

// ********************************************************************************
/// <summary>
/// Adds a new Material
/// </summary>
/// <param name="_name">Name of the Material</param>
// ********************************************************************************
void Lighting::AddMaterial(std::string _name)
{
    //Create new Material
    Material* newMat = new Material;
    
    //Set default values
    newMat->Smoothness = 50;
    newMat->Reflectivity = 0.0f;
    
    //Add the new Material to the map
    m_Materials.emplace(_name, newMat);

}



// ********************************************************************************
/// <summary>
/// Gets a Material by name
/// </summary>
/// <param name="_name">The name of the Material</param>
/// <returns></returns>
// ********************************************************************************
Material* Lighting::GetMaterial(std::string _name)
{
    //Finds Material with provided name and returns it
    return m_Materials.at(_name);
}

float Lighting::Global_Illumination_Strength = 0.1;
glm::vec3 Lighting::Global_Illumination_Color = glm::vec3(1.0f,1.0f,1.0f);
std::map<std::string, Material*> Lighting::m_Materials;
PointLight Lighting::PointLights[MAX_POINT_LIGHTS];
DirectionalLight Lighting::DirectionalLights[MAX_DIRECTIONAL_LIGHTS];
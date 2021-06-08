#include "Lighting.h"

void Lighting::AddMaterial(std::string _name)
{
    Material* newMat = new Material;
    
    //Set default values
    newMat->Smoothness = 32;

    m_Materials.try_emplace(_name, new Material);

}



Material* Lighting::GetMaterial(std::string _name)
{
    return m_Materials.at(_name);
}

float Lighting::Global_Illumination_Strength = 0.1;
glm::vec3 Lighting::Global_Illumination_Color = glm::vec3(1.0f,1.0f,1.0f);
std::map<std::string, Material*> Lighting::m_Materials;
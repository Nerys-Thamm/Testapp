// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2021 Media Design School
//
// File Name   : PointLightObj.h
// Description : Object interfacing with the point lights array to represent a point light within the scene
// Author      : Nerys Thamm
// Mail        : nerys.thamm@mds.ac.nz


#pragma once
#include "GameObject.h"
#include "Lighting.h"
#include "Renderable3D.h"
#include "Camera.h"
#include "ShaderLoader.h"
class PointLightObj :
    public CGameObject
{
public:
    //Constructor
    PointLightObj(PointLight* _light, glm::vec3 _position, glm::vec3 _color, float _ambientstrength, float _specularstrength, float _attenuationconstant, float _attenuationlinear, float _attenuationexponent);
	
    //Rendering 
    void Render(Camera* _camera);

    //Getters and Setters
    void SetPosition(glm::vec3 _pos);
    glm::vec3 GetPosition();

private:
    //Mesh
    Renderable3D* m_Renderable;
    //Light
    PointLight* m_Light;
    //Shader
    GLuint m_Shader;
    //Position
    glm::vec3 m_Position;
    //Implementation of Update method
    virtual void Update(float _fDeltaTime);
};


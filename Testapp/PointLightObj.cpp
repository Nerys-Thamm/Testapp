// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2021 Media Design School
//
// File Name   : PointLightObj.cpp
// Description : Defines methods and members for a point light object that represents a point light in the scene
// Author      : Nerys Thamm
// Mail        : nerys.thamm@mds.ac.nz

#include "PointLightObj.h"

// ********************************************************************************
/// <summary>
/// Constructor
/// </summary>
/// <param name="_light">Pointer to the light in the Lighting manager</param>
/// <param name="_position">Position of the light</param>
/// <param name="_color">Color of the light</param>
/// <param name="_ambientstrength">Ambient strength of the light</param>
/// <param name="_specularstrength">Specular strength of the light</param>
/// <param name="_attenuationconstant">Attenuation Constant</param>
/// <param name="_attenuationlinear">Attenuation Linear</param>
/// <param name="_attenuationexponent">Attenuation Exponent</param>
/// <returns></returns>
// ********************************************************************************
PointLightObj::PointLightObj(PointLight* _light, glm::vec3 _position, glm::vec3 _color, float _ambientstrength, float _specularstrength, float _attenuationconstant, float _attenuationlinear, float _attenuationexponent)
{
	//Set variables
	_light->Position = _position;
	_light->Color = _color;
	_light->AmbientStrength = _ambientstrength;
	_light->SpecularStrength = _specularstrength;
	_light->AttenuationConstant = _attenuationconstant;
	_light->AttenuationLinear = _attenuationlinear;
	_light->AttenuationExponent = _attenuationexponent;

	//Create new Renderable with sphere mesh
	m_Renderable = new Renderable3D(Sphere3D::GetMesh(0.1f, 10.0f), Lighting::GetMaterial("Default"));

	//Scale Renderable
	m_Renderable->Scale(glm::vec3(0.5f, 0.5f, 0.5f));

	//Set light params
	m_Light = _light;
	m_Position = _position;

	//Get Unlit Shader
	m_Shader = ShaderLoader::CreateProgram("Resources/Shaders/ClipSpace.vs",
		"Resources/Shaders/FixedColor.fs");
}

// ********************************************************************************
/// <summary>
/// Renders the mesh
/// </summary>
/// <param name="_camera">The camera to use to render</param>
// ********************************************************************************
void PointLightObj::Render(Camera* _camera)
{
	m_Renderable->Render(*_camera, m_Shader);
}

// ********************************************************************************
/// <summary>
/// Position setter
/// </summary>
/// <param name="_pos"></param>
// ********************************************************************************
void PointLightObj::SetPosition(glm::vec3 _pos)
{
	m_Position = _pos;
}

// ********************************************************************************
/// <summary>
/// Position getter
/// </summary>
/// <returns></returns>
// ********************************************************************************
glm::vec3 PointLightObj::GetPosition()
{
	return m_Position;
}

// ********************************************************************************
/// <summary>
/// THis is called every frame
/// </summary>
/// <param name="_fDeltaTime">Time since last frame</param>
// ********************************************************************************
void PointLightObj::Update(float _fDeltaTime)
{
	m_Renderable->Position(m_Position);
	m_Light->Position = m_Position;
	m_Renderable->Color(m_Light->Color);

}

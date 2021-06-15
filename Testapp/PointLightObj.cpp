#include "PointLightObj.h"

PointLightObj::PointLightObj(PointLight* _light, glm::vec3 _position, glm::vec3 _color, float _ambientstrength, float _specularstrength, float _attenuationconstant, float _attenuationlinear, float _attenuationexponent)
{
	_light->Position = _position;
	_light->Color = _color;
	_light->AmbientStrength = _ambientstrength;
	_light->SpecularStrength = _specularstrength;
	_light->AttenuationConstant = _attenuationconstant;
	_light->AttenuationLinear = _attenuationlinear;
	_light->AttenuationExponent = _attenuationexponent;
	m_Renderable = new Renderable3D(Sphere3D::GetMesh(0.1f, 10.0f), Lighting::GetMaterial("Default"));
	m_Renderable->Scale(glm::vec3(0.5f, 0.5f, 0.5f));
	m_Light = _light;
	m_Position = _position;
	m_Shader = ShaderLoader::CreateProgram("Resources/Shaders/ClipSpace.vs",
		"Resources/Shaders/FixedColor.fs");
}

void PointLightObj::Render(Camera* _camera)
{
	m_Renderable->Render(*_camera, m_Shader);
}

void PointLightObj::SetPosition(glm::vec3 _pos)
{
	m_Position = _pos;
}

glm::vec3 PointLightObj::GetPosition()
{
	return m_Position;
}

void PointLightObj::Update(float _fDeltaTime)
{
	m_Renderable->Position(m_Position);
	m_Light->Position = m_Position;
	m_Renderable->Color(m_Light->Color);

}

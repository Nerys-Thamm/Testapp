// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2021 Media Design School
//
// File Name   :
// Description :
// Author      : Nerys Thamm
// Mail        : nerys.thamm@mds.ac.nz

#pragma once
#include "GameObject.h"
#include "Mesh3D.h"

class Character :
	public CGameObject
{
public:
	Character(GLFWwindow* _window);
	void Render(Camera _camera, GLuint _program);
private:
	virtual void Update(float _fDeltaTime);
	GLFWwindow* m_currwindow = nullptr;
protected:
	Mesh3D* m_charmodel = nullptr;
	glm::vec3 m_worldpos = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 m_rotation = glm::vec3(0.0f, 0.0f, 0.0f);
};

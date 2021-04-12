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
#include <glew.h>
#include <glfw3.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include "Shape2D.h"
// --------------------------------------------------------------------------------
/// <summary>
/// 
/// </summary>
// --------------------------------------------------------------------------------
class Camera
{
public:
	glm::vec3 m_cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
	glm::vec3 m_cameraLookDir = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 m_cameraTargetPos = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 m_cameraUpDir = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::mat4 m_viewMat;
	glm::mat4 m_projectionMat;

	// ********************************************************************************
	/// <summary>
	/// 
	/// </summary>
	/// <param name="_fwindowwidth"></param>
	/// <param name="_fwindowheight"></param>
	/// <param name="_fcurrtime"></param>
	/// <returns></returns>
	// ********************************************************************************
	Camera(float _fWindowWidth, float _fWindowHeight, GLfloat& _fCurrTime);

	// ********************************************************************************
	/// <summary>
	/// 
	/// </summary>
	/// <param name="_shape"></param>
	/// <param name="_program"></param>
	// ********************************************************************************
	void Render(Shape2D& _shape, GLuint& _program);
private:
	GLfloat* m_pCurrentTime;
};

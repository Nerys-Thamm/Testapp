// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
// 
// (c) 2021 Media Design School
//
// File Name   : Camera.h
// Description : Declares class for opengl camera
// Author      : Nerys Thamm
// Mail        : nerys.thamm@mds.ac.nz

#pragma once
#include <glew.h>
#include <glfw3.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include "Shape2D.h"
#include "Mesh3D.h"
// --------------------------------------------------------------------------------
/// <summary>
/// Orthographic camera class
/// </summary>
// --------------------------------------------------------------------------------
class Camera
{
public:
	glm::vec3 m_cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
	glm::vec3 m_cameraLookDir = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 m_cameraTargetPos = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 m_cameraUpDir = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::mat4 m_viewMat = glm::mat4();
	glm::mat4 m_projectionMat = glm::mat4();

	// ********************************************************************************
	/// <summary>
	/// Constructor
	/// </summary>
	/// <param name="_fwindowwidth">Height of the window</param>
	/// <param name="_fwindowheight">Width of the window</param>
	/// <param name="_fcurrtime">Pointer to a variable that holds the current time</param>
	/// <returns></returns>
	// ********************************************************************************
	Camera(float _fWindowWidth, float _fWindowHeight, GLfloat& _fCurrTime, bool _bPerspective = true);

	// ********************************************************************************
	/// <summary>
	/// Renders a shape with the provided program
	/// </summary>
	/// <param name="_shape">The shape to be rendered</param>
	/// <param name="_program">The program to use to render the shape</param>
	// ********************************************************************************
	void Render(Shape2D& _shape, GLuint& _program);
	void Render(Mesh3D& _shape, GLuint& _program);
private:
	GLfloat* m_pCurrentTime;
};

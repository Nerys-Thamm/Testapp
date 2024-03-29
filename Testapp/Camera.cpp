// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2021 Media Design School
//
// File Name   : Camera.cpp
// Description : Defines the Camera class
// Author      : Nerys Thamm
// Mail        : nerys.thamm@mds.ac.nz

#include "Camera.h"

// ********************************************************************************
/// <summary>
/// Constructor
/// </summary>
/// <param name="_fWindowWidth">The width of the window</param>
/// <param name="_fWindowHeight">The height of the window</param>
/// <param name="_fcurrtime">Pointer to a currenttime variable</param>
/// <param name="_bPerspective">Is the camera Perspective? (Orthographic if false)</param>
/// <returns></returns>
// ********************************************************************************
Camera::Camera(float _fWindowWidth, float _fWindowHeight, GLfloat& _fcurrtime, bool _bPerspective)
{
	//Set defaults for member variables
	m_projectionMat = (_bPerspective ? glm::perspective(glm::radians(45.0f), (float)_fWindowWidth / (float)_fWindowHeight, 0.1f, 100.0f) : glm::ortho(-(_fWindowWidth * 0.5f), _fWindowWidth * 0.5f, -(_fWindowHeight * 0.5f), _fWindowHeight * 0.5f, 0.1f, 100.0f));
	m_viewMat = glm::lookAt(m_cameraPos, m_cameraPos + m_cameraLookDir, m_cameraUpDir);
	m_pCurrentTime = &_fcurrtime;
}

// ********************************************************************************
/// <summary>
/// Gets the PVM matrix
/// </summary>
/// <param name="_modelmat">The model matrix of the mesh being rendered</param>
/// <returns>The PVM matrix</returns>
// ********************************************************************************
glm::mat4 Camera::GetPVM(glm::mat4 _modelmat)
{
	m_viewMat = glm::lookAt(m_cameraPos, (m_lookAtTarget ? m_cameraTargetPos : m_cameraPos + m_cameraLookDir), m_cameraUpDir);
	return m_projectionMat * m_viewMat * _modelmat;
}
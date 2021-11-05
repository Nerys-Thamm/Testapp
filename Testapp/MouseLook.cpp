// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
// 
// (c) 2021 Media Design School
//
// File Name   : MouseLook.cpp
// Description : Implementation file
// Author      : Nerys Thamm
// Mail        : nerys.thamm@mds.ac.nz
#include "MouseLook.h"

// ********************************************************************************
/// <summary>
/// Constructor
/// </summary>
/// <param name="_parent"></param>
/// <returns></returns>
// ********************************************************************************
MouseLook::MouseLook(CEntity& _parent) : IBehaviour(_parent)
{
	m_Window = nullptr;
}

// ********************************************************************************
/// <summary>
/// Runs on first Update
/// </summary>
// ********************************************************************************
void MouseLook::OnAwake()
{
}

// ********************************************************************************
/// <summary>
/// Runs every frame
/// </summary>
/// <param name="_fDeltaTime"></param>
// ********************************************************************************
void MouseLook::OnUpdate(float _fDeltaTime)
{
	if (m_Window == nullptr) return;
	//Get cursor position
	double xpos, ypos;
	glfwGetCursorPos(m_Window, &xpos, &ypos);

	//Get change in cursor position since last frame
	glm::vec2 CursorDelta = glm::vec2(xpos, ypos) - m_lastcursorpos;

	//Invert
	if (!m_inverted)
	{
		CursorDelta.x *= -1;
	}
	else
	{
		CursorDelta.y *= -1;
	}
	if (m_lockPitch)
	{
		CursorDelta.y = 0.0f;
	}
	if (m_lockYaw)
	{
		CursorDelta.x = 0.0f;
	}
	

	//Rotate the camera using cursor delta
	m_entity.m_transform.rotation += glm::vec3(CursorDelta.y * m_sensitivity, CursorDelta.x * m_sensitivity, 0.0f);

	m_entity.m_transform.rotation.x = std::clamp(m_entity.m_transform.rotation.x, -80.0f, 80.0f);
	m_lastcursorpos = glm::vec2(xpos, ypos);
	
	
}

// ********************************************************************************
/// <summary>
/// Runs at a fixed timestep
/// </summary>
// ********************************************************************************
void MouseLook::OnFixedUpdate()
{
}

// ********************************************************************************
/// <summary>
/// Runs after all other updates are complete
/// </summary>
/// <param name="_fDeltaTime"></param>
// ********************************************************************************
void MouseLook::OnLateUpdate(float _fDeltaTime)
{
}

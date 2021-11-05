// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
// 
// (c) 2021 Media Design School
//
// File Name   : MouseLook.h
// Description : Header file
// Author      : Nerys Thamm
// Mail        : nerys.thamm@mds.ac.nz
#pragma once
#include "Entity.h"
class MouseLook :
    public IBehaviour
{
public:
	MouseLook(CEntity& _parent);
	void SetWindow(GLFWwindow* _window, glm::vec2 _windowSize) {
		m_Window = _window; m_windowExtents = _windowSize;
		m_lastcursorpos = _windowSize / 2.0f;
	}
	void SetIsInverted(bool _isInverted) { m_inverted = _isInverted; }
	void SetSensitivity(float _sensitivity) { m_sensitivity = _sensitivity; }
	void SetAxisLockState(bool _yawLocked, bool _pitchLocked) { m_lockYaw = _yawLocked; m_lockPitch = _pitchLocked; }
private:
	GLFWwindow* m_Window;
	glm::vec2 m_windowExtents;
	glm::vec2 m_lastcursorpos;
	float m_sensitivity = 1.0f;
	bool m_inverted = false;
	bool m_lockYaw = false, m_lockPitch = false;
	void OnAwake();
	void OnUpdate(float _fDeltaTime);
	void OnFixedUpdate();
	void OnLateUpdate(float _fDeltaTime);
};


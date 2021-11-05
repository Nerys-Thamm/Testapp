// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
// 
// (c) 2021 Media Design School
//
// File Name   : FreeCam.h
// Description : Header file
// Author      : Nerys Thamm
// Mail        : nerys.thamm@mds.ac.nz
#pragma once
#include "GameObject.h"
#include "Camera.h"
#include "SceneManager.h"
class FreeCam :
    public CGameObject
{
public:
    FreeCam(GLFWwindow* _window, float _fWindowWidth, float _fWindowHeight, GLfloat& _fcurrtime, bool _bPerspective);
    Camera* GetCamera();
private:
    Camera* m_Camera;
    GLFWwindow* m_Window;
    glm::vec2 m_lastcursorpos;
    glm::vec3 m_camerarightvector;
    glm::vec2 m_cameraviewangle;
    virtual void Update(float _fDeltaTime);
};


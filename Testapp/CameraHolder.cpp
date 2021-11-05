// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
// 
// (c) 2021 Media Design School
//
// File Name   : CameraHolder.cpp
// Description : Implementation file
// Author      : Nerys Thamm
// Mail        : nerys.thamm@mds.ac.nz
#include "CameraHolder.h"

// ********************************************************************************
/// <summary>
/// Constructor
/// </summary>
/// <param name="_parent"></param>
/// <returns></returns>
// ********************************************************************************
CameraHolder::CameraHolder(CEntity& _parent) : IBehaviour(_parent)
{
	m_camera = nullptr;
}

// ********************************************************************************
/// <summary>
/// Runs on first update
/// </summary>
// ********************************************************************************
void CameraHolder::OnAwake()
{
}

// ********************************************************************************
/// <summary>
/// Runs every frame
/// </summary>
/// <param name="_fDeltaTime"></param>
// ********************************************************************************
void CameraHolder::OnUpdate(float _fDeltaTime)
{
	if (m_camera == nullptr)return;
	m_camera->m_cameraPos = m_entity.m_globalTransform.position;
	m_camera->m_cameraLookDir = m_entity.m_globalTransform.Forward();
}

// ********************************************************************************
/// <summary>
/// Runs every fixed timestep
/// </summary>
// ********************************************************************************
void CameraHolder::OnFixedUpdate()
{
}

// ********************************************************************************
/// <summary>
/// Runs after all other Updates
/// </summary>
/// <param name="_fDeltaTime"></param>
// ********************************************************************************
void CameraHolder::OnLateUpdate(float _fDeltaTime)
{
}

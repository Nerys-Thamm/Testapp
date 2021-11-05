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

CameraHolder::CameraHolder(CEntity& _parent) : IBehaviour(_parent)
{
	m_camera = nullptr;
}

void CameraHolder::OnAwake()
{
}

void CameraHolder::OnUpdate(float _fDeltaTime)
{
	if (m_camera == nullptr)return;
	m_camera->m_cameraPos = m_entity.m_globalTransform.position;
	m_camera->m_cameraLookDir = m_entity.m_globalTransform.Forward();
}

void CameraHolder::OnFixedUpdate()
{
}

void CameraHolder::OnLateUpdate(float _fDeltaTime)
{
}

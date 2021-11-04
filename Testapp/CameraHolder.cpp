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

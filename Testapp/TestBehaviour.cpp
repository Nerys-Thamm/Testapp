#include "TestBehaviour.h"

TestBehaviour::TestBehaviour(CEntity& _parent) : IBehaviour(_parent)
{
}

void TestBehaviour::OnAwake()
{
	return;
}

void TestBehaviour::OnUpdate(float _fDeltaTime)
{
	m_entity.m_transform.rotation.x += _fDeltaTime * 10;
}

void TestBehaviour::OnFixedUpdate()
{
	return;
}

void TestBehaviour::OnLateUpdate(float _fDeltaTime)
{
	return;
}

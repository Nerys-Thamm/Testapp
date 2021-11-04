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
	m_entity.m_transform.rotation.y = ((sin((float)glfwGetTime()) * 30.0f));
	if (m_entity.m_transform.rotation.y > 360.0f)
	{
		m_entity.m_transform.rotation.y = 0.0f;
	}

}

void TestBehaviour::OnFixedUpdate()
{
	return;
}

void TestBehaviour::OnLateUpdate(float _fDeltaTime)
{
	return;
}

// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
// 
// (c) 2021 Media Design School
//
// File Name   : TestBehaviour.cpp
// Description : Implementation file
// Author      : Nerys Thamm
// Mail        : nerys.thamm@mds.ac.nz
#include "TestBehaviour.h"

// ********************************************************************************
/// <summary>
/// Constructor
/// </summary>
/// <param name="_parent"></param>
/// <returns></returns>
// ********************************************************************************
TestBehaviour::TestBehaviour(CEntity& _parent) : IBehaviour(_parent)
{
}

// ********************************************************************************
/// <summary>
/// 
/// </summary>
// ********************************************************************************
void TestBehaviour::OnAwake()
{
	return;
}

// ********************************************************************************
/// <summary>
/// 
/// </summary>
/// <param name="_fDeltaTime"></param>
// ********************************************************************************
void TestBehaviour::OnUpdate(float _fDeltaTime)
{
	m_entity.m_transform.rotation.y = ((sin(glfwGetTime()) * 30.0f));
	if (m_entity.m_transform.rotation.y > 360.0f)
	{
		m_entity.m_transform.rotation.y = 0.0f;
	}

}

// ********************************************************************************
/// <summary>
/// 
/// </summary>
// ********************************************************************************
void TestBehaviour::OnFixedUpdate()
{
	return;
}

// ********************************************************************************
/// <summary>
/// 
/// </summary>
/// <param name="_fDeltaTime"></param>
// ********************************************************************************
void TestBehaviour::OnLateUpdate(float _fDeltaTime)
{
	return;
}

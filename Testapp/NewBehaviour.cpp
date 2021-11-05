// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
// 
// (c) 2021 Media Design School
//
// File Name   : NewBehaviour.cpp
// Description : Implementation file
// Author      : Nerys Thamm
// Mail        : nerys.thamm@mds.ac.nz

#include "NewBehaviour.h"

//*********************************
// BEHAVIOR | NewBehaviour 
//*********************************

// ********************************************************************************
/// <summary>
/// Constructor
/// </summary>
/// <param name="_parent"></param>
/// <returns></returns>
// ********************************************************************************
NewBehaviour::NewBehaviour(CEntity& _parent) : IBehaviour(_parent)
{
}

// ********************************************************************************
/// <summary>
/// Runs before the object is updated for the first time
/// </summary>
// ********************************************************************************
void NewBehaviour::OnAwake()
{
}

// ********************************************************************************
/// <summary>
/// Runs every frame
/// </summary>
/// <param name="_fDeltaTime"></param>
// ********************************************************************************
void NewBehaviour::OnUpdate(float _fDeltaTime)
{
}

// ********************************************************************************
/// <summary>
/// Runs every frame, at a fixed interval
/// </summary>
// ********************************************************************************
void NewBehaviour::OnFixedUpdate()
{
}

// ********************************************************************************
/// <summary>
/// Runs every frame, after all other updates have completed
/// </summary>
/// <param name="_fDeltaTime"></param>
// ********************************************************************************
void NewBehaviour::OnLateUpdate(float _fDeltaTime)
{
}

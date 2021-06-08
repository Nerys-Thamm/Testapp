// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2021 Media Design School
//
// File Name   : Collider2D.h
// Description : Header file for 2D collision system
// Author      : Nerys Thamm
// Mail        : nerys.thamm@mds.ac.nz

#pragma once
#include <iostream>
#include <glew.h>
#include <glfw3.h>
#include <glm.hpp>
#include <vector>
#include "GameObject.h"
class Collider2D
{
	friend class Collision2D;
public:
	unsigned int m_layerMask = 0x0;
	glm::vec3* m_pos = nullptr;
	glm::vec3* m_scale = nullptr;
	CGameObject* GetParentObject();
protected:
	Collider2D();
	CGameObject* m_parentObj = nullptr;
};

class RectCollider2D :
	public Collider2D
{
public:
	RectCollider2D(CGameObject* _parentObj, glm::vec3* _objPos, glm::vec3* _objScale);
};

class OvalCollider2D :
	public Collider2D
{
};

class Collision2D
{
	friend class Collider2D;
public:
	static bool CheckCollision(RectCollider2D _rect, glm::vec2 _point);

	//TODO: Implement these Collision Checks:
	/*static bool CheckCollision(RectCollider2D _rect, RectCollider2D _otherRect);
	static bool CheckCollision(OvalCollider2D _oval, glm::vec2 _point);
	static bool CheckCollision(OvalCollider2D _oval, OvalCollider2D _otherOval);
	static bool CheckCollision(OvalCollider2D _oval, RectCollider2D _rect);
	static bool CheckCollision(RectCollider2D _rect, OvalCollider2D _oval);*/
};

// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2021 Media Design School
//
// File Name   : Collider2D.cpp
// Description : Implementation file for 2D collision system
// Author      : Nerys Thamm
// Mail        : nerys.thamm@mds.ac.nz

#include "Collider2D.h"

// ********************************************************************************
/// <summary>
/// Checks whether a point is colliding with a rect collider
/// </summary>
/// <param name="_rect">The rect collider</param>
/// <param name="_point">The point</param>
/// <returns>If a collision was found</returns>
// ********************************************************************************
bool Collision2D::CheckCollision(RectCollider2D _rect, glm::vec2 _point)
{
	if ((_point.x < (_rect.m_pos->x + _rect.m_scale->x / 2)) && (-_point.y < (_rect.m_pos->y + _rect.m_scale->y / 2)) && (_point.x > (_rect.m_pos->x - _rect.m_scale->x / 2)) && (-_point.y > (_rect.m_pos->y - _rect.m_scale->y / 2)))
	{
		return true;
	}
	else
	{
		return false;
	}
}

//TODO: Implement these collision checks:
//bool Collision2D::CheckCollision(RectCollider2D _rect, RectCollider2D _otherRect)
//{
//	return false;
//}
//
//
//bool Collision2D::CheckCollision(OvalCollider2D _oval, glm::vec2 _point)
//{
//	return false;
//}
//
//
//bool Collision2D::CheckCollision(OvalCollider2D _oval, OvalCollider2D _otherOval)
//{
//	return false;
//}
//
//bool Collision2D::CheckCollision(OvalCollider2D _oval, RectCollider2D _rect)
//{
//	return false;
//}
//
//
//bool Collision2D::CheckCollision(RectCollider2D _rect, OvalCollider2D _oval)
//{
//	return false;
//}

// ********************************************************************************
/// <summary>
///Constructor
/// </summary>
/// <param name="_parentObj">Object the collider is part of</param>
/// <param name="_objPos">Pointer to position variable of the object</param>
/// <param name="_objScale">Pointer to the scale variable of the object</param>
/// <returns></returns>
// ********************************************************************************
RectCollider2D::RectCollider2D(CGameObject* _parentObj, glm::vec3* _objPos, glm::vec3* _objScale)
{
	m_parentObj = _parentObj;
	m_pos = _objPos;
	m_scale = _objScale;
}


// ********************************************************************************
/// <summary>
/// Gets the Parent Object
/// </summary>
/// <returns>The Parent Object</returns>
// ********************************************************************************
CGameObject* Collider2D::GetParentObject()
{
	return m_parentObj;
}

// ********************************************************************************
/// <summary>
/// Constructor, should not ever be manually called/
/// </summary>
/// <returns></returns>
// ********************************************************************************
Collider2D::Collider2D()
{
}
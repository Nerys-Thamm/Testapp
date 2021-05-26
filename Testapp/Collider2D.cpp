#include "Collider2D.h"

bool Collision2D::CheckCollision(RectCollider2D _rect, glm::vec2 _point)
{
	if((_point.x < (_rect.m_pos->x + _rect.m_scale->x)) && (_point.y < (_rect.m_pos->y + _rect.m_scale->y)) && (_point.x > (_rect.m_pos->x - _rect.m_scale->x)) && (_point.y > (_rect.m_pos->y - _rect.m_scale->y)))
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool Collision2D::CheckCollision(RectCollider2D _rect, RectCollider2D _otherRect)
{
	return false;
}

bool Collision2D::CheckCollision(OvalCollider2D _oval, glm::vec2 _point)
{
	return false;
}

bool Collision2D::CheckCollision(OvalCollider2D _oval, OvalCollider2D _otherOval)
{
	return false;
}

bool Collision2D::CheckCollision(OvalCollider2D _oval, RectCollider2D _rect)
{
	return false;
}

bool Collision2D::CheckCollision(RectCollider2D _rect, OvalCollider2D _oval)
{
	return false;
}

RectCollider2D::RectCollider2D(CGameObject* _parentObj, glm::vec3* _objPos, glm::vec3* _objScale)
{
	m_parentObj = _parentObj;
	m_pos = _objPos;
	m_scale = _objScale;
}

CGameObject* Collider2D::GetParentObject()
{
	return nullptr;
}

Collider2D::Collider2D()
{
}

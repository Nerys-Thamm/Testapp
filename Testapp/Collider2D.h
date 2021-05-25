#pragma once
#include <iostream>
#include <glew.h>
#include <glfw3.h>
#include <glm.hpp>
#include <vector>
class Collider2D
{
	friend class Collision2D;
protected:
	Collider2D();
	unsigned int m_layerMask = 0x0;
	glm::vec2 m_pos;
	glm::vec2 m_scale;
};

class RectCollider2D :
	protected Collider2D
{

};

class OvalCollider2D :
	protected Collider2D
{

};

class Collision2D
{
	friend class Collider2D;
public:
	static bool CheckCollision(RectCollider2D _rect, glm::vec2 _point);
	static bool CheckCollision(RectCollider2D _rect, RectCollider2D _otherRect);
	static bool CheckCollision(OvalCollider2D _oval, glm::vec2 _point);
	static bool CheckCollision(OvalCollider2D _oval, OvalCollider2D _otherOval);
	static bool CheckCollision(OvalCollider2D _oval, RectCollider2D _rect);
	static bool CheckCollision(RectCollider2D _rect, OvalCollider2D _oval);
};



#pragma once
#include "GameObject.h"
#include "Collider2D.h"
#include "Shape2D.h"

class FadingRect:
	public CGameObject
{
public:
	RectCollider2D* m_collider = nullptr;
	Shape2D* m_rect = nullptr;
	FadingRect(glm::vec3 _pos, glm::vec3 _scale, GLuint _tex1, GLuint _tex2);
	virtual void Update(float _fDeltaTime);
	void Render(Camera _camera, GLuint _program);
private:
	bool m_ishovering = false;
	bool m_isfading = false;

};


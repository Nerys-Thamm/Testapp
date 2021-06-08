// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2021 Media Design School
//
// File Name   : UIButton.h
// Description : Implementation file for Button
// Author      : Nerys Thamm
// Mail        : nerys.thamm@mds.ac.nz

#pragma once
#include "GameObject.h"
#include "Collider2D.h"
#include "Shape2D.h"

class UIButton :
	public CGameObject
{
public:
	enum class State
	{
		BUTTON_PRESSED,
		BUTTON_HELD,
		BUTTON_RELEASED,
	};
	State GetState();
	UIButton(glm::vec3 _pos, glm::vec3 _scale, GLuint _texture, GLuint _hovertexture, GLuint _pressedtexture);
	void Render(Camera _camera, GLuint _program);
private:
	virtual void Update(float _fDeltaTime);
	RectCollider2D* m_collider = nullptr;
	Shape2D* m_rect = nullptr;
	bool m_ishovering = false;
	bool m_clicked = false;
	bool m_clickedthisframe = false;
	bool m_isfading = false;
};

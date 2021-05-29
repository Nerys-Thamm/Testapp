// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2021 Media Design School
//
// File Name   : UIButton.cpp
// Description : Implementation file for the UIButton class
// Author      : Nerys Thamm
// Mail        : nerys.thamm@mds.ac.nz

#include "UIButton.h"
#include "Config.h"

// ********************************************************************************
/// <summary>
/// Gets the state of the button. BUTTON_PRESSED means the button was pressed this frame,
/// BUTTON_HELD means the button was not pressed this frame but is being held down,
/// BUTTON_RELEASED means the button is not currently being pressed.
/// </summary>
/// <returns>The state of the button</returns>
// ********************************************************************************
UIButton::State UIButton::GetState()
{
	if (m_clickedthisframe)
	{
		return UIButton::State::BUTTON_PRESSED;
	}
	else if (m_clicked)
	{
		return UIButton::State::BUTTON_HELD;
	}
	else
	{
		return UIButton::State::BUTTON_RELEASED;
	}
}

// ********************************************************************************
/// <summary>
/// Constructor
/// </summary>
/// <param name="_pos">Position</param>
/// <param name="_scale">Scale</param>
/// <param name="_texture">Default texture for the button</param>
/// <param name="_hovertexture">Texture that is displayed when the mouse hovers over the button</param>
/// <param name="_pressedtexture">Texture that is displayed when the button is pressed</param>
/// <returns></returns>
// ********************************************************************************
UIButton::UIButton(glm::vec3 _pos, glm::vec3 _scale, GLuint _texture, GLuint _hovertexture, GLuint _pressedtexture)
{
	m_rect = new Quad2D();
	m_rect->Scale(_scale);
	m_rect->Position(_pos);
	m_rect->AddTexture(_texture);
	m_rect->AddTexture(_hovertexture);
	m_rect->AddTexture(_pressedtexture);
	m_collider = new RectCollider2D(this, &m_rect->m_position, &m_rect->m_scale);
}

// ********************************************************************************
/// <summary>
/// Is called every frame
/// </summary>
/// <param name="_fDeltaTime"></param>
// ********************************************************************************
void UIButton::Update(float _fDeltaTime)
{
	if (m_isfading) { m_isfading = false; }
	bool washoveringlastframe = m_ishovering;
	
	//Get Cursor position
	double xpos;
	double ypos;
	glfwGetCursorPos(CObjectController::GetMainWindow(), &xpos, &ypos);

	//Check if the mouse is colliding with the button
	m_ishovering = Collision2D::CheckCollision(*m_collider, glm::vec2(xpos - cfWINDOW_WIDTH() / 2, ypos - cfWINDOW_HEIGHT() / 2));
	if (m_ishovering != washoveringlastframe) { m_isfading = true; }


	if (m_ishovering) //If the mouse is colliding with the button...
	{
		if (glfwGetMouseButton(CObjectController::GetMainWindow(), GLFW_MOUSE_BUTTON_1) == GLFW_PRESS) //If the left mouse button is being pressed...
		{
			if (!m_clicked) //If the button press was not registered before this frame...
			{
				//TODO: Button press callback
				m_clickedthisframe = true; //Then the button must have been pressed this frame
				m_clicked = true;
			}
			else
			{
				m_clickedthisframe = false; //Then the button must have been clicked on a previous frame
			}

			m_rect->m_iTextureIndex = 2; //Set the texture to the Pressed texture
		}
		else
		{
			m_rect->m_iTextureIndex = 1; //Set the texture to the Hover texture
			m_clicked = false; //The button is not currently being pressed
		}
	}
	else
	{
		m_rect->m_iTextureIndex = 0; //Set the texture to the default texture
	}
}

// ********************************************************************************
/// <summary>
/// Renders the Button
/// </summary>
/// <param name="_camera">The camera to render the button with</param>
/// <param name="_program">The program to render the button with</param>
// ********************************************************************************
void UIButton::Render(Camera _camera, GLuint _program)
{
	m_rect->Render(_camera, _program);
}
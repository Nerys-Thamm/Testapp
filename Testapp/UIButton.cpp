#include "UIButton.h"
#include "Config.h"



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


void UIButton::Update(float _fDeltaTime)
{
	if (m_isfading) { m_isfading = false; }
	bool washoveringlastframe = m_ishovering;
	double xpos;
	double ypos;
	glfwGetCursorPos(CObjectController::GetMainWindow(), &xpos, &ypos);
	m_ishovering = Collision2D::CheckCollision(*m_collider, glm::vec2(xpos - cfWINDOW_WIDTH() / 2, ypos - cfWINDOW_HEIGHT() / 2));
	if (m_ishovering != washoveringlastframe) { m_isfading = true; }
	if (m_ishovering)
	{
		if (glfwGetMouseButton(CObjectController::GetMainWindow(), GLFW_MOUSE_BUTTON_1) == GLFW_PRESS)
		{
			if (!m_clicked)
			{
				//TODO: Button press callback
				m_clickedthisframe = true;
				m_clicked = true;
			}
			else
			{
				m_clickedthisframe = false;
			}
			m_rect->m_iTextureIndex = 2;
			
		}
		else
		{
			m_rect->m_iTextureIndex = 1;
			m_clicked = false;
		}
	}
	else
	{
		m_rect->m_iTextureIndex = 0;

	}
}

void UIButton::Render(Camera _camera, GLuint _program)
{
	m_rect->Render(_camera, _program);
}

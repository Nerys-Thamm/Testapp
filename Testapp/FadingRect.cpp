#include "FadingRect.h"
#include "Config.h"



FadingRect::FadingRect(glm::vec3 _pos, glm::vec3 _scale, GLuint _tex1, GLuint _tex2)
{
	m_rect = new Quad2D();
	m_rect->Scale(_scale);
	m_rect->Position(_pos);
	m_rect->AddTexture(_tex1);
	m_rect->AddTexture(_tex2);
	m_collider = new RectCollider2D(this, &m_rect->m_position, new glm::vec3(100,100, 0));
}


void FadingRect::Update(float _fDeltaTime)
{
	double xpos;
	double ypos;
	glfwGetCursorPos(CObjectController::GetMainWindow(), &xpos, &ypos);
	m_ishovering = Collision2D::CheckCollision(*m_collider, glm::vec2(xpos - cfWINDOW_WIDTH() / 2, ypos - cfWINDOW_HEIGHT() / 2));
	if (m_ishovering)
	{
		m_rect->m_iTextureIndex = 0;
		m_rect->m_iFadeIndex = 1;
	}
	else
	{
		m_rect->m_iTextureIndex = 1;
		m_rect->m_iFadeIndex = 0;
	}
}

void FadingRect::Render(Camera _camera, GLuint _program)
{
	m_rect->Render(_camera, _program);
}

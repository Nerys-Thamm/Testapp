#include "Character.h"

Character::Character(GLFWwindow* _window)
{
	m_charmodel = new Cube3D();
	m_charmodel->Scale(glm::vec3(0.5f, 1.0f, 0.5f));
	m_currwindow = _window;
}

void Character::Render(Camera _camera, GLuint _program)
{
	m_charmodel->Position(m_worldpos);
	m_charmodel->Rotation(m_rotation);
	m_charmodel->Render(_camera, _program);
}

void Character::Update(float _fDeltaTime)
{
	GLFWgamepadstate state;
	glm::vec3 direction = glm::vec3(0.0f, 0.0f, 0.0f);
	bool keypressed = false;
	//INPUT
	if (glfwGetKey(m_currwindow, GLFW_KEY_W) == GLFW_PRESS)
	{
		direction += glm::vec3(0.0f, 0.0f, 1.0f);
		keypressed = true;
	}
	if (glfwGetKey(m_currwindow, GLFW_KEY_A) == GLFW_PRESS)
	{
		direction += glm::vec3(1.0f, 0.0f, 0.0f);
		keypressed = true;
	}
	if (glfwGetKey(m_currwindow, GLFW_KEY_S) == GLFW_PRESS)
	{
		direction += glm::vec3(0.0f, 0.0f, -1.0f);
		keypressed = true;
	}
	if (glfwGetKey(m_currwindow, GLFW_KEY_D) == GLFW_PRESS)
	{
		direction += glm::vec3(-1.0f, 0.0f, 0.0f);
		keypressed = true;
	}
	if (glfwGetKey(m_currwindow, GLFW_KEY_Q) == GLFW_PRESS)
	{
		direction += glm::vec3(0.0f, -1.0f, 0.0f);
		keypressed = true;
	}
	if (glfwGetKey(m_currwindow, GLFW_KEY_E) == GLFW_PRESS)
	{
		direction += glm::vec3(0.0f, 1.0f, 0.0f);
		keypressed = true;
	}
	if (keypressed)
	{
		m_worldpos += glm::normalize(direction) * _fDeltaTime;
	}

	if (glfwGetGamepadState(GLFW_JOYSTICK_2, &state))
	{
		direction += glm::vec3(-state.axes[GLFW_GAMEPAD_AXIS_LEFT_X], -state.axes[GLFW_GAMEPAD_AXIS_LEFT_TRIGGER] + state.axes[GLFW_GAMEPAD_AXIS_RIGHT_TRIGGER], -state.axes[GLFW_GAMEPAD_AXIS_LEFT_Y]);
		m_worldpos += direction * _fDeltaTime;
	}
}

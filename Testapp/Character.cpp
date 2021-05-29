// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2021 Media Design School
//
// File Name   : Character.cpp
// Description : Implementation file for the Character class
// Author      : Nerys Thamm
// Mail        : nerys.thamm@mds.ac.nz

#include "Character.h"

// ********************************************************************************
/// <summary>
/// Constructor
/// </summary>
/// <param name="_window">The window the object is in</param>
/// <returns></returns>
// ********************************************************************************
Character::Character(GLFWwindow* _window)
{
	m_charmodel = new Cube3D();
	m_charmodel->Scale(glm::vec3(0.5f, 1.0f, 0.5f));
	m_currwindow = _window;
}

// ********************************************************************************
/// <summary>
/// Renders the object
/// </summary>
/// <param name="_camera">The camera to render with</param>
/// <param name="_program">The program to use to render</param>
// ********************************************************************************
void Character::Render(Camera _camera, GLuint _program)
{
	m_charmodel->Position(m_worldpos);
	m_charmodel->Rotation(m_rotation);
	m_charmodel->Render(_camera, _program);
}

// ********************************************************************************
/// <summary>
/// Is called every frame
/// </summary>
/// <param name="_fDeltaTime">Time since last frame</param>
// ********************************************************************************
void Character::Update(float _fDeltaTime)
{
	GLFWgamepadstate state;
	glm::vec3 direction = glm::vec3(0.0f, 0.0f, 0.0f);
	bool keypressed = false;

	//INPUT

	//Add to direction vector for each direction key being pressed
	if (glfwGetKey(m_currwindow, GLFW_KEY_W) == GLFW_PRESS) 
	{
		direction += glm::vec3(0.0f, 0.0f, -1.0f);
		keypressed = true;
	}
	if (glfwGetKey(m_currwindow, GLFW_KEY_A) == GLFW_PRESS)
	{
		direction += glm::vec3(-1.0f, 0.0f, 0.0f);
		keypressed = true;
	}
	if (glfwGetKey(m_currwindow, GLFW_KEY_S) == GLFW_PRESS)
	{
		direction += glm::vec3(0.0f, 0.0f, 1.0f);
		keypressed = true;
	}
	if (glfwGetKey(m_currwindow, GLFW_KEY_D) == GLFW_PRESS)
	{
		direction += glm::vec3(1.0f, 0.0f, 0.0f);
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

	//If a key is pressed, normalise the direction vector and multiply by deltatime for movement
	if (keypressed)
	{
		m_worldpos += glm::normalize(direction) * _fDeltaTime;
	}

	//GAMEPAD INPUT
	if (glfwGetGamepadState(GLFW_JOYSTICK_2, &state))
	{
		direction += glm::vec3(state.axes[GLFW_GAMEPAD_AXIS_LEFT_X], -state.axes[GLFW_GAMEPAD_AXIS_LEFT_TRIGGER] + state.axes[GLFW_GAMEPAD_AXIS_RIGHT_TRIGGER], state.axes[GLFW_GAMEPAD_AXIS_LEFT_Y]);
		m_worldpos += direction * _fDeltaTime;
	}
}
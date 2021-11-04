#include "CharacterMotor.h"

CharacterMotor::CharacterMotor(CEntity& _parent) : IBehaviour(_parent)
{
	m_Window = nullptr;
}

void CharacterMotor::OnAwake()
{
}

void CharacterMotor::OnUpdate(float _fDeltaTime)
{
	if (m_Window == nullptr) return;
	//Init Variables
	glm::vec3 direction = glm::vec3(0.0f, 0.0f, 0.0f);
	bool keypressed = false;

	//INPUT

	//Add to direction vector for each direction key being pressed
	if (glfwGetKey(m_Window, GLFW_KEY_W) == GLFW_PRESS && !(glfwGetKey(m_Window, GLFW_KEY_S) == GLFW_PRESS))
	{
		direction += glm::vec3(0.0f, 0.0f, -1.0f);
		keypressed = true;
	}
	if (glfwGetKey(m_Window, GLFW_KEY_A) == GLFW_PRESS && !(glfwGetKey(m_Window, GLFW_KEY_D) == GLFW_PRESS))
	{
		direction += glm::vec3(-1.0f, 0.0f, 0.0f);
		keypressed = true;
	}
	if (glfwGetKey(m_Window, GLFW_KEY_S) == GLFW_PRESS && !(glfwGetKey(m_Window, GLFW_KEY_W) == GLFW_PRESS))
	{
		direction += glm::vec3(0.0f, 0.0f, 1.0f);
		keypressed = true;
	}
	if (glfwGetKey(m_Window, GLFW_KEY_D) == GLFW_PRESS && !(glfwGetKey(m_Window, GLFW_KEY_A) == GLFW_PRESS))
	{
		direction += glm::vec3(1.0f, 0.0f, 0.0f);
		keypressed = true;
	}
	
	glm::vec3 Movement = (m_entity.m_transform.Forward() * -direction.z) + (m_entity.m_transform.Right() * -direction.x) + (glm::vec3(0.0f, 1.0f, 0.0f) * direction.y);

	//If a key is pressed, normalise the direction vector and multiply by deltatime for movement
	if (keypressed)
	{
		m_entity.m_transform.position += glm::normalize(Movement) * _fDeltaTime * 10.0f;
	}
}

void CharacterMotor::OnFixedUpdate()
{
}

void CharacterMotor::OnLateUpdate(float _fDeltaTime)
{
}

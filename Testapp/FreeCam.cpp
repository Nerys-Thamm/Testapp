#include "FreeCam.h"

FreeCam::FreeCam(GLFWwindow* _window, float _fWindowWidth, float _fWindowHeight, GLfloat& _fcurrtime, bool _bPerspective)
{
	m_Camera = new Camera(_fWindowWidth, _fWindowHeight, _fcurrtime, _bPerspective);
	m_Window = _window;
	m_lastcursorpos = glm::vec2(_fWindowWidth/2, _fWindowHeight/2);
	m_cameraviewangle = glm::vec2(0.0f, 0.0f);
	m_camerarightvector = glm::vec3(0.0f, 0.0f, 0.0f);
}

Camera* FreeCam::GetCamera()
{
	return m_Camera;
}

void FreeCam::Update(float _fDeltaTime)
{
	glm::vec3 direction = glm::vec3(0.0f, 0.0f, 0.0f);
	bool keypressed = false;

	//INPUT

	//Add to direction vector for each direction key being pressed
	if (glfwGetKey(m_Window, GLFW_KEY_W) == GLFW_PRESS)
	{
		direction += glm::vec3(0.0f, 0.0f, -1.0f);
		keypressed = true;
	}
	if (glfwGetKey(m_Window, GLFW_KEY_A) == GLFW_PRESS)
	{
		direction += glm::vec3(-1.0f, 0.0f, 0.0f);
		keypressed = true;
	}
	if (glfwGetKey(m_Window, GLFW_KEY_S) == GLFW_PRESS)
	{
		direction += glm::vec3(0.0f, 0.0f, 1.0f);
		keypressed = true;
	}
	if (glfwGetKey(m_Window, GLFW_KEY_D) == GLFW_PRESS)
	{
		direction += glm::vec3(1.0f, 0.0f, 0.0f);
		keypressed = true;
	}
	if (glfwGetKey(m_Window, GLFW_KEY_Q) == GLFW_PRESS)
	{
		direction += glm::vec3(0.0f, -1.0f, 0.0f);
		keypressed = true;
	}
	if (glfwGetKey(m_Window, GLFW_KEY_E) == GLFW_PRESS)
	{
		direction += glm::vec3(0.0f, 1.0f, 0.0f);
		keypressed = true;
	}
	double xpos, ypos;
	glfwGetCursorPos(m_Window, &xpos, &ypos);
	glm::vec2 CursorDelta = glm::vec2(xpos, ypos) - m_lastcursorpos;

	CursorDelta.x *= -1;

	m_cameraviewangle += CursorDelta * (_fDeltaTime/2);

	if (m_cameraviewangle.y > -18.0f)
	{
		m_cameraviewangle.y = -18.0f;
	}
	if (m_cameraviewangle.y < -20.0f)
	{
		m_cameraviewangle.y = -20.0f;
	}

	//std::cout << std::to_string(m_cameraviewangle.y) << std::endl;

	m_Camera->m_cameraLookDir = glm::vec3(cos(m_cameraviewangle.y) * sin(m_cameraviewangle.x), -sin(m_cameraviewangle.y), cos(m_cameraviewangle.y) * cos(m_cameraviewangle.x));
	m_camerarightvector = glm::vec3(cos(m_cameraviewangle.x), 0.0f, -sin(m_cameraviewangle.x));

	glm::vec3 Movement = (m_Camera->m_cameraLookDir * -direction.z) + (m_camerarightvector * -direction.x) + (glm::vec3(0.0f,1.0f,0.0f) * direction.y);

	//If a key is pressed, normalise the direction vector and multiply by deltatime for movement
	if (keypressed)
	{
		m_Camera->m_cameraPos += glm::normalize(Movement)  * _fDeltaTime * 10.0f;
	}

	m_lastcursorpos = glm::vec2(xpos, ypos);
}

#include "FreeCam.h"

// ********************************************************************************
/// <summary>
/// Constructor
/// </summary>
/// <param name="_window">The window to capture input from</param>
/// <param name="_fWindowWidth">The width of the window</param>
/// <param name="_fWindowHeight">The height of the window</param>
/// <param name="_fcurrtime">Reference to float containing the time since program start</param>
/// <param name="_bPerspective">Whether or not the Camera is perspective, if this is false the camera is Orthographic</param>
/// <returns></returns>
// ********************************************************************************
FreeCam::FreeCam(GLFWwindow* _window, float _fWindowWidth, float _fWindowHeight, GLfloat& _fcurrtime, bool _bPerspective)
{
	//Initialise variables
	m_Camera = new Camera(_fWindowWidth, _fWindowHeight, _fcurrtime, _bPerspective);
	m_Window = _window;
	m_lastcursorpos = glm::vec2(_fWindowWidth / 2, _fWindowHeight / 2);
	m_cameraviewangle = glm::vec2(0.0f, 0.0f);
	m_camerarightvector = glm::vec3(0.0f, 0.0f, 0.0f);
}

// ********************************************************************************
/// <summary>
/// Getter for the Camera
/// </summary>
/// <returns></returns>
// ********************************************************************************
Camera* FreeCam::GetCamera()
{
	return m_Camera;
}

// ********************************************************************************
/// <summary>
/// This runs every frame
/// </summary>
/// <param name="_fDeltaTime">Time since last frame</param>
// ********************************************************************************
void FreeCam::Update(float _fDeltaTime)
{
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
	if (glfwGetKey(m_Window, GLFW_KEY_Q) == GLFW_PRESS && !(glfwGetKey(m_Window, GLFW_KEY_E) == GLFW_PRESS))
	{
		direction += glm::vec3(0.0f, -1.0f, 0.0f);
		keypressed = true;
	}
	if (glfwGetKey(m_Window, GLFW_KEY_E) == GLFW_PRESS && !(glfwGetKey(m_Window, GLFW_KEY_Q) == GLFW_PRESS))
	{
		direction += glm::vec3(0.0f, 1.0f, 0.0f);
		keypressed = true;
	}

	//Get cursor position
	double xpos, ypos;
	glfwGetCursorPos(m_Window, &xpos, &ypos);

	//Get change in cursor position since last frame
	glm::vec2 CursorDelta = glm::vec2(xpos, ypos) - m_lastcursorpos;

	//Invert
	CursorDelta.x *= -1;

	//Rotate the camera using cursor delta
	m_cameraviewangle += CursorDelta * (_fDeltaTime / 2);

	//Clamp camera pitch
	if (m_cameraviewangle.y > -18.0f)
	{
		m_cameraviewangle.y = -18.0f;
	}
	if (m_cameraviewangle.y < -20.0f)
	{
		m_cameraviewangle.y = -20.0f;
	}

	//Set forward and right vector of camera based on rotation
	m_Camera->m_cameraLookDir = glm::vec3(cos(m_cameraviewangle.y) * sin(m_cameraviewangle.x), -sin(m_cameraviewangle.y), cos(m_cameraviewangle.y) * cos(m_cameraviewangle.x));
	m_camerarightvector = glm::vec3(cos(m_cameraviewangle.x), 0.0f, -sin(m_cameraviewangle.x));

	//Move camera
	glm::vec3 Movement = (m_Camera->m_cameraLookDir * -direction.z) + (m_camerarightvector * -direction.x) + (glm::vec3(0.0f, 1.0f, 0.0f) * direction.y);

	//If a key is pressed, normalise the direction vector and multiply by deltatime for movement
	if (keypressed)
	{
		m_Camera->m_cameraPos += glm::normalize(Movement) * _fDeltaTime * 10.0f;
	}

	//Update variable to be used next frame
	m_lastcursorpos = glm::vec2(xpos, ypos);
}
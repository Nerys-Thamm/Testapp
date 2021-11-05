#include "SceneManager.h"

// ********************************************************************************
/// <summary>
/// Constructor
/// </summary>
/// <param name="_window">The window to manage</param>
/// <returns></returns>
// ********************************************************************************
SceneManager::SceneManager(GLFWwindow* _window)
{
	glfwSetKeyCallback(_window, OnKeyPress);
	m_window = _window;
}

// ********************************************************************************
/// <summary>
/// Gets the text input buffer
/// </summary>
/// <returns></returns>
// ********************************************************************************
std::string SceneManager::GetTextInputBuffer()
{
	return m_textinputbuffer;
}

// ********************************************************************************
/// <summary>
/// Converts a coordinate from screen space to orthographic worldspace
/// </summary>
/// <param name="_mousePos"></param>
/// <returns></returns>
// ********************************************************************************
glm::vec2 SceneManager::ScreenToWorldSpace(glm::vec2 _mousePos)
{
	return glm::vec2(_mousePos.x + (cfWINDOW_WIDTH() / 2), _mousePos.y + (cfWINDOW_HEIGHT() / 2));
}

Skybox* SceneManager::m_skybox = nullptr;

void SceneManager::SetCurrentSkybox(Skybox* _skybox)
{
	m_skybox = _skybox;
}

Skybox* SceneManager::GetCurrentSkybox()
{
	return m_skybox;
}

GLFWwindow* SceneManager::GetWindow()
{
	return m_window;
}

void SceneManager::SetMainCamera(Camera* _camera)
{
	m_mainCamera = _camera;
}

Camera* SceneManager::GetMainCamera()
{
	return m_mainCamera;
}

bool SceneManager::m_isWireframe = false;
bool SceneManager::m_isCursorVisible = true;
bool SceneManager::m_isReadingTextInput = false;
std::string SceneManager::m_textinputbuffer = "";

// ********************************************************************************
/// <summary>
/// Event Callback for Key Press
/// </summary>
/// <param name="InputWindow"></param>
/// <param name="Key"></param>
/// <param name="ScanCode"></param>
/// <param name="Action"></param>
/// <param name="Mods"></param>
// ********************************************************************************
void SceneManager::OnKeyPress(GLFWwindow* InputWindow, int Key, int ScanCode, int Action, int Mods)
{
	//Close window on Escape press
	if (Key == GLFW_KEY_ESCAPE && Action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(InputWindow, true);
	}

	//Toggle Wireframe mode with Tab
	if (Key == GLFW_KEY_TAB && Action == GLFW_PRESS)
	{
		SceneManager::m_isWireframe = !SceneManager::m_isWireframe;
		if (SceneManager::m_isWireframe)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}
		else
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}
	}

	//Toggle Text Input with Enter
	if (Key == GLFW_KEY_ENTER && Action == GLFW_PRESS)
	{
		SceneManager::m_isReadingTextInput = !SceneManager::m_isReadingTextInput;
		m_isReadingTextInput ? glfwSetCharCallback(InputWindow, TextInput) : glfwSetCharCallback(InputWindow, 0);
	}

	//Remove a character from the text input using Backspace
	if (Key == GLFW_KEY_BACKSPACE && (Action == GLFW_PRESS || Action == GLFW_REPEAT) && SceneManager::m_isReadingTextInput && !m_textinputbuffer.empty())
	{
		m_textinputbuffer.pop_back();
	}

	//Toggle Cursor visibility with Control
	if (Key == GLFW_KEY_LEFT_CONTROL && Action == GLFW_PRESS)
	{
		SceneManager::m_isCursorVisible = !SceneManager::m_isCursorVisible;
		glfwSetInputMode(InputWindow, GLFW_CURSOR, SceneManager::m_isCursorVisible ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_HIDDEN);
	}

	
}

// ********************************************************************************
/// <summary>
/// Text Input Callback
/// </summary>
/// <param name="InputWindow"></param>
/// <param name="CodePoint"></param>
// ********************************************************************************
void SceneManager::TextInput(GLFWwindow* InputWindow, unsigned int CodePoint)
{
	m_textinputbuffer += CodePoint;
}

// ********************************************************************************
/// <summary>
/// Is called every frame
/// </summary>
/// <param name="_fDeltaTime"></param>
// ********************************************************************************
void SceneManager::Update(float _fDeltaTime)
{
}
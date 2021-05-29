#include "SceneManager.h"

SceneManager::SceneManager(GLFWwindow* _window)
{
	glfwSetKeyCallback(_window, OnKeyPress);
	m_window = _window;
}

std::string SceneManager::GetTextInputBuffer()
{
	return m_textinputbuffer;
}

glm::vec2 SceneManager::ScreenToWorldSpace(glm::vec2 _mousePos)
{
	return glm::vec2(_mousePos.x + (cfWINDOW_WIDTH()/2), _mousePos.y + (cfWINDOW_HEIGHT()/2) );
}

bool SceneManager::m_isWireframe = false;
bool SceneManager::m_isCursorVisible = true;
bool SceneManager::m_isReadingTextInput = false;
std::string SceneManager::m_textinputbuffer = "";

void SceneManager::OnKeyPress(GLFWwindow* InputWindow, int Key, int ScanCode, int Action, int Mods)
{
	if (Key == GLFW_KEY_ESCAPE && Action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(InputWindow, true);
	}
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
	if (Key == GLFW_KEY_ENTER && Action == GLFW_PRESS)
	{
		SceneManager::m_isReadingTextInput = !SceneManager::m_isReadingTextInput;
		m_isReadingTextInput ? glfwSetCharCallback(InputWindow, TextInput) : glfwSetCharCallback(InputWindow, 0);
	}
	if (Key == GLFW_KEY_BACKSPACE && (Action == GLFW_PRESS || Action == GLFW_REPEAT) && SceneManager::m_isReadingTextInput && !m_textinputbuffer.empty())
	{
		m_textinputbuffer.pop_back();
	}
	if (Key == GLFW_KEY_LEFT_CONTROL && Action == GLFW_PRESS)
	{
		SceneManager::m_isCursorVisible = !SceneManager::m_isCursorVisible;
		glfwSetInputMode(InputWindow, GLFW_CURSOR, SceneManager::m_isCursorVisible ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_HIDDEN);
	}
	if (Key == GLFW_KEY_1 && Action == GLFW_PRESS)
	{
		Audiosystem::GetInstance()->PlaySound("Bruh");
	}
	if (Key == GLFW_KEY_2 && Action == GLFW_PRESS)
	{
		Audiosystem::GetInstance()->PlaySound("Airhorn");
	}

}

void SceneManager::TextInput(GLFWwindow* InputWindow, unsigned int CodePoint)
{
	m_textinputbuffer += CodePoint;
}

void SceneManager::Update(float _fDeltaTime)
{
	
}

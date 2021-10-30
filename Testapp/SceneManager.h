// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2021 Media Design School
//
// File Name   : SceneManager.h
// Description : Header file for scenemanager class
// Author      : Nerys Thamm
// Mail        : nerys.thamm@mds.ac.nz

#pragma once
#include "GameObject.h"
#include <glm.hpp>
#include "Config.h"
#include "Audiosystem.h"
#include "Skybox.h"
class SceneManager :
	public CGameObject
{
public:
	SceneManager(GLFWwindow* _window);
	static std::string GetTextInputBuffer();
	static glm::vec2 ScreenToWorldSpace(glm::vec2 _mousePos);
	static void SetCurrentSkybox(Skybox* _skybox);
	static Skybox* GetCurrentSkybox();
	GLFWwindow* GetWindow();
	void SetMainCamera(Camera* _camera);
	Camera* GetMainCamera();

private:
	GLFWwindow* m_window = nullptr;
	Camera* m_mainCamera;
	virtual void Update(float _fDeltaTime);
	static Skybox* m_skybox;
	static bool m_isWireframe;
	static bool m_isReadingTextInput;
	static bool m_isCursorVisible;
	static std::string m_textinputbuffer;
	static void OnKeyPress(GLFWwindow* InputWindow, int Key, int ScanCode, int Action, int Mods);
	static void TextInput(GLFWwindow* InputWindow, unsigned int CodePoint);
};

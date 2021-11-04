#pragma once
#include "Entity.h"
class CharacterMotor :
    public IBehaviour
{
public:
	CharacterMotor(CEntity& _parent);
	void SetWindow(GLFWwindow* _window) {
		m_Window = _window; 
	}
private:
	GLFWwindow* m_Window;
	void OnAwake();
	void OnUpdate(float _fDeltaTime);
	void OnFixedUpdate();
	void OnLateUpdate(float _fDeltaTime);
};


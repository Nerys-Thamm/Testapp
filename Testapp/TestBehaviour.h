#pragma once
#include "Entity.h"
class TestBehaviour :
    public IBehaviour
{
public:
	TestBehaviour(CEntity& _parent);
private:
	void OnAwake();
	void OnUpdate(float _fDeltaTime);
	void OnFixedUpdate();
	void OnLateUpdate(float _fDeltaTime);
};


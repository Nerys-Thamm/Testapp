#pragma once
#include "Entity.h"
#include "Camera.h"
class CameraHolder :
    public IBehaviour
{
public:
	CameraHolder(CEntity& _parent);
	void SetCamera(Camera* _cam) { m_camera = _cam; }
	Camera* GetCamera() { return m_camera; }
private:
	Camera* m_camera;
	void OnAwake();
	void OnUpdate(float _fDeltaTime);
	void OnFixedUpdate();
	void OnLateUpdate(float _fDeltaTime);
};


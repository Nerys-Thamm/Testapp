#pragma once
#include "GameObject.h"
#include "Lighting.h"
#include "Renderable3D.h"
#include "Camera.h"
#include "ShaderLoader.h"
class PointLightObj :
    public CGameObject
{
public:
    PointLightObj(PointLight* _light, glm::vec3 _position, glm::vec3 _color, float _ambientstrength, float _specularstrength, float _attenuationconstant, float _attenuationlinear, float _attenuationexponent);
	
    void Render(Camera* _camera);
    void SetPosition(glm::vec3 _pos);
    glm::vec3 GetPosition();
private:
    Renderable3D* m_Renderable;
    PointLight* m_Light;
    GLuint m_Shader;
    glm::vec3 m_Position;
    virtual void Update(float _fDeltaTime);
};


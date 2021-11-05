// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
// 
// (c) 2021 Media Design School
//
// File Name   : ClothRenderer.h
// Description : Header file
// Author      : Nerys Thamm
// Mail        : nerys.thamm@mds.ac.nz
#pragma once
#include "Entity.h"
#include "Cloth.h"
#include "Mesh3D.h"
class ClothRenderer :
    public IBehaviour
{
public:
    ClothRenderer(CEntity& _parent);
    void Render(Camera* _camera);
    void SetCloth(Cloth* _cloth) { m_cloth = _cloth; }
    Cloth* GetCloth() { return m_cloth; }
    void SetMaterial(Material* _material) { m_material = _material; }
    void SetShader(GLuint _shader) { m_shader = _shader; }
    void SetTexture(GLuint _texture) { m_texture = _texture; }
private:
    void OnUpdate(float _fDeltaTime);
    void OnFixedUpdate();
    Cloth* m_cloth;
    std::vector<std::unique_ptr<Tri3D>> m_tris;
    Material* m_material;
    GLuint m_shader;
    GLuint m_texture;
};


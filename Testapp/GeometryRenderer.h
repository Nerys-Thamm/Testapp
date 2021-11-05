// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
// 
// (c) 2021 Media Design School
//
// File Name   : GeometryRenderer.h
// Description : Header file
// Author      : Nerys Thamm
// Mail        : nerys.thamm@mds.ac.nz
#pragma once
#include "Entity.h"
#include "Camera.h"
#include "Lighting.h"
class GeometryRenderer :
    public IBehaviour
{
public:
    GeometryRenderer(CEntity& _parent);
    void Render(Camera* _camera);
    void SetMaterial(Material* _material) { m_material = _material; }
    void SetShader(GLuint _shader) { m_shader = _shader; }
    void SetTexture(GLuint _texture) { m_texture = _texture; }
private:
    Material* m_material;
    GLuint m_shader;
    GLuint m_texture;
    GLuint m_vao;
    GLuint m_vbo;
};


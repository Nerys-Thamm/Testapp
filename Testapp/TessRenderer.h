// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
// 
// (c) 2021 Media Design School
//
// File Name   : TessRenderer.h
// Description : Header file
// Author      : Nerys Thamm
// Mail        : nerys.thamm@mds.ac.nz
#pragma once
#include "Entity.h""
#include "Camera.h"
#include "Lighting.h"
class TessRenderer :
    public IBehaviour
{
public:
    TessRenderer(CEntity& _parent);
    void Render(Camera* _camera);
    void SetMaterial(Material* _material) { m_material = _material; }
    void SetShader(GLuint _shader) { m_shader = _shader; }
    void SetTexture(GLuint _texture) { m_texture = _texture; }
    void SetUseLOD(bool _val) { m_useLOD = _val; }
private:
    int innerLOD[10] = { 48, 24 , 18, 14, 10, 8, 6, 4, 3, 2 };
    int outerLOD[10] = { 64, 48 , 24, 10, 8, 5, 3, 3, 2, 1 };
    bool m_useLOD = false;
    Material* m_material;
    GLuint m_shader;
    GLuint m_texture;
    GLuint m_vao;
    GLuint m_vbo;
};


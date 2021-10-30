#pragma once
#include "Entity.h"
#include "Mesh3D.h"
#include <vector>
#define _USE_MATH_DEFINES
#include <cmath>
#include <math.h>
#include "Lighting.h"
#include "SceneManager.h"
class MeshRenderer :
    public IBehaviour
{
public :
    MeshRenderer(CEntity* _parent);
    void Render();
    void SetMesh(Mesh3D* _mesh) { m_mesh = _mesh; }
    void SetMaterial(Material* _material) { m_material = _material; }
    void SetShader(GLuint _shader) { m_shader = _shader; }
private:
    Mesh3D* m_mesh;
    Material* m_material;
    GLuint m_shader;
};


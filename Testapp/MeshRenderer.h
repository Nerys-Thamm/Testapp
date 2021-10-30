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
    void Render(Camera* _camera);
    void SetMesh(Mesh3D* _mesh) { m_mesh = _mesh; }
    void SetMaterial(Material* _material) { m_material = _material; }
    void SetShader(GLuint _shader) { m_shader = _shader; }
    void SetTexture(GLuint _texture) { m_texture = _texture; }
private:
    Mesh3D* m_mesh;
    Material* m_material;
    GLuint m_shader;
    GLuint m_texture;
};


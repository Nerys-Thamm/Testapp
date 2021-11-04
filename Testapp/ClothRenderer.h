#pragma once
#include "Entity.h"
#include "Cloth.h"
#include "Mesh3D.h"
class ClothRenderer :
    public IBehaviour
{
public:
    ClothRenderer(CEntity& _parent);
    ~ClothRenderer();
    void Render(Camera* _camera);
    void SetCloth(Cloth* _cloth);
    Cloth* GetCloth() { return m_cloth; }
    void SetMaterial(Material* _material) { m_material = _material; }
    void SetShader(GLuint _shader) { m_shader = _shader; }
    void SetTexture(GLuint _texture) { m_texture = _texture; }
private:
    void OnUpdate(float _fDeltaTime);
    void OnFixedUpdate();
    Cloth* m_cloth;
    std::vector<std::unique_ptr<Tri3D>> m_tris;
    std::vector<int> m_indices;
    Material* m_material;
    GLuint m_shader;
    GLuint m_texture;
};


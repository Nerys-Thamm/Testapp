#include "Cloth.h"

void ClothParticle::ApplyForce(glm::vec3 _forceVector)
{
    m_accel += _forceVector / m_particleMass;
}

void ClothParticle::Update(float _fDeltaTime)
{
    if (!m_dynamic) return;
    glm::vec3 currPos = m_pos;
    m_pos = m_pos + (m_pos - m_lastPos) * (1.0f - 0.5f) + m_accel * _fDeltaTime;
    m_lastPos = currPos;
    Accel({ 0.0f,0.0f,0.0f });
}

void ClothParticleConstraint::Constrain()
{
    glm::vec3 AB = m_secondParticle->Pos() - m_firstParticle->Pos();
    glm::vec3 halfCorrection = (AB * (1.0f - m_desiredSeperation / AB.length())) * 0.5f;
    m_firstParticle->Pos(m_firstParticle->Pos() + halfCorrection);
    m_secondParticle->Pos(m_secondParticle->Pos() - halfCorrection);
}

Cloth::Cloth(glm::vec2 _scale, glm::ivec2 _density)
{
}

void Cloth::Update(float _fDeltaTime)
{
}

void Cloth::AddForce(glm::vec3 _force)
{
}

void Cloth::AddWind(glm::vec3 _force)
{
}

glm::vec3 Cloth::GetTriNormal(ClothParticle* _a, ClothParticle* _b, ClothParticle* _c)
{
    return glm::vec3();
}

void Cloth::ApplyWindForce(ClothParticle* _a, ClothParticle* _b, ClothParticle* _c, glm::vec3 _force)
{
}

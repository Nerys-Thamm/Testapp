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

Cloth::Cloth(glm::vec2 _scale, glm::ivec2 _density) : m_particleDensity(_density)
{
    
    //Create a Grid of cloth particles to turn into a cloth
    for (int x = 0; x < m_particleDensity.x; x++)
    {
        for (int y = 0; y < m_particleDensity.y; y++)
        {
            m_particles.push_back(ClothParticle({ _scale.x * (x / (float)m_particleDensity.x), -_scale.y * (y / (float)m_particleDensity.y), 0.0f }));
        }
    }

    //Add Structural Spring Constraints
    for (int x = 0; x < m_particleDensity.x; x++)
    {
        for (int y = 0; y < m_particleDensity.y; y++)
        {
            if (x < (m_particleDensity.x - 1)) m_constraints.push_back(ClothParticleConstraint(GetParticleAtIndex(x, y), GetParticleAtIndex(x + 1, y)));
            if (y < (m_particleDensity.y - 1)) m_constraints.push_back(ClothParticleConstraint(GetParticleAtIndex(x, y), GetParticleAtIndex(x, y + 1)));
        }
    }

    //Add Shear Spring Constraints
    for (int x = 0; x < m_particleDensity.x; x++)
    {
        for (int y = 0; y < m_particleDensity.y; y++)
        {
            if (x < (m_particleDensity.x - 1) && (y > 0)) m_constraints.push_back(ClothParticleConstraint(GetParticleAtIndex(x, y), GetParticleAtIndex(x + 1, y-1)));
            if (x < (m_particleDensity.x - 1) && (y < (m_particleDensity.y - 1))) m_constraints.push_back(ClothParticleConstraint(GetParticleAtIndex(x, y), GetParticleAtIndex(x  +1, y + 1)));
        }
    }
    GetParticleAtIndex(0, 0)->Dynamic(false);
    GetParticleAtIndex(m_particleDensity.x - 1, 0)->Dynamic(false);
}

void Cloth::Update(float _fDeltaTime)
{
    //Apply the constraints
    for (int i = 0; i < 5; i++)
    {
        std::for_each(m_constraints.begin(), m_constraints.end(), [&](ClothParticleConstraint _c) { _c.Constrain(); });
    }
    //Update the Particles
    std::for_each(m_particles.begin(), m_particles.end(), [&](ClothParticle _p) { _p.Update(_fDeltaTime); _p.LocalPos(_p.Pos() - m_particles[0].Pos()); });
}

void Cloth::AddForce(glm::vec3 _force)
{
    std::for_each(m_particles.begin(), m_particles.end(), [&](ClothParticle _p) { _p.ApplyForce(_force); });
}

void Cloth::AddWind(glm::vec3 _force)
{
}

glm::vec3 Cloth::GetTriNormal(ClothParticle* _a, ClothParticle* _b, ClothParticle* _c)
{
    return glm::cross((_b->Pos() - _a->Pos()), (_c->Pos() - _a->Pos()));
}

void Cloth::ApplyWindForce(ClothParticle* _a, ClothParticle* _b, ClothParticle* _c, glm::vec3 _force)
{
}

// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
// 
// (c) 2021 Media Design School
//
// File Name   : Cloth.cpp
// Description : Implementation file
// Author      : Nerys Thamm
// Mail        : nerys.thamm@mds.ac.nz
#include "Cloth.h"

void ClothParticle::ApplyForce(glm::vec3 _forceVector)
{
    m_accel += _forceVector / m_particleMass;
}

void ClothParticle::Update(float _fDeltaTime)
{
    if (!m_dynamic) return;
    glm::vec3 currPos = m_pos;
    m_pos += (m_pos - m_lastPos) * (1.0f - 0.5f) + m_accel * _fDeltaTime;
    m_lastPos = currPos;
    m_accel = glm::vec3{ 0.0f, 0.0f, 0.0f };
}

void ClothParticleConstraint::Constrain()
{
    glm::vec3 AB = m_secondParticle->Pos() - m_firstParticle->Pos();
    float len = glm::distance(m_secondParticle->Pos(), m_firstParticle->Pos());
    
    AB = glm::normalize(AB);
    glm::vec3 halfCorrection = (AB * (len - m_desiredSeperation)) * 0.5f;
    if(m_firstParticle->Dynamic()) m_firstParticle->Pos(m_firstParticle->Pos() + halfCorrection);
    if(m_secondParticle->Dynamic()) m_secondParticle->Pos(m_secondParticle->Pos() - halfCorrection);
}

Cloth::Cloth(glm::vec2 _scale, glm::ivec2 _density) : m_particleDensity(_density)
{
    
    //Create a Grid of cloth particles to turn into a cloth
    for (int y = 0; y < m_particleDensity.y; y++)
    {
        for (int x = 0; x < m_particleDensity.x; x++)
        {
            m_particles.push_back(ClothParticle({ x * (_scale.x / (float)m_particleDensity.x), -y * (_scale.y / (float)m_particleDensity.y), 0.0f }));
        }
    }

    //Add Structural Spring Constraints
    for (int y = 0; y < m_particleDensity.y; y++)
    {
        for (int x = 0; x < m_particleDensity.x; x++)
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
    
    
}

void Cloth::FixedUpdate()
{
    //Apply the constraints
    for (int i = 0; i < 1; i++)
    {
        std::for_each(m_constraints.begin(), m_constraints.end(), [&](ClothParticleConstraint& _c) { _c.Constrain(); });
    }
    //Update the Particles
    std::for_each(m_particles.begin(), m_particles.end(), [&](ClothParticle& _p) { _p.Update(1.0f/60.0f); _p.LocalPos(_p.Pos() - m_particles[0].Pos()); });
    
}

void Cloth::AddForce(glm::vec3 _force)
{
    std::for_each(m_particles.begin(), m_particles.end(), [&](ClothParticle& _p) { _p.ApplyForce(_force); });
}

void Cloth::AddWind(glm::vec3 _force)
{
    for (int x = 0; x < m_particleDensity.x - 1; x++)
    {
        for (int y = 0; y < m_particleDensity.y - 1; y++)
        {
            ApplyWindForce(GetParticleAtIndex(x + 1, y), GetParticleAtIndex(x, y), GetParticleAtIndex(x, y + 1), _force);
            ApplyWindForce(GetParticleAtIndex(x + 1, y + 1), GetParticleAtIndex(x + 1, y), GetParticleAtIndex(x, y + 1), _force);
        }
    }
}

glm::vec3 Cloth::GetTriNormal(ClothParticle* _a, ClothParticle* _b, ClothParticle* _c)
{
    return glm::cross((_b->Pos() - _a->Pos()), (_c->Pos() - _a->Pos()));
}

void Cloth::ApplyWindForce(ClothParticle* _a, ClothParticle* _b, ClothParticle* _c, glm::vec3 _force)
{
    glm::vec3 norm = GetTriNormal(_a, _b, _c);
    glm::vec3 d = glm::normalize(norm);
    glm::vec3 force = norm * (glm::dot(d, _force));
    _a->ApplyForce(force);
    _b->ApplyForce(force);
    _c->ApplyForce(force);

}

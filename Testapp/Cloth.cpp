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

// ********************************************************************************
/// <summary>
/// Applies force to the Particle
/// </summary>
/// <param name="_forceVector"></param>
// ********************************************************************************
void ClothParticle::ApplyForce(glm::vec3 _forceVector)
{
    if (!m_dynamic) return;
    m_accel += _forceVector / m_particleMass;
}

// ********************************************************************************
/// <summary>
/// Applies Gravity force to the Particle
/// </summary>
// ********************************************************************************
void ClothParticle::ApplyGravity()
{
    if (!m_dynamic) return;
    m_accel += -glm::vec3(0.0f, (9.8f/2.0f), 0.0f);
}

// ********************************************************************************
/// <summary>
/// Update the position and acceleration of the Particle
/// </summary>
/// <param name="_fDeltaTime"></param>
// ********************************************************************************
void ClothParticle::Update(float _fDeltaTime)
{
    if (!m_dynamic) return;
    glm::vec3 currPos = m_pos;
    m_pos += (m_pos - m_lastPos) * (1.0f - 0.5f) + m_accel * _fDeltaTime;
    m_lastPos = currPos;
    m_accel = glm::vec3{ 0.0f, 0.0f, 0.0f };
}

// ********************************************************************************
/// <summary>
/// Apply the constraint to the particles
/// </summary>
// ********************************************************************************
void ClothParticleConstraint::Constrain()
{
    if (!m_firstParticle->m_isEnabled || !m_secondParticle->m_isEnabled) return;
    
    //Get the particle positions
    glm::vec3 firstPos = m_firstParticle->Pos();
    glm::vec3 secondPos = m_secondParticle->Pos();

    //Find the vector between the particles
    glm::vec3 AB = secondPos - firstPos;

    //Find the distance between the particles
    float len = glm::distance(secondPos, firstPos);

    //Get the normalised direction vector
    AB = glm::normalize(AB);
    
    //Get the correction vector
    glm::vec3 halfCorrection = (AB * (len - m_desiredSeperation)) * (0.5f*m_stiffness);

    //Lock this method to protect thread safety when multithreading the simulation
    std::lock_guard<std::mutex> guard(*cloth_constraint_mutex);

    //Apply the Constraint
    if (m_firstParticle->Dynamic() && m_firstParticle->Pos().y < -50.0f) m_firstParticle->Pos(glm::vec3((firstPos + halfCorrection).x, -50.0f, (firstPos + halfCorrection).z));
    else if (m_firstParticle->Dynamic()) m_firstParticle->Pos(firstPos + halfCorrection);
    if (m_secondParticle->Dynamic() && m_secondParticle->Pos().y < -50.0f) m_secondParticle->Pos(glm::vec3((secondPos - halfCorrection).x, -50.0f, (secondPos - halfCorrection).z));
    else if(m_secondParticle->Dynamic()) m_secondParticle->Pos(secondPos - halfCorrection);
   
}

// ********************************************************************************
/// <summary>
/// Apply a Sphere collision check TODO: MAKE THIS WORK
/// </summary>
/// <param name="_origin"></param>
/// <param name="_radius"></param>
// ********************************************************************************
void Cloth::SphereCollision(glm::vec3 _origin, float _radius)
{
    _origin = _origin - m_worldPos;
    for (int i = 0; i < (int)m_particles.size(); i++)
    {
        if (!m_particles[i].Dynamic())
        {
            continue;
        }
        glm::vec3 particlePos = m_particles[i].Pos();
        float len = glm::distance(_origin, particlePos);
        if (len > _radius * 1.1f) continue;
        glm::vec3 AB = particlePos - _origin;
        AB = glm::normalize(AB);

        m_particles[i].Accel({0.0f, 0.0f, 0.0f});
        m_particles[i].ApplyForce(AB * m_particles[i].Mass() * 1.1f);
        m_particles[i].Pos(_origin + (AB * (_radius * 1.1f)));
    }
}

// Find the particle with the largest dot product with the normal
ClothParticle* Cloth::RaycastParticle(glm::vec3 _origin, glm::vec3 _direction, float _tolerance)
{
    _origin = _origin - m_worldPos;
    float maxDot = 0.0f;
    ClothParticle* maxParticle = nullptr;
    for (int i = 0; i < (int)m_particles.size(); i++)
    {
        
        glm::vec3 particlePos = m_particles[i].Pos();
        glm::vec3 AB = particlePos - _origin;
        
        
        glm::vec3 normal = glm::normalize(AB);
        float dot = glm::dot(normal, _direction);
        if (dot > maxDot)
        {
            maxDot = dot;
            maxParticle = &m_particles[i];
        }
    }
    if (maxDot > _tolerance)
    {
        return maxParticle;
    }
    else
    {
        return nullptr;
    }
    
}

// ********************************************************************************
/// <summary>
/// Sets the distance of the pegs the cloth is hung by
/// </summary>
/// <param name="_distance"></param>
// ********************************************************************************
void Cloth::SetPegDistance(float _distance)
{
    for (int i = 0; i < (int)m_pegXIndexes.size(); i++)
    {
        m_particles[m_pegXIndexes[i]].Pos(glm::vec3((_distance/m_numOfPegs) * i, 0.0f, (i % 2 == 0 ? -1.0f : 1.0f)));
    }
}

// ********************************************************************************
/// <summary>
/// Constructor
/// </summary>
/// <param name="_stiffness"></param>
/// <returns></returns>
// ********************************************************************************
Cloth::Cloth(glm::vec2 _scale, glm::ivec2 _density, float _mass, int _pegs, float& _stiffness) : m_particleDensity(_density), m_numOfPegs(_pegs), m_pegDistance(_scale.x / _pegs), m_stiffness(_stiffness)
{
    int totalcount = _density.x * _density.y;
    //Create a Grid of cloth particles to turn into a cloth
    for (int y = 0; y < m_particleDensity.y; y++)
    {
        for (int x = 0; x < m_particleDensity.x; x++)
        {
            m_particles.push_back(ClothParticle({ x * (_scale.x / (float)m_particleDensity.x), -y * (_scale.y / (float)m_particleDensity.y), 0.0f }, _mass/totalcount));
        }
    }

    //Add Structural Spring Constraints
    for (int y = 0; y < m_particleDensity.y; y++)
    {
        for (int x = 0; x < m_particleDensity.x; x++)
        {
            if (x < (m_particleDensity.x - 1)) m_structuralConstraints.push_back(ClothParticleConstraint(GetParticleAtIndex(x, y), GetParticleAtIndex(x + 1, y), m_stiffness));
            if (y < (m_particleDensity.y - 1)) m_structuralConstraints.push_back(ClothParticleConstraint(GetParticleAtIndex(x, y), GetParticleAtIndex(x, y + 1), m_stiffness));
        }
    }

    //Add Shear Spring Constraints
    for (int x = 0; x < m_particleDensity.x; x++)
    {
        for (int y = 0; y < m_particleDensity.y; y++)
        {
            if (x < (m_particleDensity.x - 1) && (y > 0)) m_shearConstraints.push_back(ClothParticleConstraint(GetParticleAtIndex(x, y), GetParticleAtIndex(x + 1, y-1), m_stiffness));
            if (x < (m_particleDensity.x - 1) && (y < (m_particleDensity.y - 1))) m_shearConstraints.push_back(ClothParticleConstraint(GetParticleAtIndex(x, y), GetParticleAtIndex(x  +1, y + 1), m_stiffness));
        }
    }

    //Add Bend Spring Constraints
    for (int x = 0; x < m_particleDensity.x; x++)
    {
        for (int y = 0; y < m_particleDensity.y; y++)
        {
            if (x < (m_particleDensity.x - 2)) m_bendConstraints.push_back(ClothParticleConstraint(GetParticleAtIndex(x, y), GetParticleAtIndex(x + 2, y), m_stiffness));
            if (y < (m_particleDensity.y - 2)) m_bendConstraints.push_back(ClothParticleConstraint(GetParticleAtIndex(x, y), GetParticleAtIndex(x, y + 2), m_stiffness));

            if (x < (m_particleDensity.x - 2) && (y < m_particleDensity.y - 2))
            {
                m_bendConstraints.push_back(ClothParticleConstraint(GetParticleAtIndex(x, y), GetParticleAtIndex(x + 2, y + 2), m_stiffness));
                m_bendConstraints.push_back(ClothParticleConstraint(GetParticleAtIndex(x + 2, y), GetParticleAtIndex(x, y + 2), m_stiffness));
            }
        }
    }


    //Make the Pegs hang up the cloth
    if (m_numOfPegs > 0 && m_numOfPegs <= m_particleDensity.x)
    {
        
        for (int i = 0; i < m_numOfPegs; i++)
        {
            int index = (int)floorf(i * (m_particleDensity.x / (float)m_numOfPegs));
            GetParticleAtIndex(index, 0)->Dynamic(false);
            m_pegXIndexes.push_back(index);
        }
        
    }
}


// ********************************************************************************
/// <summary>
/// Called every frame
/// </summary>
/// <param name="_fDeltaTime"></param>
// ********************************************************************************
void Cloth::Update(float _fDeltaTime)
{
    
    
}

// ********************************************************************************
/// <summary>
/// Called at a fixed timestep
/// </summary>
// ********************************************************************************
void Cloth::FixedUpdate()
{
   
    //Update the Particles
    ApplyGravity();
    std::for_each(m_particles.begin(), m_particles.end(), [&](ClothParticle& _p) { _p.Update(1.0f/60.0f); _p.LocalPos(_p.Pos() - m_particles[0].Pos()); });
    //Apply the constraints
    
    for (int i = 0; i < 2; i++)
    {
        for (int i = 0; i < (int)m_structuralConstraints.size(); i++)
        {
            m_structuralConstraints[i].Constrain();
        }
        for (int i = 0; i < (int)m_shearConstraints.size(); i++)
        {
            m_shearConstraints[i].Constrain();
        }
        for (int i = 0; i < (int)m_bendConstraints.size(); i++)
        {
            m_bendConstraints[i].Constrain();
        }
    }
  
}

// ********************************************************************************
/// <summary>
/// Add a force to all particles in the cloth
/// </summary>
/// <param name="_force"></param>
// ********************************************************************************
void Cloth::AddForce(glm::vec3 _force)
{
    std::for_each(m_particles.begin(), m_particles.end(), [&](ClothParticle& _p) { _p.ApplyForce(_force); });
}

// ********************************************************************************
/// <summary>
/// Apply gravity to the cloth
/// </summary>
// ********************************************************************************
void Cloth::ApplyGravity()
{
    std::for_each(m_particles.begin(), m_particles.end(), [&](ClothParticle& _p) { _p.ApplyGravity(); });
}

// ********************************************************************************
/// <summary>
/// Apply a wind force to the cloth
/// </summary>
/// <param name="_force"></param>
// ********************************************************************************
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

// ********************************************************************************
/// <summary>
/// Get the normal of a triangle defined by three Particles
/// </summary>
/// <param name="_a"></param>
/// <param name="_b"></param>
/// <param name="_c"></param>
/// <returns></returns>
// ********************************************************************************
glm::vec3 Cloth::GetTriNormal(ClothParticle* _a, ClothParticle* _b, ClothParticle* _c)
{
    return glm::cross((_b->Pos() - _a->Pos()), (_c->Pos() - _a->Pos()));
}

// ********************************************************************************
/// <summary>
/// Get the normal of a triangle defined by three Indices
/// </summary>
/// <param name="_a"></param>
/// <param name="_b"></param>
/// <param name="_c"></param>
/// <returns></returns>
// ********************************************************************************
glm::vec3 Cloth::GetTriNormal(int _a, int _b, int _c)
{
    return GetTriNormal(&m_particles[_a], &m_particles[_b], &m_particles[_c]);
}

// ********************************************************************************
/// <summary>
/// Make all particles dynamic so it falls
/// </summary>
// ********************************************************************************
void Cloth::DropCloth()
{
    for (int i = 0; i < (int)m_particles.size(); i++)
    {
        m_particles[i].Dynamic(true);
    }
}

Cloth::~Cloth()
{

}

// ********************************************************************************
/// <summary>
/// Apply a force proportional to the dot product of the triangle and force vector
/// </summary>
/// <param name="_a"></param>
/// <param name="_b"></param>
/// <param name="_c"></param>
/// <param name="_force"></param>
// ********************************************************************************
void Cloth::ApplyWindForce(ClothParticle* _a, ClothParticle* _b, ClothParticle* _c, glm::vec3 _force)
{
    glm::vec3 norm = GetTriNormal(_a, _b, _c);
    glm::vec3 d = glm::normalize(norm);
    glm::vec3 force = norm * (glm::dot(d, _force));
    _a->ApplyForce(force);
    _b->ApplyForce(force);
    _c->ApplyForce(force);

}

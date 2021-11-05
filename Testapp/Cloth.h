// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
// 
// (c) 2021 Media Design School
//
// File Name   : Cloth.h
// Description : Header file
// Author      : Nerys Thamm
// Mail        : nerys.thamm@mds.ac.nz
#pragma once
#include "GameObject.h"
#include <glew.h>
#include <glfw3.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <vector>
#define _USE_MATH_DEFINES
#include <cmath>
#include <math.h>
#include <iostream>
#include <algorithm>
#include <string>
#include <mutex>


class ClothParticle
{
public: //Public Vars

private: //Private Vars
    glm::vec3 m_pos;
    glm::vec3 m_localPos;
    glm::vec3 m_lastPos;
    glm::vec3 m_accel;
    float m_particleMass;
    bool m_dynamic;

public: //Public Methods
    ClothParticle(glm::vec3 _pos) : m_pos(_pos), m_lastPos(_pos), m_accel({ 0.0f,0.0f,0.0f }), m_particleMass(0.1f), m_dynamic(true) {}
    ClothParticle(glm::vec3 _pos, float _mass) : m_pos(_pos), m_lastPos(_pos), m_accel({ 0.0f,0.0f,0.0f }), m_particleMass(_mass), m_dynamic(true) {}

    void ApplyForce(glm::vec3 _forceVector);
    void ApplyGravity();
    void SetMass(float _mass) { m_particleMass = _mass; }
    void Update(float _fDeltaTime);

    //Getters and Setters
    glm::vec3& Pos() { return m_pos; }
    void Pos(glm::vec3 _newPos) { m_pos = _newPos; }
    glm::vec3& LocalPos() { return m_localPos; }
    void LocalPos(glm::vec3 _newPos) { m_localPos = _newPos; }
    glm::vec3& Accel() { return m_accel; }
    void Accel(glm::vec3 _newAccel) { m_accel = _newAccel; }
    void Dynamic(bool _isDynamic) { m_dynamic = _isDynamic; }
    bool Dynamic() { return m_dynamic; }
private: //Private Methods
};

class ClothParticleConstraint
{
public: //Public Vars
    ClothParticle* m_firstParticle, * m_secondParticle;
    std::mutex* cloth_constraint_mutex;
private: //Private Vars
    float m_desiredSeperation;
    float m_maxStretch;
    float& m_stiffness;
public: //Public Methods
    ClothParticleConstraint(ClothParticle* _firstParticle, ClothParticle* _secondParticle, float& _stiffness) :
        m_firstParticle(_firstParticle),
        m_secondParticle(_secondParticle),
        m_desiredSeperation(glm::distance(_firstParticle->Pos(), _secondParticle->Pos())),
        m_maxStretch(3.0f),
        m_stiffness(_stiffness),
        cloth_constraint_mutex(new std::mutex())
    {}

    void Constrain();
    
    
private: //Private Methods
};


class Cloth 
{
public: //Public Vars
    int GetSize() { return m_particles.size(); }
    glm::ivec2 m_particleDensity;
    glm::vec3 GetParticlePositionAtIndex(int _index) { return m_particles[_index].Pos(); }
    void SetPegDistance(float _distance);
private: //Private Vars
    
    std::vector<ClothParticle> m_particles;
    std::vector<ClothParticleConstraint> m_structuralConstraints;
    std::vector<ClothParticleConstraint> m_shearConstraints;
    std::vector<ClothParticleConstraint> m_bendConstraints;
    std::vector<int> m_pegXIndexes;
    int m_numOfPegs;
    float m_pegDistance;
    float& m_stiffness;
public: //Public Methods
    Cloth(glm::vec2 _scale, glm::ivec2 _density, float _mass, int _pegs, float& _stiffness);
    void Update(float _fDeltaTime);
    void FixedUpdate();
    void AddForce(glm::vec3 _force);
    void ApplyGravity();
    void AddWind(glm::vec3 _force);
    void SphereCollision(glm::vec3 _origin, float _radius);
    glm::vec3 GetTriNormal(ClothParticle* _a, ClothParticle* _b, ClothParticle* _c);
    glm::vec3 GetTriNormal(int _a, int _b, int _c);
    void DropCloth();
    ~Cloth();

private: //Private Methods
    ClothParticle* GetParticleAtIndex(int _X, int _Y) { return &m_particles[_Y * m_particleDensity.x + _X]; }
    
    void ApplyWindForce(ClothParticle* _a, ClothParticle* _b, ClothParticle* _c, glm::vec3 _force);
};






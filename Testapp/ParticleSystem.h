// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
// 
// (c) 2021 Media Design School
//
// File Name   : ParticleSystem.h
// Description : Header file
// Author      : Nerys Thamm
// Mail        : nerys.thamm@mds.ac.nz

#pragma once
#include "Entity.h"
#include "Camera.h"
#include "ShaderLoader.h"

class Particle
{
public:
	Particle(glm::vec3 _position, glm::vec3 _velocity, glm::vec4 _colour, glm::vec4 _endColour, float _lifetime, float _size, float _endSize, float _gravity);
	

	void Update(float _fDeltaTime);
	void Reset(glm::vec3 _pos);
	glm::vec3 GetPosition() { return m_position; }
	glm::vec4 GetColour() { return glm::mix(m_colour, m_endColour, 1.0f - (m_lifetime / m_initLifetime)); }
	float GetSize() { return glm::mix(m_size, m_endSize, 1.0f - (m_lifetime / m_initLifetime)); }
	bool IsAlive() { return m_alive; }

private:
	glm::vec3 m_initPos;
	float m_initLifetime;
	glm::vec3 m_position;
	glm::vec3 m_velocity;
	glm::vec4 m_colour;
	glm::vec4 m_endColour;
	float m_size;
	float m_endSize;
	float m_lifetime;
	bool m_alive;
	float m_gravity;
	float m_initSpeed;
	

};


class ParticleSystem :
    public IBehaviour
{
public:
	ParticleSystem(CEntity& _parent);
	void SetParticleCount(int _iCount) { m_iParticleCount = _iCount; }
	void SetParticleLifetime(float _fLifetime) { m_fParticleLifetime = _fLifetime; }
	void SetParticleSpeed(float _fSpeed) { m_fParticleSpeed = _fSpeed; }
	void SetParticleSize(float _fSize) { m_fParticleSize = _fSize; }
	void SetParticleColour(glm::vec4 _vColour) { m_vParticleColour = _vColour; }
	void SetParticleTexture(GLuint _iTexture) { m_iParticleTexture = _iTexture; }
	void SetGravity(float _fGravity) { m_fGravity = _fGravity; }
	void SetEmissionRate(float _fRate) { m_fEmissionRate = _fRate; }
	void SetBufferArrayPos(int _index, glm::vec3 _pos);
	void SetBufferArrayColour(int _index, glm::vec4 _colour);
	void SetBufferArrayParticleSize(int _index, float _size);
	void SetParticleEndSize(float _fSize) { m_fParticleEndSize = _fSize; }
	void SetParticleEndColour(glm::vec4 _vColour) { m_vParticleEndColour = _vColour; }
	void Init();
	void Start();
	void Stop();
	void Render(Camera& _camera);

private:
	std::vector<Particle> m_particles;
	std::vector<glm::vec3> m_particlePositions;
	std::vector<GLfloat> m_bufferarray;
	bool m_emmiterActive;
	float m_fParticleLifetime;
	float m_fParticleSpeed;
	float m_fParticleSize;
	float m_fParticleEndSize;
	float m_fEmissionRate;
	glm::vec4 m_vParticleColour;
	glm::vec4 m_vParticleEndColour;
	GLuint m_iParticleTexture;
	int m_iParticleCount;
	float m_fGravity = 0.0f;
	GLuint m_VAO;
	GLuint m_VBO;
	GLuint m_program;
	float cumulativeSpawnTime = 0.0f;
	void OnAwake();
	void OnUpdate(float _fDeltaTime);
	void OnFixedUpdate();
	void OnLateUpdate(float _fDeltaTime);
};




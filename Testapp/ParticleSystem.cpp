// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
// 
// (c) 2021 Media Design School
//
// File Name   : ParticleSystem.cpp
// Description : Implementation file
// Author      : Nerys Thamm
// Mail        : nerys.thamm@mds.ac.nz

#include "ParticleSystem.h"
#define NUM_PARTICLES 128 * 20000

//*********************************
// BEHAVIOR | ParticleSystem 
//*********************************

// ********************************************************************************
/// <summary>
/// Constructor
/// </summary>
/// <param name="_parent"></param>
/// <returns></returns>
// ********************************************************************************
ParticleSystem::ParticleSystem(CEntity& _parent) : IBehaviour(_parent)
{
    m_program = ShaderLoader::CreateProgram(
        {
            ShaderLoader::ShaderFile{"Resources/Shaders/Particle.vert", GL_VERTEX_SHADER}, 
            ShaderLoader::ShaderFile{"Resources/Shaders/Particle.frag", GL_FRAGMENT_SHADER},
            ShaderLoader::ShaderFile{"Resources/Shaders/Particle.geom", GL_GEOMETRY_SHADER}
        });

    //Initialize all member variables
    m_emmiterActive = false;
	m_fParticleLifetime = 0.0f;
	m_fParticleSpeed = 0.0f;
	m_fParticleSize = 0.0f;
    m_fParticleEndSize = 0.0f;
	m_vParticleColour = glm::vec4(0.0f);
    m_vParticleEndColour = glm::vec4(0.0f);
	m_iParticleTexture = 0;
	m_iParticleCount = 0;
	m_fGravity = 0.0f;
}

// ********************************************************************************
/// <summary>
/// Runs before the object is updated for the first time
/// </summary>
// ********************************************************************************
void ParticleSystem::OnAwake()
{
}

// ********************************************************************************
/// <summary>
/// Runs every frame
/// </summary>
/// <param name="_fDeltaTime"></param>
// ********************************************************************************
void ParticleSystem::OnUpdate(float _fDeltaTime)
{
    if( m_emmiterActive && m_particles.size() < m_iParticleCount)
    {
        cumulativeSpawnTime += m_fEmissionRate * _fDeltaTime;
        int genThisFrame = std::floorf(cumulativeSpawnTime);
        cumulativeSpawnTime -= genThisFrame;

        if (genThisFrame > m_iParticleCount - m_particles.size())
        {
            genThisFrame = 0;
        }
        for(int i = 0; i < genThisFrame; i++)
            {
                //Generate random spherical direction vector using trigonometry
                glm::vec3 direction = glm::vec3(
                    glm::cos(glm::radians((float)(rand() % 360))),
                    glm::sin(glm::radians((float)(rand() % 360))),
                    glm::cos(glm::radians((float)(rand() % 360)))
                );
                
                
                m_particles.push_back(Particle(
                    glm::vec3(0.0f, 0.0f, 0.0f), 
                    direction * m_fParticleSpeed,
                    m_vParticleColour, 
                    m_vParticleEndColour, 
                    m_fParticleLifetime, 
                    m_fParticleSize,
                    m_fParticleEndSize,
                    m_fGravity));
                
            }
    }
    for (int i = 0; i < m_particles.size(); i++)
    {
        m_particles[i].Update(_fDeltaTime);
        if (!m_particles[i].IsAlive())
        {
            m_particles[i].Reset(m_entity.m_globalTransform.position);
        }
        m_particlePositions[i] = m_particles[i].GetPosition();
        SetBufferArrayPos(i, m_particlePositions[i]);
        SetBufferArrayColour(i, m_particles[i].GetColour());
        SetBufferArrayParticleSize(i, m_particles[i].GetSize());
        
        
    }
    
}

// ********************************************************************************
/// <summary>
/// Runs every frame, at a fixed interval
/// </summary>
// ********************************************************************************
void ParticleSystem::OnFixedUpdate()
{
    
}

// ********************************************************************************
/// <summary>
/// Runs every frame, after all other updates have completed
/// </summary>
/// <param name="_fDeltaTime"></param>
// ********************************************************************************
void ParticleSystem::OnLateUpdate(float _fDeltaTime)
{
}


// ********************************************************************************
/// <summary>
/// 
/// </summary>
// ********************************************************************************
void ParticleSystem::Start()
{
    

    m_emmiterActive = true;
    
    
}

// ********************************************************************************
/// <summary>
/// 
/// </summary>
// ********************************************************************************
void ParticleSystem::Stop()
{
    m_emmiterActive = false;
    m_particles.clear();
    m_particlePositions.clear();
}

// ********************************************************************************
/// <summary>
/// 
/// </summary>
/// <param name="_camera"></param>
// ********************************************************************************
void ParticleSystem::Render(Camera& _camera)
{
    //Calculate billboard vector
    glm::vec3 cameraPosition = _camera.m_cameraPos;
    glm::vec3 cameraDirection = _camera.m_cameraLookDir;
    glm::vec3 cameraUp = _camera.m_cameraUpDir;

    glm::vec3 vQuad1, vQuad2;
    
    vQuad1 = glm::cross(cameraDirection, cameraUp);
    vQuad1 = glm::normalize(vQuad1);
    vQuad2 = glm::cross(cameraDirection, vQuad1);
    vQuad2 = glm::normalize(vQuad2);

    glUseProgram(m_program);

    glUniform3f(glGetUniformLocation(m_program, "vQuad1"), vQuad1.x, vQuad1.y, vQuad1.z);
    glUniform3f(glGetUniformLocation(m_program, "vQuad2"), vQuad2.x, vQuad2.y, vQuad2.z);

    glUniformMatrix4fv(glGetUniformLocation(m_program, "vp"), 1, GL_FALSE, glm::value_ptr(_camera.GetPV()));

    glActiveTexture(GL_TEXTURE0);
    glUniform1i(glGetUniformLocation(m_program, "Texture"), 0);
    glBindTexture(GL_TEXTURE_2D, m_iParticleTexture);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDepthMask(GL_FALSE);

    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * m_bufferarray.size(), &m_bufferarray[0], GL_STREAM_DRAW);
    glBindVertexArray(m_VAO);
    glDrawArrays(GL_POINTS,0, m_particlePositions.size());

    glBindVertexArray(0);
    glUseProgram(0);

    glDisable(GL_BLEND);
    glDepthMask(GL_TRUE);
}

// ********************************************************************************
/// <summary>
/// 
/// </summary>
// ********************************************************************************
void ParticleSystem::Init()
{
    
    for (int i = 0; i < m_iParticleCount; i++)
    {
        m_particlePositions.push_back(glm::vec3(0.0f, 0.0f, 0.0f));
    }
    m_bufferarray.resize(m_iParticleCount * 8);
    
    glGenVertexArrays(1, &m_VAO);
    glBindVertexArray(m_VAO);

    glGenBuffers(1, &m_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * m_bufferarray.size(), &m_bufferarray[0], GL_STREAM_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, false, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 4, GL_FLOAT, false, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 1, GL_FLOAT, false, 8 * sizeof(GLfloat), (GLvoid*)(7 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

// ********************************************************************************
/// <summary>
/// 
/// </summary>
/// <param name="_index"></param>
/// <param name="_pos"></param>
// ********************************************************************************
void ParticleSystem::SetBufferArrayPos(int _index, glm::vec3 _pos)
{
    int elementOffset = 8 * _index;
    //Set position using offset
    m_bufferarray[elementOffset] = _pos.x;
    m_bufferarray[elementOffset + 1] = _pos.y;
    m_bufferarray[elementOffset + 2] = _pos.z;
}

// ********************************************************************************
/// <summary>
/// 
/// </summary>
/// <param name="_index"></param>
/// <param name="_colour"></param>
// ********************************************************************************
void ParticleSystem::SetBufferArrayColour(int _index, glm::vec4 _colour)
{
    int elementOffset = 8 * _index;
    //Set color using offset
    m_bufferarray[elementOffset + 3] = _colour.r;
    m_bufferarray[elementOffset + 4] = _colour.g;
    m_bufferarray[elementOffset + 5] = _colour.b;
    m_bufferarray[elementOffset + 6] = _colour.a;
}

// ********************************************************************************
/// <summary>
/// 
/// </summary>
/// <param name="_index"></param>
/// <param name="_size"></param>
// ********************************************************************************
void ParticleSystem:: SetBufferArrayParticleSize(int _index, float _size)
{
    int elementOffset = 8 * _index;

    m_bufferarray[elementOffset + 7] = _size;
}


// ********************************************************************************
/// <summary>
/// 
/// </summary>
/// <param name="_position"></param>
/// <param name="_velocity"></param>
/// <param name="_colour"></param>
/// <param name="_endColour"></param>
/// <param name="_lifetime"></param>
/// <param name="_size"></param>
/// <param name="_endSize"></param>
/// <param name="_gravity"></param>
/// <returns></returns>
// ********************************************************************************
Particle::Particle(glm::vec3 _position, glm::vec3 _velocity, glm::vec4 _colour, glm::vec4 _endColour, float _lifetime, float _size, float _endSize, float _gravity)
{
    m_initPos = _position;
    m_position = _position;
    m_velocity = _velocity;
    m_colour = _colour;
    m_endColour = _endColour;
    m_size = _size;
    m_endSize = _endSize;
    m_lifetime = _lifetime;
    m_alive = true;
    m_gravity = _gravity;
    m_initSpeed = glm::length(m_velocity);
    m_initLifetime = _lifetime + glm::cos(glm::radians((float)(rand() % 360))) * 0.01;
}


// ********************************************************************************
/// <summary>
/// 
/// </summary>
/// <param name="_pos"></param>
// ********************************************************************************
void Particle::Reset(glm::vec3 _pos)
{
    //Add randomness to the particle
    
    m_lifetime = m_initLifetime;
    m_alive = true;
    m_velocity = glm::normalize( glm::vec3(
                 glm::cos(glm::radians((float)(rand() % 360))),
                 glm::sin(glm::radians((float)(rand() % 360))),
                 glm::cos(glm::radians((float)(rand() % 360)))
                 )) * m_initSpeed;
    m_position = _pos + m_velocity;
    
}

// ********************************************************************************
/// <summary>
/// 
/// </summary>
/// <param name="_fDeltaTime"></param>
// ********************************************************************************
void Particle::Update(float _fDeltaTime)
{
    m_velocity.y -= m_gravity * _fDeltaTime;
    m_position += m_velocity * _fDeltaTime;
    m_lifetime -= _fDeltaTime;
    
    if(m_lifetime <= 0.0f)
    {
        m_lifetime = 0.0f;
        m_alive = false;
    }
}


// ********************************************************************************
/// <summary>
/// Constructor
/// </summary>
/// <param name="_parent"></param>
/// <returns></returns>
// ********************************************************************************
GPUParticleSystem::GPUParticleSystem(CEntity& _parent) : IBehaviour(_parent)
{
    
    
}

// ********************************************************************************
/// <summary>
/// 
/// </summary>
// ********************************************************************************
void GPUParticleSystem::Init()
{
    m_program = ShaderLoader::CreateProgram(
        {
            ShaderLoader::ShaderFile{"Resources/Shaders/GPUParticle.vert", GL_VERTEX_SHADER}, 
            ShaderLoader::ShaderFile{"Resources/Shaders/GPUParticle.frag", GL_FRAGMENT_SHADER}
        });
    m_computeProgram = ShaderLoader::CreateProgram(
        {
            ShaderLoader::ShaderFile{"Resources/Shaders/Particle.comp", GL_COMPUTE_SHADER}
        });

   



    for (int i = 0; i < NUM_PARTICLES; i++)
    {
        float LO = -300.0f;
        float HI = 300.0f;
        m_initPositions.push_back(glm::vec4(
            LO + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(HI-LO))),
            -200.0f + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (400.0f - -200.0f))),
            LO + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(HI-LO))), 
            (static_cast <float> (rand()) / static_cast <float> (RAND_MAX)) + 0.125f
            )
            );
        
        m_initVelocities.push_back(glm::vec4(
            glm::cos(glm::radians((float)(rand() % 360))) * 0.25f,
            glm::sin(glm::radians((float)(rand() % 360))) * 0.01f,
            glm::cos(glm::radians((float)(rand() % 360))) * 0.25f,
            (static_cast <float> (rand()) / static_cast <float> (RAND_MAX)) + 0.125f
            )
            );

    }
    glGenBuffers(1, &m_posVBO);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_posVBO);
    glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(glm::vec4) * m_initPositions.size(), &m_initPositions[0], GL_DYNAMIC_DRAW);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, m_posVBO);

    glGenBuffers(1, &m_velVBO);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_velVBO);
    glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(glm::vec4) * m_initVelocities.size(), &m_initVelocities[0], GL_DYNAMIC_DRAW);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, m_velVBO);

    glGenBuffers(1, &m_initVelVBO);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_initVelVBO);
    glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(glm::vec4) * m_initVelocities.size(), &m_initVelocities[0], GL_DYNAMIC_DRAW);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, m_initVelVBO);

    glGenVertexArrays(1, &m_spoofVAO);
    glBindVertexArray(m_spoofVAO);

    glBindBuffer(GL_ARRAY_BUFFER, m_posVBO);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    m_initialized = true;

}

// ********************************************************************************
/// <summary>
/// Runs before the object is updated for the first time
/// </summary>
// ********************************************************************************
void GPUParticleSystem::OnAwake()
{
}

// ********************************************************************************
/// <summary>
/// Runs every frame
/// </summary>
/// <param name="_fDeltaTime"></param>
// ********************************************************************************
void GPUParticleSystem::OnUpdate(float _fDeltaTime)
{
   
    
}

// ********************************************************************************
/// <summary>
/// Runs every frame, at a fixed interval
/// </summary>
// ********************************************************************************
void GPUParticleSystem::OnFixedUpdate()
{
    
}

// ********************************************************************************
/// <summary>
/// Runs every frame, after all other updates have completed
/// </summary>
/// <param name="_fDeltaTime"></param>
// ********************************************************************************
void GPUParticleSystem::OnLateUpdate(float _fDeltaTime)
{
}


// ********************************************************************************
/// <summary>
/// 
/// </summary>
// ********************************************************************************
void GPUParticleSystem::Start()
{
    

    m_emmiterActive = true;
    
    
}

// ********************************************************************************
/// <summary>
/// 
/// </summary>
// ********************************************************************************
void GPUParticleSystem::Stop()
{
    m_emmiterActive = false;
    
}

// ********************************************************************************
/// <summary>
/// 
/// </summary>
/// <param name="_camera"></param>
// ********************************************************************************
void GPUParticleSystem::Render(Camera& _camera)
{
    if(!m_initialized) return;
    glUseProgram(m_computeProgram);
    glDispatchCompute(NUM_PARTICLES / 128, 1, 1);
    glMemoryBarrier(GL_ALL_BARRIER_BITS);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

    glUseProgram(m_program);
    

    glUniformMatrix4fv(glGetUniformLocation(m_program, "vp"), 1, GL_FALSE, glm::value_ptr(_camera.GetPV()));

   

    glBindBuffer(GL_ARRAY_BUFFER, m_posVBO);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, NULL, 0);
    glEnableVertexAttribArray(0);
    
    glDrawArrays(GL_POINTS,0, NUM_PARTICLES);

    glDisableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glUseProgram(0);

    
}


// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2021 Media Design School
//
// File Name   : CubeObject.cpp
// Description : Implementation file for CubeObject class
// Author      : Nerys Thamm
// Mail        : nerys.thamm@mds.ac.nz

#include "Renderable3D.h"
// ********************************************************************************
/// <summary>
/// Gets the model matrix of the mesh
/// </summary>
/// <returns>The model matrix of the mesh</returns>
// ********************************************************************************
glm::mat4 Renderable3D::GetModelMatrix()
{
	//Calculate model matrix
	m_translationMat = glm::translate(glm::mat4(), m_position);
	m_rotationMat = glm::rotate(glm::mat4(), glm::radians(m_fRotationX), glm::vec3(1.0f, 0.0f, 0.0f)) * glm::rotate(glm::mat4(), glm::radians(m_fRotationY), glm::vec3(0.0f, 1.0f, 0.0f)) * glm::rotate(glm::mat4(), glm::radians(m_fRotationZ), glm::vec3(0.0f, 0.0f, 1.0f));
	m_scaleMat = glm::scale(glm::mat4(), m_scale);
	return m_translationMat * m_rotationMat * m_scaleMat;
}

// ********************************************************************************
/// <summary>
/// Adds a texture to the Mesh's texturelist
/// </summary>
/// <param name="_texture">The texture to add</param>
// ********************************************************************************
void Renderable3D::AddTexture(GLuint _texture)
{
	//Add the texture
	m_textures.push_back(_texture);
}

// ********************************************************************************
/// <summary>
/// Adds a Vector of textures to the mesh's texturelist
/// </summary>
/// <param name="_textures">A vector of textures to add</param>
// ********************************************************************************
void Renderable3D::AddTextures(std::vector<GLuint>& _textures)
{
	//Add all proivided textures
	for (GLuint t : _textures)
	{
		m_textures.push_back(t);
	}
}

// ********************************************************************************
/// <summary>
///  Constructor
/// </summary>
/// <returns></returns>
// ********************************************************************************
Renderable3D::Renderable3D(Mesh3D* _mesh, Material* _material)
{
	//Initialise variables
	m_color = glm::vec3(1.0f, 1.0f, 1.0f);
	m_iTextureIndex = 0;
	m_iFadeIndex = 0;

	m_fRotationX = 0;
	m_fRotationY = 0;
	m_fRotationZ = 0;

	m_position = glm::vec3(0.0f, 0.0f, 0.0f);
	m_scale = glm::vec3(1.0f, 1.0f, 1.0f);

	m_mesh = _mesh;
	m_material = _material;
}

// ********************************************************************************
/// <summary>
/// Gets the Position
/// </summary>
/// <returns></returns>
// ********************************************************************************
glm::vec3 Renderable3D::Position()
{
	return m_position;
}

// ********************************************************************************
/// <summary>
/// Gets the Rotation
/// </summary>
/// <returns></returns>
// ********************************************************************************
glm::vec3 Renderable3D::Rotation()
{
	return glm::vec3(m_fRotationX, m_fRotationY, m_fRotationZ);
}

// ********************************************************************************
/// <summary>
/// Gets the Scale
/// </summary>
/// <returns></returns>
// ********************************************************************************
glm::vec3 Renderable3D::Scale()
{
	return m_scale;
}

// ********************************************************************************
/// <summary>
/// Gets the Color
/// </summary>
/// <returns></returns>
// ********************************************************************************
glm::vec3 Renderable3D::Color()
{
	return m_color;
}

// ********************************************************************************
/// <summary>
/// Sets the Position
/// </summary>
/// <param name="_pos"></param>
// ********************************************************************************
void Renderable3D::Position(glm::vec3 _pos)
{
	m_position = _pos;
}

// ********************************************************************************
/// <summary>
/// Sets the Rotation
/// </summary>
/// <param name="_rot"></param>
// ********************************************************************************
void Renderable3D::Rotation(glm::vec3 _rot)
{
	m_fRotationX = _rot.x;
	m_fRotationY = _rot.y;
	m_fRotationZ = _rot.z;
}

// ********************************************************************************
/// <summary>
/// Sets the Scale
/// </summary>
/// <param name="_scl"></param>
// ********************************************************************************
void Renderable3D::Scale(glm::vec3 _scl)
{
	m_scale = _scl;
}

// ********************************************************************************
/// <summary>
/// Sets the Color
/// </summary>
/// <param name="_col"></param>
// ********************************************************************************
void Renderable3D::Color(glm::vec3 _col)
{
	m_color = _col;
}


// ********************************************************************************
/// <summary>
/// Renders the attached mesh
/// </summary>
/// <param name="_camera"></param>
/// <param name="_program"></param>
// ********************************************************************************
void Renderable3D::Render(Camera _camera, GLuint _program)
{
	m_mesh->Render(_camera, _program, GetModelMatrix(), m_textures, *m_material);
}

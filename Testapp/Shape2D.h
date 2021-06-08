// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2021 Media Design School
//
// File Name   : Shape2D.h
// Description : Declares classes for 2d shape rendering
// Author      : Nerys Thamm
// Mail        : nerys.thamm@mds.ac.nz

#pragma once
#include <glew.h>
#include <glfw3.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <vector>
#include "Camera.h"
// --------------------------------------------------------------------------------
/// <summary>
/// Base class for shapes
/// </summary>
// --------------------------------------------------------------------------------
class Shape2D
{
public:
	glm::vec3 m_position;
	float m_fRotation;
	glm::vec3 m_scale;
	glm::vec3 m_color;

	glm::vec3 Position();
	glm::vec3 Rotation();
	glm::vec3 Scale();
	glm::vec3 Color();

	void Position(glm::vec3 _pos);
	void Rotation(glm::vec3 _rot);
	void Scale(glm::vec3 _scl);
	void Color(glm::vec3 _col);

	void Render(Camera _camera, GLuint _program);
	

	// ********************************************************************************
	/// <summary>
	/// Computes and returns the model matrix for the shape
	/// </summary>
	/// <returns>The model matrix for the shape as mat4</returns>
	// ********************************************************************************
	glm::mat4 GetModelMatrix();
	GLuint m_VAO;
	std::vector<GLuint> m_textures;

	// ********************************************************************************
	/// <summary>
	/// Adds a texture to the texture list
	/// </summary>
	/// <param name="_texture">The texture to add</param>
	// ********************************************************************************
	void AddTexture(GLuint _texture);
	// ********************************************************************************
	/// <summary>
	/// Adds multiple textures to the texture list
	/// </summary>
	/// <param name="_textures">A vector of textures to add</param>
	// ********************************************************************************
	void AddTextures(std::vector<GLuint>& _textures);
	int m_iTextureIndex;
	int m_iFadeIndex;
	int m_verticesCount;
protected:
	// ********************************************************************************
	/// <summary>
	/// Constructor
	/// </summary>
	/// <returns></returns>
	// ********************************************************************************
	Shape2D();
	// ********************************************************************************
	/// <summary>
	/// Virtual destructor to prevent instantiation of base class
	/// </summary>
	/// <returns></returns>
	// ********************************************************************************
	virtual ~Shape2D() = 0;
	GLfloat m_vertices;
	GLuint m_indices;
	GLuint m_VBO;
	GLuint m_EBO;

private:

	glm::mat4 m_translationMat;
	glm::mat4 m_rotationMat;
	glm::mat4 m_scaleMat;
};

// --------------------------------------------------------------------------------
/// <summary>
/// Hexagon shape class
/// </summary>
// --------------------------------------------------------------------------------
class Hex2D : public Shape2D
{
public:
	// ********************************************************************************
	/// <summary>
	/// Constructor
	/// </summary>
	/// <returns></returns>
	// ********************************************************************************
	Hex2D();
	GLfloat m_vertices[48] = {
		// Index   // Position         // Colour                           // Texture Coords
		/* 1 */   -0.25f, 0.5f, 0.0f,   m_color.x, m_color.y, m_color.z,   0.25f, 1.0f,
		/* 2 */   -0.5f, 0.0f, 0.0f,   m_color.x, m_color.y, m_color.z,   0.0f, 0.5f,
		/* 3 */   -0.25f,-0.5f, 0.0f,   m_color.x, m_color.y, m_color.z,   0.25f, 0.0f,
		/* 4 */    0.25f, -0.5f, 0.0f,   m_color.x, m_color.y, m_color.z,   0.75f, 0.0f,
		/* 5 */    0.5f, 0.0f, 0.0f,   m_color.x, m_color.y, m_color.z,   1.0f, 0.5f,
		/* 6 */    0.25f, 0.5f, 0.0f,   m_color.x, m_color.y, m_color.z,   0.75f, 1.0f,
	};
	GLuint m_indices[12] = {
	0, 1, 2,
	0, 2, 5,
	5, 2, 4,
	4, 2, 3,
	};
};
// --------------------------------------------------------------------------------
/// <summary>
/// Quad shape class
/// </summary>
// --------------------------------------------------------------------------------
class Quad2D : public Shape2D
{
public:
	// ********************************************************************************
	/// <summary>
	/// Constructor
	/// </summary>
	/// <returns></returns>
	// ********************************************************************************
	Quad2D();
	GLfloat m_vertices[44] = {
		// Index   // Position         // Colour                          // Texture Coords   //Normals
		/* 1 */   -0.5f, 0.5f, 0.0f,   m_color.x, m_color.y, m_color.z,   0.0f, 1.0f,         0.0f, 0.0f, 1.0f,            // Top - Left
		/* 2 */   -0.5f,-0.5f, 0.0f,   m_color.x, m_color.y, m_color.z,   0.0f, 0.0f,         0.0f, 0.0f, 1.0f,            // Bot - Left
		/* 3 */    0.5f,-0.5f, 0.0f,   m_color.x, m_color.y, m_color.z,   1.0f, 0.0f,         0.0f, 0.0f, 1.0f,            // Bot - Right
		/* 4 */    0.5f, 0.5f, 0.0f,   m_color.x, m_color.y, m_color.z,   1.0f, 1.0f,         0.0f, 0.0f, 1.0f,            // Top - Right
	};
	GLuint m_indices[6] = {
	0, 1, 2, // First Triangle (TL -> BL -> BR)
	0, 2, 3, // Second Triangle (TL -> BR -> TR)
	};
};

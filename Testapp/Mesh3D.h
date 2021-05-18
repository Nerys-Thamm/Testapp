#pragma once
#include <glew.h>
#include <glfw3.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include "Camera.h"
#include <vector>
#define _USE_MATH_DEFINES
#include <cmath>
#include <math.h>

class Mesh3D
{
public:
	glm::vec3 m_position;
	float m_fRotationX;
	float m_fRotationY;
	float m_fRotationZ;
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
	Mesh3D();
	// ********************************************************************************
	/// <summary>
	/// Virtual destructor to prevent instantiation of base class
	/// </summary>
	/// <returns></returns>
	// ********************************************************************************
	virtual ~Mesh3D() = 0;
	GLfloat m_vertices;
	GLuint m_indices;
	GLuint m_VBO;
	GLuint m_EBO;

private:

	glm::mat4 m_translationMat;
	glm::mat4 m_rotationMat;
	glm::mat4 m_scaleMat;
};

class Pyramid3D : public Mesh3D
{
public:
	// ********************************************************************************
	/// <summary>
	/// Constructor
	/// </summary>
	/// <returns></returns>
	// ********************************************************************************
	Pyramid3D();
	GLfloat m_vertices[48] = {
		// Index   // Position         // Colour                           // Texture Coords
		/* 0 */   -0.5f, 0.0f, -0.5f,   m_color.x, m_color.y, m_color.z,   0.0f, 1.0f,
		/* 1 */   -0.5f, 0.0f, 0.5f,   m_color.x, m_color.y, m_color.z,   0.0f, 0.0f,
		/* 2 */    0.5f, 0.0f, 0.5f,   m_color.x, m_color.y, m_color.z,   1.0f, 0.0f,
		/* 3 */    0.5f, 0.0f, -0.5f,   m_color.x, m_color.y, m_color.z,   1.0f, 1.0f,

		/* 4 */    0.0f, 0.5f, 0.0f,   m_color.x, m_color.y, m_color.z,   0.5f, 0.5f, //Top Point
		
	};
	GLuint m_indices[18] = {
	0, 4, 3,
	3, 4, 2,
	2, 4, 1,
	1, 4, 0,

	1, 0, 3,
	1, 3, 2,
	};
};

class Cube3D : public Mesh3D
{
public:
	// ********************************************************************************
	/// <summary>
	/// Constructor
	/// </summary>
	/// <returns></returns>
	// ********************************************************************************
	Cube3D();
	GLfloat m_vertices[192] = {
		// Index   // Position         // Colour                           // Texture Coords
		           //Front
		/* 00 */   -0.5f,  0.5f,  0.5f,   m_color.x, m_color.y, m_color.z,   0.0f, 1.0f,
		/* 01 */   -0.5f, -0.5f,  0.5f,   m_color.x, m_color.y, m_color.z,   0.0f, 0.0f,
		/* 02 */    0.5f, -0.5f,  0.5f,   m_color.x, m_color.y, m_color.z,   1.0f, 0.0f,
		/* 03 */    0.5f,  0.5f,  0.5f,   m_color.x, m_color.y, m_color.z,   1.0f, 1.0f,
		           //Back
		/* 04 */    0.5f,  0.5f, -0.5f,   m_color.x, m_color.y, m_color.z,   0.0f, 1.0f,
		/* 05 */    0.5f, -0.5f, -0.5f,   m_color.x, m_color.y, m_color.z,   0.0f, 0.0f,
		/* 06 */   -0.5f, -0.5f, -0.5f,   m_color.x, m_color.y, m_color.z,   1.0f, 0.0f,
		/* 07 */   -0.5f,  0.5f, -0.5f,   m_color.x, m_color.y, m_color.z,   1.0f, 1.0f,
		           //Right
		/* 08 */    0.5f,  0.5f,  0.5f,   m_color.x, m_color.y, m_color.z,   0.0f, 1.0f,
		/* 09 */    0.5f, -0.5f,  0.5f,   m_color.x, m_color.y, m_color.z,   0.0f, 0.0f,
		/* 10 */    0.5f, -0.5f, -0.5f,   m_color.x, m_color.y, m_color.z,   1.0f, 0.0f,
		/* 11 */    0.5f,  0.5f, -0.5f,   m_color.x, m_color.y, m_color.z,   1.0f, 1.0f,
		           //Left
		/* 12 */   -0.5f,  0.5f, -0.5f,   m_color.x, m_color.y, m_color.z,   0.0f, 1.0f,
		/* 13 */   -0.5f, -0.5f, -0.5f,   m_color.x, m_color.y, m_color.z,   0.0f, 0.0f,
		/* 14 */   -0.5f, -0.5f,  0.5f,   m_color.x, m_color.y, m_color.z,   1.0f, 0.0f,
		/* 15 */   -0.5f,  0.5f,  0.5f,   m_color.x, m_color.y, m_color.z,   1.0f, 1.0f,
		           //Top
		/* 16 */   -0.5f,  0.5f, -0.5f,   m_color.x, m_color.y, m_color.z,   0.0f, 1.0f,
		/* 17 */   -0.5f,  0.5f,  0.5f,   m_color.x, m_color.y, m_color.z,   0.0f, 0.0f,
		/* 18 */    0.5f,  0.5f,  0.5f,   m_color.x, m_color.y, m_color.z,   1.0f, 0.0f,
		/* 19 */    0.5f,  0.5f, -0.5f,   m_color.x, m_color.y, m_color.z,   1.0f, 1.0f,
		           //Bottom
		/* 20 */   -0.5f, -0.5f,  0.5f,   m_color.x, m_color.y, m_color.z,   0.0f, 1.0f,
		/* 21 */   -0.5f, -0.5f, -0.5f,   m_color.x, m_color.y, m_color.z,   0.0f, 0.0f,
		/* 22 */    0.5f, -0.5f, -0.5f,   m_color.x, m_color.y, m_color.z,   1.0f, 0.0f,
		/* 23 */    0.5f, -0.5f,  0.5f,   m_color.x, m_color.y, m_color.z,   1.0f, 1.0f,

	};
	GLuint m_indices[36] = {
	0, 1, 2,
	0, 2, 3,
	4, 5, 6,
	4, 6, 7,
	8, 9, 10,
	8, 10, 11,
	12, 13, 14,
	12, 14, 15,
	16, 17, 18,
	16, 18, 19,
	20, 21, 22,
	20, 22, 23,
	};
};

class Sphere3D : public Mesh3D
{
public:
	Sphere3D(float Radius, int Fidelity);
private:
	int IndexCount;
	int DrawType;
};
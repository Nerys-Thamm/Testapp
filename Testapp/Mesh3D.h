#pragma once
#include <glew.h>
#include <glfw3.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <vector>
class Mesh3D
{
public:
	glm::vec3 m_position;
	float m_fRotationX;
	float m_fRotationY;
	float m_fRotationZ;
	glm::vec3 m_scale;
	glm::vec3 m_color;
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
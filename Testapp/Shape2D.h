#pragma once
#include <glew.h>
#include <glfw3.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <vector>
class Shape2D
{
public:
	glm::vec3 m_position;
	float m_rotation;
	glm::vec3 m_scale;
	glm::vec3 m_color;
	glm::mat4 GetModelMatrix();
	GLuint m_VAO;
	std::vector<GLuint> m_textures;
	Shape2D();
	void SetTexture(GLuint _texture);
	void AddTextures(std::vector<GLuint>& _textures);
	int m_itextureindex;
	int m_ifadeindex;
	int m_verticescount;
protected:
	GLfloat m_vertices;
	GLuint m_indices;
	GLuint m_VBO;
	GLuint m_EBO;

private:

	glm::mat4 m_translation_mat;
	glm::mat4 m_rotation_mat;
	glm::mat4 m_scale_mat;
};

class Hex2D : public Shape2D
{
public:
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
class Quad2D : public Shape2D
{
public:
	Quad2D();
	GLfloat m_vertices[32] = {
		// Index   // Position         // Colour                          // Texture Coords
		/* 1 */   -0.5f, 0.5f, 0.0f,   m_color.x, m_color.y, m_color.z,   0.0f, 1.0f,        // Top - Left
		/* 2 */   -0.5f,-0.5f, 0.0f,   m_color.x, m_color.y, m_color.z,   0.0f, 0.0f,        // Bot - Left
		/* 3 */    0.5f,-0.5f, 0.0f,   m_color.x, m_color.y, m_color.z,   1.0f, 0.0f,        // Bot - Right
		/* 4 */    0.5f, 0.5f, 0.0f,   m_color.x, m_color.y, m_color.z,   1.0f, 1.0f,        // Top - Right
	};
	GLuint m_indices[6] = {
	0, 1, 2, // First Triangle (TL -> BL -> BR)
	0, 2, 3, // Second Triangle (TL -> BR -> TR)
	};
};

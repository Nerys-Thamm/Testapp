// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
// 
// (c) 2021 Media Design School
//
// File Name   : Shape2D.cpp
// Description : Defines 2D shape rendering classes
// Author      : Nerys Thamm
// Mail        : nerys.thamm@mds.ac.nz

#include "Shape2D.h"

glm::mat4 Shape2D::GetModelMatrix()
{
	//Calculate model matrix
	m_translationMat = glm::translate(glm::mat4(), m_position);
	m_rotationMat = glm::rotate(glm::mat4(), glm::radians(m_fRotation), glm::vec3(0.0f, 0.0f, 1.0f));
	m_scaleMat = glm::scale(glm::mat4(), m_scale);
	return m_translationMat * m_rotationMat * m_scaleMat;
}

Shape2D::Shape2D()
{
	//Initialise variables
	m_color = glm::vec3(1.0f, 1.0f, 1.0f);
	m_iTextureIndex = 0;
	m_iFadeIndex = 0;
	m_EBO = 0;
	m_VAO = 0;
	m_VBO = 0;
	m_indices = 0;
	m_fRotation = 0;
	m_vertices = 0;
	m_verticesCount = 0;


}
Shape2D::~Shape2D()
{

}
void Shape2D::AddTexture(GLuint _texture)
{
	//Add the texture
	m_textures.push_back(_texture);
}

void Shape2D::AddTextures(std::vector<GLuint>& _textures)
{
	//Add all proivided textures
	for (GLuint t : _textures)
	{
		m_textures.push_back(t);
	}
}

Hex2D::Hex2D()
{
	
	m_verticesCount = 12;

	m_position = glm::vec3(0.0f, 0.0f, 0.0f);
	m_fRotation = 0.0f;
	m_scale = glm::vec3(1.0f, 1.0f, 1.0f);

	//Generate and bind vertex array to VAO
	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);

	//Generate and bind EBO
	glGenBuffers(1, &m_EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(m_indices), m_indices, GL_STATIC_DRAW);

	//Generate and bind VBO
	glGenBuffers(1, &m_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(m_vertices), m_vertices, GL_STATIC_DRAW);

	//Set attribute pointers
	glVertexAttribPointer(0, 3, GL_FLOAT, false, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, false, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, false, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);
}

Quad2D::Quad2D()
{
	m_verticesCount = 6;

	m_position = glm::vec3(0.0f, 0.0f, 0.0f);
	m_fRotation = 0.0f;
	m_scale = glm::vec3(1.0f, 1.0f, 1.0f);

	//Generate and bind vertex array to VAO
	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);

	//Generate and bind EBO
	glGenBuffers(1, &m_EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(m_indices), m_indices, GL_STATIC_DRAW);

	//Generate and bind VBO
	glGenBuffers(1, &m_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(m_vertices), m_vertices, GL_STATIC_DRAW);

	//Set attribute pointers
	glVertexAttribPointer(0, 3, GL_FLOAT, false, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, false, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, false, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);
}
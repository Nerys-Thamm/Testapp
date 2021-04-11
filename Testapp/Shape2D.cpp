#include "Shape2D.h"

glm::mat4 Shape2D::GetModelMatrix()
{
	m_translation_mat = glm::translate(glm::mat4(), m_position);
	m_rotation_mat = glm::rotate(glm::mat4(), glm::radians(m_rotation), glm::vec3(0.0f, 0.0f, 1.0f));
	m_scale_mat = glm::scale(glm::mat4(), m_scale);
	return m_translation_mat * m_rotation_mat * m_scale_mat;
}

Shape2D::Shape2D()
{
	m_color = glm::vec3(1.0f, 1.0f, 1.0f);
	m_itextureindex = 0;
	m_ifadeindex = 0;
}

void Shape2D::SetTexture(GLuint _texture)
{
	m_textures.push_back(_texture);
}

void Shape2D::AddTextures(std::vector<GLuint>& _textures)
{
	for (GLuint t : _textures)
	{
		m_textures.push_back(t);
	}
}

Hex2D::Hex2D()
{
	m_verticescount = 12;

	m_position = glm::vec3(0.0f, 0.0f, 0.0f);
	m_rotation = 0.0f;
	m_scale = glm::vec3(1.0f, 1.0f, 1.0f);

	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);

	glGenBuffers(1, &m_EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(m_indices), m_indices, GL_STATIC_DRAW);

	glGenBuffers(1, &m_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(m_vertices), m_vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, false, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, false, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, false, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);
}

Quad2D::Quad2D()
{
	m_verticescount = 6;

	m_position = glm::vec3(0.0f, 0.0f, 0.0f);
	m_rotation = 0.0f;
	m_scale = glm::vec3(1.0f, 1.0f, 1.0f);

	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);

	glGenBuffers(1, &m_EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(m_indices), m_indices, GL_STATIC_DRAW);

	glGenBuffers(1, &m_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(m_vertices), m_vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, false, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, false, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, false, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);
}
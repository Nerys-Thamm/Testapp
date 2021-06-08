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

// ********************************************************************************
/// <summary>
/// Gets the model matrix of the shape
/// </summary>
/// <returns></returns>
// ********************************************************************************
glm::mat4 Shape2D::GetModelMatrix()
{
	//Calculate model matrix
	m_translationMat = glm::translate(glm::mat4(), m_position);
	m_rotationMat = glm::rotate(glm::mat4(), glm::radians(m_fRotation), glm::vec3(0.0f, 0.0f, 1.0f));
	m_scaleMat = glm::scale(glm::mat4(), m_scale);
	return m_translationMat * m_rotationMat * m_scaleMat;
}

// ********************************************************************************
/// <summary>
/// Constructor
/// </summary>
/// <returns></returns>
// ********************************************************************************
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
// ********************************************************************************
/// <summary>
/// Destructor
/// </summary>
/// <returns></returns>
// ********************************************************************************
Shape2D::~Shape2D()
{
}

// ********************************************************************************
/// <summary>
/// Adds a texture to the shapes' texture list
/// </summary>
/// <param name="_texture">The texture to add</param>
// ********************************************************************************
void Shape2D::AddTexture(GLuint _texture)
{
	//Add the texture
	m_textures.push_back(_texture);
}

// ********************************************************************************
/// <summary>
/// Adds a vector of Textures to the shapes' texture list
/// </summary>
/// <param name="_textures">The vector of textures to add</param>
// ********************************************************************************
void Shape2D::AddTextures(std::vector<GLuint>& _textures)
{
	//Add all proivided textures
	for (GLuint t : _textures)
	{
		m_textures.push_back(t);
	}
}

// ********************************************************************************
/// <summary>
/// Constructor
/// </summary>
/// <returns></returns>
// ********************************************************************************
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

// ********************************************************************************
/// <summary>
/// Constructor
/// </summary>
/// <returns></returns>
// ********************************************************************************
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
	glVertexAttribPointer(3, 3, GL_FLOAT, false, 8 * sizeof(GLfloat), (GLvoid*)(8 * sizeof(GLfloat)));
	glEnableVertexAttribArray(3);
}

// ********************************************************************************
/// <summary>
/// Gets the position of the shape
/// </summary>
/// <returns></returns>
// ********************************************************************************
glm::vec3 Shape2D::Position()
{
	return m_position;
}

// ********************************************************************************
/// <summary>
/// Gets the rotation of the shape
/// </summary>
/// <returns></returns>
// ********************************************************************************
glm::vec3 Shape2D::Rotation()
{
	return glm::vec3(0.0f, 0.0f, m_fRotation);
}

// ********************************************************************************
/// <summary>
/// Gets the scale of the shape
/// </summary>
/// <returns></returns>
// ********************************************************************************
glm::vec3 Shape2D::Scale()
{
	return m_scale;
}

// ********************************************************************************
/// <summary>
/// Gets the color of the shape
/// </summary>
/// <returns></returns>
// ********************************************************************************
glm::vec3 Shape2D::Color()
{
	return m_color;
}

// ********************************************************************************
/// <summary>
/// Sets the position of the shape
/// </summary>
/// <param name="_pos"></param>
// ********************************************************************************
void Shape2D::Position(glm::vec3 _pos)
{
	m_position = _pos;
}

// ********************************************************************************
/// <summary>
/// Sets the rotation of the shape
/// </summary>
/// <param name="_rot"></param>
// ********************************************************************************
void Shape2D::Rotation(glm::vec3 _rot)
{
	m_fRotation = _rot.z;
}

// ********************************************************************************
/// <summary>
/// Sets the scale of the shape
/// </summary>
/// <param name="_scl"></param>
// ********************************************************************************
void Shape2D::Scale(glm::vec3 _scl)
{
	m_scale = _scl;
}

// ********************************************************************************
/// <summary>
/// Sets the color of the shape
/// </summary>
/// <param name="_col"></param>
// ********************************************************************************
void Shape2D::Color(glm::vec3 _col)
{
	m_color = _col;
}

// ********************************************************************************
/// <summary>
/// Renders the shape
/// </summary>
/// <param name="_camera">The camera to use to render</param>
/// <param name="_program">The program to use to render</param>
// ********************************************************************************
void Shape2D::Render(Camera _camera, GLuint _program)
{
	//Calculate the PVM matrix
	glm::mat4 PVMMat = _camera.GetPVM(GetModelMatrix());

	//Bind program and VAO
	glUseProgram(_program);
	glBindVertexArray(m_VAO);

	//Send Vars to shaders via Uniform
	GLint CurrentTimeLoc = glGetUniformLocation(_program, "CurrentTime");
	glUniform1f(CurrentTimeLoc, glfwGetTime());
	GLint PVMMatLoc = glGetUniformLocation(_program, "PVMMat");
	glUniformMatrix4fv(PVMMatLoc, 1, GL_FALSE, glm::value_ptr(PVMMat));

	//If the shape has textures provided
	if (!m_textures.empty())
	{
		//Set texture uniforms
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_textures[m_iTextureIndex]);
		glUniform1i(glGetUniformLocation(_program, "ImageTexture"), 0);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, m_textures[m_iFadeIndex]);
		glUniform1i(glGetUniformLocation(_program, "ImageTexture1"), 1);
	}

	//Render the Shape
	glDrawElements(GL_TRIANGLES, m_verticesCount, GL_UNSIGNED_INT, 0);

	//Unbind assets
	glBindVertexArray(0);
	glUseProgram(0);
}


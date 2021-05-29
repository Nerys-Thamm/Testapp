// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2021 Media Design School
//
// File Name   : Mesh3D.cpp
// Description : Implementation file for the Mesh3D class
// Author      : Nerys Thamm
// Mail        : nerys.thamm@mds.ac.nz

#include "Mesh3D.h"

// ********************************************************************************
/// <summary>
/// Gets the model matrix of the mesh
/// </summary>
/// <returns>The model matrix of the mesh</returns>
// ********************************************************************************
glm::mat4 Mesh3D::GetModelMatrix()
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
void Mesh3D::AddTexture(GLuint _texture)
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
void Mesh3D::AddTextures(std::vector<GLuint>& _textures)
{
	//Add all proivided textures
	for (GLuint t : _textures)
	{
		m_textures.push_back(t);
	}
}

// ********************************************************************************
/// <summary>
/// Base constructor, should never be called manually
/// </summary>
/// <returns></returns>
// ********************************************************************************
Mesh3D::Mesh3D()
{
	//Initialise variables
	m_color = glm::vec3(1.0f, 1.0f, 1.0f);
	m_iTextureIndex = 0;
	m_iFadeIndex = 0;
	m_EBO = 0;
	m_VAO = 0;
	m_VBO = 0;
	m_indices = 0;
	m_fRotationX = 0;
	m_fRotationY = 0;
	m_fRotationZ = 0;
	m_vertices = 0;
	m_verticesCount = 0;
	m_position = glm::vec3(0.0f, 0.0f, 0.0f);
	m_scale = glm::vec3(1.0f, 1.0f, 1.0f);
}
// ********************************************************************************
/// <summary>
/// Destructor
/// </summary>
/// <returns></returns>
// ********************************************************************************
Mesh3D::~Mesh3D()
{
}

// ********************************************************************************
/// <summary>
/// 3D Pyramid Mesh Constructor
/// </summary>
/// <returns></returns>
// ********************************************************************************
Pyramid3D::Pyramid3D()
{
	m_verticesCount = 18;

	m_position = glm::vec3(0.0f, 0.0f, 0.0f);
	m_fRotationX = 0.0f;
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
/// 3D Cube Mesh Constructor
/// </summary>
/// <returns></returns>
// ********************************************************************************
Cube3D::Cube3D()
{
	m_verticesCount = sizeof(m_indices) / sizeof(int32_t);

	m_position = glm::vec3(0.0f, 0.0f, 0.0f);
	m_fRotationX = 0.0f;
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
/// Renders the Mesh
/// </summary>
/// <param name="_camera">The camera to use</param>
/// <param name="_program">The program to use</param>
// ********************************************************************************
void Mesh3D::Render(Camera _camera, GLuint _program)
{
	//Calculate the PVM matrix
	glm::mat4 PVMMat = _camera.GetPVM(GetModelMatrix());
	glm::mat4 ModelMat = GetModelMatrix();
	//Bind program and VAO
	glUseProgram(_program);
	glBindVertexArray(m_VAO);

	//Send Vars to shaders via Uniform
	GLint CurrentTimeLoc = glGetUniformLocation(_program, "CurrentTime");
	glUniform1f(CurrentTimeLoc, glfwGetTime());
	GLint PVMMatLoc = glGetUniformLocation(_program, "PVMMat");
	glUniformMatrix4fv(PVMMatLoc, 1, GL_FALSE, glm::value_ptr(PVMMat));
	GLint ModelLoc = glGetUniformLocation(_program, "Model");
	glUniformMatrix4fv(ModelLoc, 1, GL_FALSE, glm::value_ptr(ModelMat));

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

// ********************************************************************************
/// <summary>
/// Gets the Position
/// </summary>
/// <returns></returns>
// ********************************************************************************
glm::vec3 Mesh3D::Position()
{
	return m_position;
}

// ********************************************************************************
/// <summary>
/// Gets the Rotation
/// </summary>
/// <returns></returns>
// ********************************************************************************
glm::vec3 Mesh3D::Rotation()
{
	return glm::vec3(m_fRotationX, m_fRotationY, m_fRotationZ);
}

// ********************************************************************************
/// <summary>
/// Gets the Scale
/// </summary>
/// <returns></returns>
// ********************************************************************************
glm::vec3 Mesh3D::Scale()
{
	return m_scale;
}

// ********************************************************************************
/// <summary>
/// Gets the Color
/// </summary>
/// <returns></returns>
// ********************************************************************************
glm::vec3 Mesh3D::Color()
{
	return m_color;
}

// ********************************************************************************
/// <summary>
/// Sets the Position
/// </summary>
/// <param name="_pos"></param>
// ********************************************************************************
void Mesh3D::Position(glm::vec3 _pos)
{
	m_position = _pos;
}

// ********************************************************************************
/// <summary>
/// Sets the Rotation
/// </summary>
/// <param name="_rot"></param>
// ********************************************************************************
void Mesh3D::Rotation(glm::vec3 _rot)
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
void Mesh3D::Scale(glm::vec3 _scl)
{
	m_scale = _scl;
}

// ********************************************************************************
/// <summary>
/// Sets the Color
/// </summary>
/// <param name="_col"></param>
// ********************************************************************************
void Mesh3D::Color(glm::vec3 _col)
{
	m_color = _col;
}

// ********************************************************************************
/// <summary>
/// 3D Sphere Mesh Constructor
/// </summary>
/// <param name="Radius">Radius of the sphere</param>
/// <param name="Fidelity">Quality of the sphere</param>
/// <returns></returns>
// ********************************************************************************
Sphere3D::Sphere3D(float Radius, int Fidelity)
{
	int VertexAttrib = 8;	// Float components are needed for each vertex point
	int IndexPerQuad = 6;	// Indices needed to create a quad

	// Angles to keep track of the sphere points
	float Phi = 0.0f;
	float Theta = 0.0f;

	// Create the vertex array to hold the correct number of elements based on the fidelity of the sphere
	int VertexCount = Fidelity * Fidelity * VertexAttrib;
	GLfloat* Vertices = new GLfloat[VertexCount];
	int Element = 0;

	// Each cycle moves down on the vertical (Y axis) to start the next ring
	for (int i = 0; i < Fidelity; i++)
	{
		// A new  horizontal ring starts at 0 degrees
		Theta = 0.0f;

		// Creates a horizontal ring and adds each new vertex point to the vertex array
		for (int j = 0; j < Fidelity; j++)
		{
			// Calculate the new vertex position point with the new angles
			float x = cos(Phi) * sin(Theta);
			float y = cos(Theta);
			float z = sin(Phi) * sin(Theta);

			// Set the position of the current vertex point
			Vertices[Element++] = x * Radius;
			Vertices[Element++] = y * Radius;
			Vertices[Element++] = z * Radius;

			// Set the texture coordinates of the current vertex point
			Vertices[Element++] = (float)i / (Fidelity - 1);
			Vertices[Element++] = 1 - ((float)j / (Fidelity - 1)); // 1 minus in order to flip the direction of 0-1 (0 at the bottom)

			// Set the normal direction of the current vertex point
			Vertices[Element++] = x;
			Vertices[Element++] = y;
			Vertices[Element++] = z;

			// Theta (Y axis) angle is incremented based on the angle created by number of sections
			// As the sphere is built ring by ring, the theta is only needed to do half the circumferance therefore using just PI
			Theta += ((float)M_PI / ((float)Fidelity - 1.0f));
		}

		// Phi angle (X and Z axes) is incremented based on the angle created by the number of sections
		// Angle uses 2*PI to get the full circumference as this layer is built as a full ring
		Phi += (2.0f * (float)M_PI) / ((float)Fidelity - 1.0f);
	}

	// Create the index array to hold the correct number of elements based on the fidelity of the sphere
	IndexCount = Fidelity * Fidelity * IndexPerQuad;
	GLuint* Indices = new GLuint[IndexCount];

	Element = 0;	// Reset the element offset for the new array
	for (int i = 0; i < Fidelity; i++)
	{
		for (int j = 0; j < Fidelity; j++)
		{
			// First triangle of the quad
			Indices[Element++] = (((i + 1) % Fidelity) * Fidelity) + ((j + 1) % Fidelity);
			Indices[Element++] = (i * Fidelity) + (j);
			Indices[Element++] = (((i + 1) % Fidelity) * Fidelity) + (j);

			// Second triangle of the quad
			Indices[Element++] = (i * Fidelity) + ((j + 1) % Fidelity);
			Indices[Element++] = (i * Fidelity) + (j);
			Indices[Element++] = (((i + 1) % Fidelity) * Fidelity) + ((j + 1) % Fidelity);
		}
	}

	// Create the Vertex Array and associated buffers

	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);
	glGenBuffers(1, &m_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, VertexCount * sizeof(GLfloat), Vertices, GL_STATIC_DRAW);
	glGenBuffers(1, &m_EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, IndexCount * sizeof(GLuint), Indices, GL_STATIC_DRAW);

	// Vertex Information (Position, Texture Coords and Normals)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(5 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	DrawType = GL_TRIANGLES;

	// Clean up the used memory
	delete[] Vertices;
	delete[] Indices;

	m_verticesCount = IndexCount;
}
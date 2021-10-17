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
/// Renders the Mesh
/// </summary>
/// <param name="_camera">The camera to use</param>
/// <param name="_program">The program to use</param>
// ********************************************************************************

void Mesh3D::Render(Camera _camera, GLuint _program, glm::mat4 _modelmat, glm::vec3 _color)
{
	//Calculate the PVM matrix
	glm::mat4 PVMMat = _camera.GetPVM(_modelmat);
	glm::mat4 ModelMat = _modelmat;
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

	glUniform3fv(glGetUniformLocation(_program, "Color"), 1, glm::value_ptr(_color));

	//Render the Shape
	glDrawElements(GL_TRIANGLES, m_verticesCount, GL_UNSIGNED_INT, 0);

	//Unbind assets
	glBindVertexArray(0);
	glUseProgram(0);
}



// ********************************************************************************
/// <summary>
/// Renders the Mesh
/// </summary>
/// <param name="_camera">The camera to use</param>
/// <param name="_program">The program to use</param>
// ********************************************************************************

void Mesh3D::Render(Camera _camera, GLuint _program, glm::mat4 _modelmat, std::vector<GLuint> _textures, Material& _material, int _textureindex, int _fadeindex)
{
	//Calculate the PVM matrix
	glm::mat4 PVMMat = _camera.GetPVM(_modelmat);
	glm::mat4 ModelMat = _modelmat;
	//Bind program and VAO
	glUseProgram(_program);
	glBindVertexArray(m_VAO);

	//Send Vars to shaders via Uniform

	//Time
	GLint CurrentTimeLoc = glGetUniformLocation(_program, "CurrentTime");
	glUniform1f(CurrentTimeLoc, glfwGetTime());

	//Matrices
	GLint PVMMatLoc = glGetUniformLocation(_program, "PVMMat");
	glUniformMatrix4fv(PVMMatLoc, 1, GL_FALSE, glm::value_ptr(PVMMat));
	GLint ModelLoc = glGetUniformLocation(_program, "Model");
	glUniformMatrix4fv(ModelLoc, 1, GL_FALSE, glm::value_ptr(ModelMat));

	//Camera
	GLint CamLoc = glGetUniformLocation(_program, "CameraPos");
	glUniform3fv(CamLoc, 1, glm::value_ptr(_camera.m_cameraPos));

	//Material
	glUniform1f(glGetUniformLocation(_program, "Mat[0].Smoothness"), _material.Smoothness);
	glUniform1f(glGetUniformLocation(_program, "Mat[0].Reflectivity"), _material.Reflectivity);


	//Lighting
	//PointLights
	for (size_t i = 0; i < Lighting::MAX_POINT_LIGHTS; i++)
	{
		glUniform3fv(glGetUniformLocation(_program, ("PointLights[" + std::to_string(i) + "].Position").c_str()), 1, glm::value_ptr(Lighting::PointLights[i].Position));
		glUniform3fv(glGetUniformLocation(_program, ("PointLights[" + std::to_string(i) + "].Color").c_str()), 1, glm::value_ptr(Lighting::PointLights[i].Color));
		glUniform1f(glGetUniformLocation(_program, ("PointLights[" + std::to_string(i) + "].AmbientStrength").c_str()), Lighting::PointLights[i].AmbientStrength);
		glUniform1f(glGetUniformLocation(_program, ("PointLights[" + std::to_string(i) + "].SpecularStrength").c_str()), Lighting::PointLights[i].SpecularStrength);
		glUniform1f(glGetUniformLocation(_program, ("PointLights[" + std::to_string(i) + "].AttenuationConstant").c_str()), Lighting::PointLights[i].AttenuationConstant);
		glUniform1f(glGetUniformLocation(_program, ("PointLights[" + std::to_string(i) + "].AttenuationExponent").c_str()), Lighting::PointLights[i].AttenuationExponent);
		glUniform1f(glGetUniformLocation(_program, ("PointLights[" + std::to_string(i) + "].AttenuationLinear").c_str()), Lighting::PointLights[i].AttenuationLinear);
	}

	//DirectionalLights
	for (size_t i = 0; i < Lighting::MAX_DIRECTIONAL_LIGHTS; i++)
	{
		glUniform3fv(glGetUniformLocation(_program, ("DirectionalLights[" + std::to_string(i) + "].Direction").c_str()), 1, glm::value_ptr(Lighting::DirectionalLights[i].Direction));
		glUniform3fv(glGetUniformLocation(_program, ("DirectionalLights[" + std::to_string(i) + "].Color").c_str()), 1, glm::value_ptr(Lighting::DirectionalLights[i].Color));
		glUniform1f(glGetUniformLocation(_program, ("DirectionalLights[" + std::to_string(i) + "].AmbientStrength").c_str()), Lighting::DirectionalLights[i].AmbientStrength);
		glUniform1f(glGetUniformLocation(_program, ("DirectionalLights[" + std::to_string(i) + "].SpecularStrength").c_str()), Lighting::DirectionalLights[i].SpecularStrength);
	}

	
	
	

	

	//If the shape has textures provided
	if (!_textures.empty())
	{
		//Set texture uniforms
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, _textures[_textureindex]);
		glUniform1i(glGetUniformLocation(_program, "ImageTexture"), 0);
		if (_textures.size() > 1)
		{
			glActiveTexture(GL_TEXTURE2);
			glBindTexture(GL_TEXTURE_2D, _textures[_fadeindex]);
			glUniform1i(glGetUniformLocation(_program, "ReflectionMap"), 2);
		}
	}
	
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_CUBE_MAP, SceneManager::GetCurrentSkybox()->GetCubemap());
	glUniform1i(glGetUniformLocation(_program, "CubeMap"), 1);
	


	//Render the Shape
	glDrawElements(GL_TRIANGLES, m_verticesCount, GL_UNSIGNED_INT, 0);

	//Unbind assets
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	glUseProgram(0);
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
	m_EBO = 0;
	m_VAO = 0;
	m_VBO = 0;
	m_indices = 0;
	m_vertices = 0;
	m_verticesCount = 0;
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

Mesh3D* Pyramid3D::GetMesh()
{
	if (m_mesh == nullptr)
	{
		m_mesh = new Pyramid3D();
	}
	return m_mesh;
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

Mesh3D* Cube3D::GetMesh()
{
	if (m_mesh == nullptr)
	{
		m_mesh = new Cube3D();
	}
	return m_mesh;
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
	glVertexAttribPointer(1, 2, GL_FLOAT, false, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 3, GL_FLOAT, false, 8 * sizeof(GLfloat), (GLvoid*)(5 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

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

Mesh3D* Sphere3D::GetMesh(float Radius, int Fidelity)
{
	if (m_mesh == nullptr)
	{
		m_mesh = new Sphere3D(Radius, Fidelity);
	}
	return m_mesh;
}

Mesh3D* Cube3D::m_mesh = nullptr;
Mesh3D* Pyramid3D::m_mesh = nullptr;
Mesh3D* Sphere3D::m_mesh = nullptr;
Mesh3D* Quad3D::m_mesh = nullptr;

Quad3D::Quad3D()
{
	m_verticesCount = sizeof(m_indices) / sizeof(int32_t);



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
	glVertexAttribPointer(1, 2, GL_FLOAT, false, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 3, GL_FLOAT, false, 8 * sizeof(GLfloat), (GLvoid*)(5 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);
}

Mesh3D* Quad3D::GetMesh()
{
	if (m_mesh == nullptr)
	{
		m_mesh = new Quad3D();
	}
	return m_mesh;
}

std::map<std::string, Terrain3D*> Terrain3D::m_terrains;

void Terrain3D::LoadFromRaw(std::string _name, int _size)
{
	Terrain3D* newTerr = new Terrain3D(_name, _size);
	Terrain3D::m_terrains.emplace(_name, newTerr);
}

Mesh3D* Terrain3D::GetTerrainMesh(std::string _name)
{
	return Terrain3D::m_terrains.find(_name)->second;
}

Terrain3D::Terrain3D(std::string _name, int _size)
{
	int VertexAttrib = 8;	// Float components are needed for each vertex point
	int IndexPerQuad = 6;	// Indices needed to create a quad
	std::string Path = "Resources/Terrain/";
	
	std::vector<unsigned char> data(_size * _size);
	
	std::ifstream rawFile;
	rawFile.open((Path + _name).c_str(), std::ios_base::binary);


	

	

	// Create the vertex array to hold the correct number of elements based on the fidelity of the sphere
	int VertexCount = _size * _size * VertexAttrib;
	int TerrainPointCount = _size * _size;
	float* Heights = new float[TerrainPointCount];


	rawFile.read((char*)&data[0],(std::streamsize)data.size());
	rawFile.close();



	GLfloat* Vertices = new GLfloat[VertexCount];
	int Element = 0;
	int terrainPoint = 0;
	// Each cycle moves down on the vertical (Y axis) to start the next ring
	for (int i = 0; i < _size; i++)
	{
		

		// Creates a horizontal ring and adds each new vertex point to the vertex array
		for (int j = 0; j < _size; j++)
		{
			// Calculate the new vertex position point with the new angles
			float x = (float)j - (_size/2);
			float y = (float)data[terrainPoint++];
			float z = (float)i - (_size / 2);

			// Set the position of the current vertex point
			Vertices[Element++] = x;
			Vertices[Element++] = y;
			Vertices[Element++] = z;

			// Set the texture coordinates of the current vertex point
			Vertices[Element++] = (float)i / (_size - 1);
			Vertices[Element++] = 1 - ((float)j / (_size - 1)); // 1 minus in order to flip the direction of 0-1 (0 at the bottom)

			// Set the normal direction of the current vertex point
			Vertices[Element++] = x;
			Vertices[Element++] = y;
			Vertices[Element++] = z;

			
		}

	}

	// Create the index array to hold the correct number of elements based on the fidelity of the sphere
	int IndexCount = (TerrainPointCount-1) * IndexPerQuad;
	GLuint* Indices = new GLuint[IndexCount];

	Element = 0;	// Reset the element offset for the new array
	for (int gridY = 0; gridY < (_size-1); ++gridY)
	{
		for (int gridX = 0; gridX < (_size - 1); ++gridX)
		{
			int start = gridY * _size + gridX;
			// First triangle of the quad
			Indices[Element++] = (GLuint)start;
			Indices[Element++] = (GLuint)(start + _size);
			Indices[Element++] = (GLuint)(start + 1);
			

			// Second triangle of the quad
			Indices[Element++] = (GLuint)(start + 1);
			Indices[Element++] = (GLuint)(start + _size);
			Indices[Element++] = (GLuint)(start + 1 + _size);
			
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

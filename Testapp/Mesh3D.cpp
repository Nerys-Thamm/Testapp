// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
// 
// (c) 2021 Media Design School
//
// File Name   : Mesh3D.cpp
// Description : Implementation file
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
			Vertices[Element++] = ((float)i / (Fidelity - 1));
			Vertices[Element++] = (1 - ((float)j / (Fidelity - 1))); // 1 minus in order to flip the direction of 0-1 (0 at the bottom)

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

// ********************************************************************************
/// <summary>
/// Constructor
/// </summary>
/// <returns></returns>
// ********************************************************************************
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

// ********************************************************************************
/// <summary>
/// Singleton
/// </summary>
/// <returns></returns>
// ********************************************************************************
Mesh3D* Quad3D::GetMesh()
{
	if (m_mesh == nullptr)
	{
		m_mesh = new Quad3D();
	}
	return m_mesh;
}

std::map<std::string, Terrain3D*> Terrain3D::m_terrains;

// ********************************************************************************
/// <summary>
/// Loads a Heightmap from a file
/// </summary>
/// <param name="_name"></param>
/// <param name="_size"></param>
/// <param name="_xScale"></param>
/// <param name="_yScale"></param>
// ********************************************************************************
void Terrain3D::LoadFromRaw(std::string _name, int _size, float _xScale, float _yScale)
{
	Terrain3D* newTerr = new Terrain3D(_name, _size, _xScale, _yScale);
	Terrain3D::m_terrains.emplace(_name, newTerr);
}

// ********************************************************************************
/// <summary>
/// Loads a Heightmap from noise
/// </summary>
/// <param name="_name"></param>
/// <param name="_size"></param>
/// <param name="_xScale"></param>
/// <param name="_yScale"></param>
// ********************************************************************************
void Terrain3D::LoadFromNoise(std::string _name, int _size, float _xScale, float _yScale)
{
	Terrain3D* newTerr = new Terrain3D(_name, _size, _xScale, _yScale, false);
	Terrain3D::m_terrains.emplace(_name, newTerr);
}

// ********************************************************************************
/// <summary>
/// Gets a Terrain Mesh
/// </summary>
/// <param name="_name"></param>
/// <returns></returns>
// ********************************************************************************
Mesh3D* Terrain3D::GetTerrainMesh(std::string _name)
{
	return Terrain3D::m_terrains.find(_name)->second;
}

// ********************************************************************************
/// <summary>
/// Gets a Terrain
/// </summary>
/// <param name="_name"></param>
/// <returns></returns>
// ********************************************************************************
Terrain3D* Terrain3D::GetTerrain(std::string _name)
{
	return Terrain3D::m_terrains.find(_name)->second;
}

// ********************************************************************************
/// <summary>
/// Gets height at a specific index on the heightmap if within range
/// </summary>
/// <param name="_x"></param>
/// <param name="_y"></param>
/// <returns></returns>
// ********************************************************************************
float Terrain3D::GetHeightAt(int _x, int _y)
{
	if (_x < 0 || _y < 0 || _x > (m_size-1) || _y > (m_size-1))
	{
		return -5.0f;
	}
	else
	{
		return m_heightMap[_y * m_size + _x];
	}
}

// ********************************************************************************
/// <summary>
/// Gets the Interpolated heightmap height based on a world position
/// </summary>
/// <param name="_terrainPos"></param>
/// <param name="_terrainRotation"></param>
/// <param name="_queryPos"></param>
/// <returns></returns>
// ********************************************************************************
float Terrain3D::GetHeightFromWorldPos(glm::vec3 _terrainPos, glm::vec3 _terrainRotation, glm::vec3 _queryPos)
{
	//Transform the position and scale of the query position to the object space of the Terrain
	glm::vec3 localQueryPos = (_queryPos - (_terrainPos - glm::vec3(((m_size*m_xScale)/2.0f), 0.0f, ((m_size * m_xScale) /2.0f))));
	localQueryPos = localQueryPos * (1.0f / m_xScale);
	//Transform the rotation of the query position to the object space of the Terrain
	glm::quat rotateToLocal = glm::quat(-_terrainRotation);
	localQueryPos = rotateToLocal * localQueryPos;

	//Gets the 4 whole numbers determining the coordinates of the vertices that form the quad the query position is within
	float xCeil = std::ceilf(localQueryPos.x);
	float xFloor = std::floorf(localQueryPos.x);
	float zCeil = std::ceilf(localQueryPos.z);
	float zFloor = std::floorf(localQueryPos.z);

	//Determine the relative position within the quad
	float xLerpAmount = localQueryPos.x - xFloor;
	float zLerpAmount = localQueryPos.z - zFloor;

	//Use these whole numbers as coordinates to get the heights of surrounding vertices
	float a = GetHeightAt(xFloor, zFloor), b = GetHeightAt(xFloor, zCeil), c = GetHeightAt(xCeil, zCeil), d = GetHeightAt(xCeil, zFloor);

	//Lerp between these heights based on the relative location
	float ab = (a * (1.0f - zLerpAmount)) + (b * zLerpAmount);
	float dc = (d * (1.0f - zLerpAmount)) + (c * zLerpAmount);
	return ((ab * (1.0f - xLerpAmount)) + (dc * xLerpAmount));

}

// ********************************************************************************
/// <summary>
/// Creates a terrain from a file
/// </summary>
/// <param name="_name"></param>
/// <param name="_size"></param>
/// <param name="_xScale"></param>
/// <param name="_yScale"></param>
/// <returns></returns>
// ********************************************************************************
Terrain3D::Terrain3D(std::string _name, int _size, float _xScale, float _yScale, bool _fromFile)
{
	m_xScale = _xScale;
	m_yScale = _yScale;
	m_size = _size;
	int VertexAttrib = 8;	// Float components are needed for each vertex point
	int IndexPerQuad = 6;	// Indices needed to create a quad
	std::string Path = "Resources/Terrain/";
	
	//Create a vector to store the data
	std::vector<unsigned char> data(_size * _size);
	
	if(_fromFile)
	{
		//Open the file in Binary mode
		std::ifstream rawFile;
		rawFile.open((Path + _name).c_str(), std::ios_base::binary);
		//Read the data
		rawFile.read((char*)&data[0],(std::streamsize)data.size());
		rawFile.close();
	}
	else
	{
		//Generate the heightmap data using Perlin Noise
		Noise n = Noise::getInstance();
		n.SetSeed(std::time(NULL));
		n.PopulateHeightMap(data, _size, _size, 8, 0.3f, 0.005f);
		
	}

	// Create the vertex array to hold the correct number of elements
	int VertexCount = _size * _size * VertexAttrib;
	int TerrainPointCount = _size * _size;
	float* Heights = new float[TerrainPointCount];
	m_heightMap = new float[TerrainPointCount];

	//Box Filter the heightmap
	for (int z = 0; z < _size; ++z)
	{
		for (int x = 0; x < _size; ++x)
		{
			float height = 0.0f;
			for (int i = 0; i < 3; ++i)
			{
				for (int j = 0; j < 3; ++j)
				{
					//check if in bounds
					if (x + i - 1 >= 0 && x + i - 1 < _size && z + j - 1 >= 0 && z + j - 1 < _size)
					{
						height += data[(z + j - 1) * _size + (x + i - 1)];
					}
					
				}
			}
			height /= 9.0f;
			Heights[z * _size + x] = height;
		}
	}


	GLfloat* Vertices = new GLfloat[VertexCount];
	int Element = 0;
	int terrainPoint = 0;
	
	//For each point in the heightmap
	for (int i = 0; i < _size; i++)
	{
		
		for (int j = 0; j < _size; j++)
		{
			//Get the position of the vertex by casting the binary data as a float
			float x = ((float)j - (_size/2))*m_xScale;
			float y = m_heightMap[terrainPoint] = (Heights[terrainPoint++]*m_yScale);
			float z = ((float)i - (_size / 2))*m_xScale;



			// Set the position of the current vertex point
			Vertices[Element++] = x;
			Vertices[Element++] = y;
			Vertices[Element++] = z;

			// Set the texture coordinates of the current vertex point
			Vertices[Element++] = (float)j / 10 * m_xScale;
			Vertices[Element++] = (float)i / 10 * m_xScale; 

			// Set the normal direction of the current vertex point
			if (j <= 1 || i <= 1 || j == (_size - 1) || i == (_size - 1))
			{
				Vertices[Element++] = x;
				Vertices[Element++] = y;
				Vertices[Element++] = z;
			}
			else
			{
				//Get the normal
				float t = m_heightMap[(i - 1) * _size + j];
				float b = m_heightMap[(i + 1) * _size + j];
				float l = m_heightMap[i * _size + j - 1];
				float r = m_heightMap[i * _size + j + 1];

				glm::vec3 tanZ(0.0f, (t - b) * 0.5f, 1.0f);
				glm::vec3 tanX(1.0f, (r - l) * 0.5f, 0.0f);

				glm::vec3 N = glm::cross(tanZ, tanX);
				N = glm::normalize(N);
				

				Vertices[Element++] = N.x;
				Vertices[Element++] = N.y;
				Vertices[Element++] = N.z;
			}
			
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

// ********************************************************************************
/// <summary>
/// Constructor
/// </summary>
/// <returns></returns>
// ********************************************************************************
Tri3D::Tri3D()
{
	m_verticesCount = sizeof(m_indices) / sizeof(int32_t);



	//Generate and bind vertex array to VAO
	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);

	//Generate and bind EBO
	glGenBuffers(1, &m_EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(m_indices), m_indices, GL_STREAM_DRAW);

	//Generate and bind VBO
	glGenBuffers(1, &m_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(m_vertices), m_vertices, GL_STREAM_DRAW);

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
/// Updates the Vertex array
/// </summary>
/// <param name="_first"></param>
/// <param name="_second"></param>
/// <param name="_third"></param>
// ********************************************************************************
void Tri3D::UpdateVertices(glm::vec3 _first, glm::vec3 _second, glm::vec3 _third)
{
	
	//m_vertices = {
	//	// Index   // Position                     // Texture Coords    //Normals
	//	/* 1 */   _first.x, _first.y, _first.z,     0.0f, 0.0f,          0.0f, 0.0f, 1.0f,            // Top - Left
	//	/* 2 */   _second.x, _second.y, _second.z,  0.0f, 1.0f,          0.0f, 0.0f, 1.0f,            // Bot - Left
	//	/* 3 */   _third.x, _third.y, _third.z,     1.0f, 0.0f,          0.0f, 0.0f, 1.0f,            // Bot - Right

	//};
	
	m_vertices[0] = _first.x;
	m_vertices[1] = _first.y;
	m_vertices[2] = _first.z;

	m_vertices[8] = _second.x;
	m_vertices[9] = _second.y;
	m_vertices[10] = _second.z;

	m_vertices[16] = _third.x;
	m_vertices[17] = _third.y;
	m_vertices[18] = _third.z;
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glInvalidateBufferData(GL_ARRAY_BUFFER);
	glBufferData(GL_ARRAY_BUFFER, sizeof(m_vertices), m_vertices, GL_STREAM_DRAW);
}

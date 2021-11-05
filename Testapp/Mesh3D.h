#pragma once
#include <glew.h>
#include <glfw3.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include "Camera.h"
#include "Lighting.h"
#include <vector>
#define _USE_MATH_DEFINES
#include <cmath>
#include <math.h>
#include <iostream>
#include <string>
#include "SceneManager.h"
#include <fstream>

class Mesh3D
{
public:

	void Render(Camera _camera, GLuint _program, glm::mat4 _modelmat, glm::vec3 _color);
	void Render(Camera _camera, GLuint _program, glm::mat4 _modelmat, std::vector<GLuint> _textures, Material& _material, int _textureindex = 0, int _fadeindex = 1);
	GLuint GetVAO() { return m_VAO; }
	int GetVertexCount() { return m_verticesCount; }
	
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
	int m_verticesCount;
	GLuint m_VAO;

protected:
	glm::vec3 m_color;
	
	
	
};

class Pyramid3D : public Mesh3D
{
public:
	// ********************************************************************************
	/// <summary>
	/// Gets a singleton of the mesh
	/// </summary>
	/// <returns></returns>
	// ********************************************************************************
	static Mesh3D* GetMesh();
private:
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
	static Mesh3D* m_mesh;

};

class Cube3D : public Mesh3D
{
public:
	// ********************************************************************************
	/// <summary>
	/// Gets a singleton of the mesh
	/// </summary>
	/// <returns></returns>
	// ********************************************************************************
	static Mesh3D* GetMesh();
private:
	// ********************************************************************************
	/// <summary>
	/// Constructor
	/// </summary>
	/// <returns></returns>
	// ********************************************************************************
	Cube3D();
	GLfloat m_vertices[264] = {
		// Index   // Position            // Texture Coords  // Normals
				   //Front
		/* 00 */   -0.5f,  0.5f,  0.5f,   0.0f, 1.0f,        0.0f, 0.0f, 1.0f,
		/* 01 */   -0.5f, -0.5f,  0.5f,   0.0f, 0.0f,        0.0f, 0.0f, 1.0f,
		/* 02 */    0.5f, -0.5f,  0.5f,   1.0f, 0.0f,        0.0f, 0.0f, 1.0f,
		/* 03 */    0.5f,  0.5f,  0.5f,   1.0f, 1.0f,        0.0f, 0.0f, 1.0f,
		//Back
		/* 04 */    0.5f,  0.5f, -0.5f,   0.0f, 1.0f,        0.0f, 0.0f,-1.0f,
		/* 05 */    0.5f, -0.5f, -0.5f,   0.0f, 0.0f,        0.0f, 0.0f,-1.0f,
		/* 06 */   -0.5f, -0.5f, -0.5f,   1.0f, 0.0f,        0.0f, 0.0f,-1.0f,
		/* 07 */   -0.5f,  0.5f, -0.5f,   1.0f, 1.0f,        0.0f, 0.0f,-1.0f,
		//Right
		/* 08 */    0.5f,  0.5f,  0.5f,   0.0f, 1.0f,        1.0f, 0.0f, 0.0f,
		/* 09 */    0.5f, -0.5f,  0.5f,   0.0f, 0.0f,        1.0f, 0.0f, 0.0f,
		/* 10 */    0.5f, -0.5f, -0.5f,   1.0f, 0.0f,        1.0f, 0.0f, 0.0f,
		/* 11 */    0.5f,  0.5f, -0.5f,   1.0f, 1.0f,        1.0f, 0.0f, 0.0f,
		//Left
		/* 12 */   -0.5f,  0.5f, -0.5f,   0.0f, 1.0f,       -1.0f, 0.0f, 0.0f,
		/* 13 */   -0.5f, -0.5f, -0.5f,   0.0f, 0.0f,       -1.0f, 0.0f, 0.0f,
		/* 14 */   -0.5f, -0.5f,  0.5f,   1.0f, 0.0f,       -1.0f, 0.0f, 0.0f,
		/* 15 */   -0.5f,  0.5f,  0.5f,   1.0f, 1.0f,       -1.0f, 0.0f, 0.0f,
		//Top
		/* 16 */   -0.5f,  0.5f, -0.5f,   0.0f, 1.0f,        0.0f, 1.0f, 0.0f,
		/* 17 */   -0.5f,  0.5f,  0.5f,   0.0f, 0.0f,        0.0f, 1.0f, 0.0f,
		/* 18 */    0.5f,  0.5f,  0.5f,   1.0f, 0.0f,        0.0f, 1.0f, 0.0f,
		/* 19 */    0.5f,  0.5f, -0.5f,   1.0f, 1.0f,        0.0f, 1.0f, 0.0f,
		//Bottom
		/* 20 */   -0.5f, -0.5f,  0.5f,   0.0f, 1.0f,        0.0f,-1.0f, 0.0f,
		/* 21 */   -0.5f, -0.5f, -0.5f,   0.0f, 0.0f,        0.0f,-1.0f, 0.0f,
		/* 22 */    0.5f, -0.5f, -0.5f,   1.0f, 0.0f,        0.0f,-1.0f, 0.0f,
		/* 23 */    0.5f, -0.5f,  0.5f,   1.0f, 1.0f,        0.0f,-1.0f, 0.0f,
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
	static Mesh3D* m_mesh;
};

class Quad3D : public Mesh3D
{
public:
	// ********************************************************************************
	/// <summary>
	/// Gets a singleton of the mesh
	/// </summary>
	/// <returns></returns>
	// ********************************************************************************
	static Mesh3D* GetMesh();
private:
	// ********************************************************************************
	/// <summary>
	/// Constructor
	/// </summary>
	/// <returns></returns>
	// ********************************************************************************
	Quad3D();
	GLfloat m_vertices[44] = {
		// Index   // Position          // Texture Coords    //Normals
		/* 1 */   -0.5f, 0.5f, 0.0f,    0.0f, 1.0f,          0.0f, 0.0f, 1.0f,            // Top - Left
		/* 2 */   -0.5f,-0.5f, 0.0f,    0.0f, 0.0f,          0.0f, 0.0f, 1.0f,            // Bot - Left
		/* 3 */    0.5f,-0.5f, 0.0f,    1.0f, 0.0f,          0.0f, 0.0f, 1.0f,            // Bot - Right
		/* 4 */    0.5f, 0.5f, 0.0f,    1.0f, 1.0f,          0.0f, 0.0f, 1.0f,            // Top - Right
	};
	GLuint m_indices[6] = {
	0, 1, 2, // First Triangle (TL -> BL -> BR)
	0, 2, 3, // Second Triangle (TL -> BR -> TR)
	};
	static Mesh3D* m_mesh;
};

class Tri3D : public Mesh3D
{
public:
	// ********************************************************************************
	/// <summary>
	/// Constructor
	/// </summary>
	/// <returns></returns>
	// ********************************************************************************
	Tri3D();
	void UpdateVertices(glm::vec3 _first, glm::vec3 _second, glm::vec3 _third, glm::vec3 _normal, glm::vec2 _texCoordA, glm::vec2 _texCoordB, glm::vec2 _texCoordC);
private:
	
	GLfloat m_vertices[24] = {
		// Index   // Position          // Texture Coords    //Normals
		/* 1 */    0.0f, 0.0f, 0.0f,    0.0f, 0.0f,          0.0f, 0.0f, 1.0f,            // Top - Left
		/* 2 */    0.0f,-0.5f, 0.0f,    0.0f, 1.0f,          0.0f, 0.0f, 1.0f,            // Bot - Left
		/* 3 */    0.5f, 0.0f, 0.0f,    1.0f, 0.0f,          0.0f, 0.0f, 1.0f,            // Bot - Right
		
	};
	GLuint m_indices[3] = {
	0, 1, 2, // First Triangle (TL -> BL -> BR)
	};
	
};

class Sphere3D : public Mesh3D
{
public:
	// ********************************************************************************
	/// <summary>
	/// Gets a singleton of the mesh
	/// </summary>
	/// <returns></returns>
	// ********************************************************************************
	static Mesh3D* GetMesh(float Radius, int Fidelity);
private:
	Sphere3D(float Radius, int Fidelity);
	int IndexCount;
	int DrawType;
	static Mesh3D* m_mesh;
};

class Terrain3D : public Mesh3D
{
public:
	static void LoadFromRaw(std::string _name, int _size, float _xScale, float _yScale);
	static Mesh3D* GetTerrainMesh(std::string _name);
	static Terrain3D* GetTerrain(std::string _name);
	float GetHeightAt(int _x, int _y);
	float GetHeightFromWorldPos(glm::vec3 _terrainPos, glm::vec3 _terrainRotation, glm::vec3 _queryPos);
private:
	static std::map<std::string, Terrain3D*> m_terrains;
	Terrain3D(std::string _name, int _size, float _xScale, float _yScale);
	int DrawType;
	float m_xScale, m_yScale;
	float* m_heightMap;
	int m_size;

};
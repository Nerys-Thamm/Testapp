#pragma once
#include <glew.h>
#include <glfw3.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include "Camera.h"
#include "Lighting.h"
#include "GameObject.h"
#include <vector>
#define _USE_MATH_DEFINES
#include <cmath>
#include <math.h>
#include <iostream>
#include <string>
#include "ShaderLoader.h"
#include "TextureLoader.h"

class Skybox
	: CGameObject
{
public:
	Skybox(Camera* _camera, std::string _filepaths[6]);
	void Render();
	GLuint GetCubemap();
private:
	GLfloat m_vertices[72] = {
		// Index   // Position            
				   //Front
		/* 00 */   -0.5f,  0.5f,  0.5f,   
		/* 01 */   -0.5f, -0.5f,  0.5f,   
		/* 02 */    0.5f, -0.5f,  0.5f,   
		/* 03 */    0.5f,  0.5f,  0.5f,   
		//Back
		/* 04 */    0.5f,  0.5f, -0.5f,   
		/* 05 */    0.5f, -0.5f, -0.5f,   
		/* 06 */   -0.5f, -0.5f, -0.5f,   
		/* 07 */   -0.5f,  0.5f, -0.5f,   
		//Right
		/* 08 */    0.5f,  0.5f,  0.5f,   
		/* 09 */    0.5f, -0.5f,  0.5f,   
		/* 10 */    0.5f, -0.5f, -0.5f,   
		/* 11 */    0.5f,  0.5f, -0.5f,   
		//Left
		/* 12 */   -0.5f,  0.5f, -0.5f,   
		/* 13 */   -0.5f, -0.5f, -0.5f,   
		/* 14 */   -0.5f, -0.5f,  0.5f,   
		/* 15 */   -0.5f,  0.5f,  0.5f,   
		//Top
		/* 16 */   -0.5f,  0.5f, -0.5f,   
		/* 17 */   -0.5f,  0.5f,  0.5f,   
		/* 18 */    0.5f,  0.5f,  0.5f,   
		/* 19 */    0.5f,  0.5f, -0.5f,   
		//Bottom
		/* 20 */   -0.5f, -0.5f,  0.5f,   
		/* 21 */   -0.5f, -0.5f, -0.5f,   
		/* 22 */    0.5f, -0.5f, -0.5f,   
		/* 23 */    0.5f, -0.5f,  0.5f,   
	};

	GLuint m_indices[36] = {
	0, 2, 1,
	0, 3, 2,
	4, 6, 5,
	4, 7, 6,
	8, 10, 9,
	8, 11, 10,
	12, 14, 13,
	12, 15, 14,
	16, 18, 17,
	16, 19, 18,
	20, 22, 21,
	20, 23, 22,
	};

	GLuint m_VBO;
	GLuint m_EBO;
	int m_verticesCount;
	GLuint m_VAO;
	Camera* m_Camera;
	GLuint m_Cubemap;
	GLuint m_Program;
	glm::mat4 m_PVM;
	virtual void Update(float _fDeltaTime);
	
};


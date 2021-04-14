// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
// 
// (c) 2021 Media Design School
//
// File Name   : Camera.cpp
// Description : Defines the Camera class
// Author      : Nerys Thamm
// Mail        : nerys.thamm@mds.ac.nz

#include "Camera.h"

Camera::Camera(float _fWindowWidth, float _fWindowHeight, GLfloat& _fcurrtime)
{
	//Set defaults for member variables
	m_projectionMat = glm::ortho(-(_fWindowWidth * 0.5f), _fWindowWidth * 0.5f, -(_fWindowHeight * 0.5f), _fWindowHeight * 0.5f, 0.1f, 100.0f);
	m_viewMat = glm::lookAt(m_cameraPos, m_cameraPos + m_cameraLookDir, m_cameraUpDir);
	m_pCurrentTime = &_fcurrtime;
}

void Camera::Render(Shape2D& _shape, GLuint& _program)
{
	//Calculate the PVM matrix
	glm::mat4 PVMMat = m_projectionMat * m_viewMat * _shape.GetModelMatrix();

	//Bind program and VAO
	glUseProgram(_program);
	glBindVertexArray(_shape.m_VAO);

	//Send Vars to shaders via Uniform
	GLint CurrentTimeLoc = glGetUniformLocation(_program, "CurrentTime");
	glUniform1f(CurrentTimeLoc, *m_pCurrentTime);
	GLint PVMMatLoc = glGetUniformLocation(_program, "PVMMat");
	glUniformMatrix4fv(PVMMatLoc, 1, GL_FALSE, glm::value_ptr(PVMMat));

	//If the shape has textures provided
	if (!_shape.m_textures.empty())
	{
		//Set texture uniforms
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, _shape.m_textures[_shape.m_iTextureIndex]);
		glUniform1i(glGetUniformLocation(_program, "ImageTexture"), 0);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, _shape.m_textures[_shape.m_iFadeIndex]);
		glUniform1i(glGetUniformLocation(_program, "ImageTexture1"), 1);
	}

	//Render the Shape
	glDrawElements(GL_TRIANGLES, _shape.m_verticesCount, GL_UNSIGNED_INT, 0);

	//Unbind assets
	glBindVertexArray(0);
	glUseProgram(0);
}
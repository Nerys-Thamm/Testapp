// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
// 
// (c) 2021 Media Design School
//
// File Name   : TessRenderer.cpp
// Description : Implementation file
// Author      : Nerys Thamm
// Mail        : nerys.thamm@mds.ac.nz
#include "TessRenderer.h"

TessRenderer::TessRenderer(CEntity& _parent) : IBehaviour(_parent)
{
	GLfloat points[] = {
	-1.0f, -1.0f, 0.0f,
	1.0f, -1.0f, 0.0f,
	1.0f, 1.0f, 0.0f,
	-1.0, 1.0, 0.0f
	};
	glPatchParameteri(GL_PATCH_VERTICES, 4); //comment for tri patch
	glGenBuffers(1, &m_vbo);
	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(points), &points, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
	glBindVertexArray(0);

	m_material = nullptr;
	m_shader = NULL;
	m_texture = NULL;
}

void TessRenderer::Render(Camera* _camera)
{
	if (m_shader == NULL)
	{
		return;
	}
	glUseProgram(m_shader);
	glm::mat4 translationMat = glm::translate(glm::mat4(), m_entity.m_globalTransform.position);
	glm::mat4 rotationMat = glm::rotate(glm::mat4(), glm::radians(m_entity.m_globalTransform.rotation.x), glm::vec3(1.0f, 0.0f, 0.0f)) * glm::rotate(glm::mat4(), glm::radians(m_entity.m_globalTransform.rotation.y), glm::vec3(0.0f, 1.0f, 0.0f)) * glm::rotate(glm::mat4(), glm::radians(m_entity.m_globalTransform.rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
	glm::mat4 scaleMat = glm::scale(glm::mat4(), m_entity.m_globalTransform.scale);
	glm::mat4 modelmat = translationMat * rotationMat * scaleMat;

	glm::mat4 mvp = _camera->GetPVM(modelmat);

	GLint vpLoc = glGetUniformLocation(m_shader, "mvp");
	glUniformMatrix4fv(vpLoc, 1, GL_FALSE, glm::value_ptr(mvp));

	GLint InLODLoc = glGetUniformLocation(m_shader, "InnerLOD");
	GLint OutLODLoc = glGetUniformLocation(m_shader, "OuterLOD");

	if (m_useLOD)
	{
		int distance = (int)std::clamp(floorf(glm::distance(_camera->m_cameraPos, m_entity.m_globalTransform.position)/2.0f), 0.0f, 9.0f);

		glUniform1i(InLODLoc, innerLOD[distance]);
		glUniform1i(OutLODLoc, outerLOD[distance]);
	}
	else
	{
		glUniform1i(InLODLoc, 3);
		glUniform1i(OutLODLoc, 5);
	}
	glBindVertexArray(m_vao);
	glDrawArrays(GL_PATCHES, 0, 4);
	glBindVertexArray(0);
}

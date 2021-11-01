#include "GeometryRenderer.h"

GeometryRenderer::GeometryRenderer(CEntity& _parent) : IBehaviour(_parent)
{
	GLfloat points[] = { 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f };
	glBindVertexArray(m_vao);
	glGenBuffers(1, &m_vbo);
	glGenVertexArrays(1, &m_vao);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(points), &points, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), 0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glBindVertexArray(0);

	m_material = nullptr;
	m_shader = NULL;
	m_texture = NULL;
}

void GeometryRenderer::Render(Camera* _camera)
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

	glBindVertexArray(m_vao);
	glDrawArrays(GL_POINTS, 0, 1);
	glBindVertexArray(0);
}

#include "Camera.h"

Camera::Camera(float _fwindowwidth, float _fwindowheight, GLfloat& _fcurrtime)
{
	ProjectionMat = glm::ortho(-(_fwindowwidth * 0.5f), _fwindowwidth * 0.5f, -(_fwindowheight * 0.5f), _fwindowheight * 0.5f, 0.1f, 100.0f);
	ViewMat = glm::lookAt(CameraPos, CameraPos + CameraLookDir, CameraUpDir);
	m_pcurrent_time = &_fcurrtime;
}

void Camera::Render(Shape2D& _shape, GLuint& _program)
{
	glm::mat4 PVMMat = ProjectionMat * ViewMat * _shape.GetModelMatrix();

	//Bind program and VAO
	glUseProgram(_program);
	glBindVertexArray(_shape.m_VAO);

	//Send Vars to shaders via Uniform
	GLint CurrentTimeLoc = glGetUniformLocation(_program, "CurrentTime");
	glUniform1f(CurrentTimeLoc, *m_pcurrent_time);
	GLint PVMMatLoc = glGetUniformLocation(_program, "PVMMat");
	glUniformMatrix4fv(PVMMatLoc, 1, GL_FALSE, glm::value_ptr(PVMMat));

	if (!_shape.m_textures.empty())
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, _shape.m_textures[_shape.m_itextureindex]);
		glUniform1i(glGetUniformLocation(_program, "ImageTexture"), 0);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, _shape.m_textures[_shape.m_ifadeindex]);
		glUniform1i(glGetUniformLocation(_program, "ImageTexture1"), 1);
	}

	//Render the Triangle
	/*glDrawArrays(GL_TRIANGLES, 0, 6);*/
	glDrawElements(GL_TRIANGLES, _shape.m_verticescount, GL_UNSIGNED_INT, 0);

	//Unbind assets
	glBindVertexArray(0);
	glUseProgram(0);
}
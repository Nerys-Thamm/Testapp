#include "MeshRenderer.h"

MeshRenderer::MeshRenderer(CEntity& _parent) : IBehaviour(_parent)
{
	m_material = nullptr;
	m_shader = NULL;
	m_texture = NULL;
	m_mesh = nullptr;
}

void MeshRenderer::Render(Camera* _camera)
{
	if (m_mesh == nullptr || m_shader == NULL)
	{
		return;
	}
	//Calculate model matrix
	glm::mat4 translationMat = glm::translate(glm::mat4(), m_entity.m_globalTransform.position);
	glm::mat4 rotationMat = glm::rotate(glm::mat4(), glm::radians(m_entity.m_globalTransform.rotation.x), glm::vec3(1.0f, 0.0f, 0.0f)) * glm::rotate(glm::mat4(), glm::radians(m_entity.m_globalTransform.rotation.y), glm::vec3(0.0f, 1.0f, 0.0f)) * glm::rotate(glm::mat4(), glm::radians(m_entity.m_globalTransform.rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
	glm::mat4 scaleMat = glm::scale(glm::mat4(), m_entity.m_globalTransform.scale);
	glm::mat4 modelmat = translationMat * rotationMat * scaleMat;
	//Calculate the PVM matrix
	glm::mat4 PVMMat = _camera->GetPVM(modelmat);
	glm::mat4 ModelMat = modelmat;
	//Bind program and VAO
	glUseProgram(m_shader);
	glBindVertexArray(m_mesh->GetVAO());

	//Send Vars to shaders via Uniform

	//Time
	GLint CurrentTimeLoc = glGetUniformLocation(m_shader, "CurrentTime");
	glUniform1f(CurrentTimeLoc, glfwGetTime());

	//Matrices
	GLint PVMMatLoc = glGetUniformLocation(m_shader, "PVMMat");
	glUniformMatrix4fv(PVMMatLoc, 1, GL_FALSE, glm::value_ptr(PVMMat));
	GLint ModelLoc = glGetUniformLocation(m_shader, "Model");
	glUniformMatrix4fv(ModelLoc, 1, GL_FALSE, glm::value_ptr(ModelMat));

	//Camera
	GLint CamLoc = glGetUniformLocation(m_shader, "CameraPos");
	glUniform3fv(CamLoc, 1, glm::value_ptr(_camera->m_cameraPos));

	//Material
	glUniform1f(glGetUniformLocation(m_shader, "Mat[0].Smoothness"), m_material->Smoothness);
	glUniform1f(glGetUniformLocation(m_shader, "Mat[0].Reflectivity"), m_material->Reflectivity);


	//Lighting
	//PointLights
	for (size_t i = 0; i < Lighting::MAX_POINT_LIGHTS; i++)
	{
		glUniform3fv(glGetUniformLocation(m_shader, ("PointLights[" + std::to_string(i) + "].Position").c_str()), 1, glm::value_ptr(Lighting::PointLights[i].Position));
		glUniform3fv(glGetUniformLocation(m_shader, ("PointLights[" + std::to_string(i) + "].Color").c_str()), 1, glm::value_ptr(Lighting::PointLights[i].Color));
		glUniform1f(glGetUniformLocation(m_shader, ("PointLights[" + std::to_string(i) + "].AmbientStrength").c_str()), Lighting::PointLights[i].AmbientStrength);
		glUniform1f(glGetUniformLocation(m_shader, ("PointLights[" + std::to_string(i) + "].SpecularStrength").c_str()), Lighting::PointLights[i].SpecularStrength);
		glUniform1f(glGetUniformLocation(m_shader, ("PointLights[" + std::to_string(i) + "].AttenuationConstant").c_str()), Lighting::PointLights[i].AttenuationConstant);
		glUniform1f(glGetUniformLocation(m_shader, ("PointLights[" + std::to_string(i) + "].AttenuationExponent").c_str()), Lighting::PointLights[i].AttenuationExponent);
		glUniform1f(glGetUniformLocation(m_shader, ("PointLights[" + std::to_string(i) + "].AttenuationLinear").c_str()), Lighting::PointLights[i].AttenuationLinear);
	}

	//DirectionalLights
	for (size_t i = 0; i < Lighting::MAX_DIRECTIONAL_LIGHTS; i++)
	{
		glUniform3fv(glGetUniformLocation(m_shader, ("DirectionalLights[" + std::to_string(i) + "].Direction").c_str()), 1, glm::value_ptr(Lighting::DirectionalLights[i].Direction));
		glUniform3fv(glGetUniformLocation(m_shader, ("DirectionalLights[" + std::to_string(i) + "].Color").c_str()), 1, glm::value_ptr(Lighting::DirectionalLights[i].Color));
		glUniform1f(glGetUniformLocation(m_shader, ("DirectionalLights[" + std::to_string(i) + "].AmbientStrength").c_str()), Lighting::DirectionalLights[i].AmbientStrength);
		glUniform1f(glGetUniformLocation(m_shader, ("DirectionalLights[" + std::to_string(i) + "].SpecularStrength").c_str()), Lighting::DirectionalLights[i].SpecularStrength);
	}


	if (m_texture != NULL)
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_texture);
		glUniform1i(glGetUniformLocation(m_shader, "ImageTexture"), 0);
	}
	/*if (m_material->AlbedoMap != NULL)
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_material->AlbedoMap);
		glUniform1i(glGetUniformLocation(m_shader, "ImageTexture"), 0);
	}

	if (m_material->ReflectionMap != NULL)
	{
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, m_material->ReflectionMap);
		glUniform1i(glGetUniformLocation(m_shader, "ReflectionMap"), 2);
	}*/

	
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_CUBE_MAP, SceneManager::GetCurrentSkybox()->GetCubemap());
	glUniform1i(glGetUniformLocation(m_shader, "CubeMap"), 1);


	//Render the Shape
	glDrawElements(GL_TRIANGLES, m_mesh->GetVertexCount(), GL_UNSIGNED_INT, 0);

	//Unbind assets
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	glUseProgram(0);
}

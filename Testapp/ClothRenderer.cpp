#include "ClothRenderer.h"

ClothRenderer::ClothRenderer(CEntity& _parent) : IBehaviour(_parent)
{
	m_material = nullptr;
	m_shader = NULL;
	m_texture = NULL;
	m_cloth = nullptr;
}

void ClothRenderer::Render(Camera* _camera)
{
	if (m_cloth == nullptr || m_shader == NULL)
	{
		return;
	}
	glDisable(GL_CULL_FACE);
	//Calculate model matrix
	glm::mat4 translationMat = glm::translate(glm::mat4(), m_entity.m_globalTransform.position);
	glm::mat4 rotationMat = glm::rotate(glm::mat4(), glm::radians(m_entity.m_globalTransform.rotation.x), glm::vec3(1.0f, 0.0f, 0.0f)) * glm::rotate(glm::mat4(), glm::radians(m_entity.m_globalTransform.rotation.y), glm::vec3(0.0f, 1.0f, 0.0f)) * glm::rotate(glm::mat4(), glm::radians(m_entity.m_globalTransform.rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
	glm::mat4 scaleMat = glm::scale(glm::mat4(), m_entity.m_globalTransform.scale);
	glm::mat4 modelmat = translationMat * rotationMat * scaleMat;
	//Calculate the PVM matrix
	glm::mat4 PVMMat = _camera->GetPVM(modelmat);
	glm::mat4 ModelMat = modelmat;
	for (int i = 0; i < m_tris.size(); i++)
	{


		//Bind program and VAO
		glUseProgram(m_shader);
		glBindVertexArray(m_tris[i]->GetVAO());

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
		


		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_CUBE_MAP, SceneManager::GetCurrentSkybox()->GetCubemap());
		glUniform1i(glGetUniformLocation(m_shader, "CubeMap"), 1);


		//Render the Shape
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);

		//Unbind assets
		glBindVertexArray(0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
		glUseProgram(0);
	}
	glEnable(GL_CULL_FACE);
}

void ClothRenderer::OnUpdate(float _fDeltaTime)
{
	
	if (m_cloth != nullptr)
	{
		m_cloth->Update(_fDeltaTime);
		if (m_tris.size() == 0)
		{

			for (int gridY = 0; gridY < (m_cloth->m_particleDensity.y - 1); ++gridY)
			{
				for (int gridX = 0; gridX < (m_cloth->m_particleDensity.x - 1); ++gridX)
				{


					m_tris.push_back(std::unique_ptr<Tri3D>(new Tri3D()));
					m_tris.push_back(std::unique_ptr<Tri3D>(new Tri3D()));


				}
			}

		}
		else
		{
			int Element = 0;
			int IndexCount = (m_cloth->GetSize() - 1) * 4;
			int* Indices = new int[IndexCount];
			for (int gridY = 0; gridY < (m_cloth->m_particleDensity.y - 1); ++gridY)
			{
				for (int gridX = 0; gridX < (m_cloth->m_particleDensity.x - 1); ++gridX)
				{
					int start = gridY * (m_cloth->m_particleDensity.x) + gridX;
					// First triangle of the quad
					Indices[Element++] = (int)start;
					Indices[Element++] = (int)(start + (m_cloth->m_particleDensity.x));
					Indices[Element++] = (int)(start + 1);

					// Second triangle of the quad
					Indices[Element++] = (int)(start + 1);
					Indices[Element++] = (int)(start + m_cloth->m_particleDensity.x);
					Indices[Element++] = (int)(start + 1 + m_cloth->m_particleDensity.x);

				}
			}
			Element = 0;
			for (int i = 0; i < m_tris.size(); i++)
			{
				m_tris[i]->UpdateVertices(
					m_cloth->GetParticlePositionAtIndex(Indices[Element++]),
					m_cloth->GetParticlePositionAtIndex(Indices[Element++]),
					m_cloth->GetParticlePositionAtIndex(Indices[Element++])
				);
			}
		}
	}
}

void ClothRenderer::OnFixedUpdate()
{
	m_cloth->FixedUpdate();
}

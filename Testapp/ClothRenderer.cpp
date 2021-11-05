// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
// 
// (c) 2021 Media Design School
//
// File Name   : ClothRenderer.cpp
// Description : Implementation file
// Author      : Nerys Thamm
// Mail        : nerys.thamm@mds.ac.nz
#include "ClothRenderer.h"

// ********************************************************************************
/// <summary>
/// Constructor
/// </summary>
/// <param name="_parent"></param>
/// <returns></returns>
// ********************************************************************************
ClothRenderer::ClothRenderer(CEntity& _parent) : IBehaviour(_parent)
{
	m_material = nullptr;
	m_shader = NULL;
	m_texture = NULL;
	m_cloth = nullptr;
}

// ********************************************************************************
/// <summary>
/// Destructor
/// </summary>
/// <returns></returns>
// ********************************************************************************
ClothRenderer::~ClothRenderer()
{
	delete m_cloth;

}

// ********************************************************************************
/// <summary>
/// Renders the cloth
/// </summary>
/// <param name="_camera"></param>
// ********************************************************************************
void ClothRenderer::Render(Camera* _camera)
{
	if (m_cloth == nullptr || m_shader == NULL) //Return immediately if any required variables are null
	{
		return;
	}

	glDisable(GL_CULL_FACE); //Disable backface culling to make the cloth two sided

	//Calculate model matrix
	glm::mat4 translationMat = glm::translate(glm::mat4(), m_entity.m_globalTransform.position);
	glm::mat4 rotationMat = glm::rotate(glm::mat4(), glm::radians(m_entity.m_globalTransform.rotation.x), glm::vec3(1.0f, 0.0f, 0.0f)) * glm::rotate(glm::mat4(), glm::radians(m_entity.m_globalTransform.rotation.y), glm::vec3(0.0f, 1.0f, 0.0f)) * glm::rotate(glm::mat4(), glm::radians(m_entity.m_globalTransform.rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
	glm::mat4 scaleMat = glm::scale(glm::mat4(), m_entity.m_globalTransform.scale);
	glm::mat4 modelmat = translationMat * rotationMat * scaleMat;
	//Calculate the PVM matrix
	glm::mat4 PVMMat = _camera->GetPVM(modelmat);
	glm::mat4 ModelMat = modelmat;
	glUseProgram(m_shader);
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
	for (int i = 0; i < (int)m_tris.size(); i++)
	{


		//Bind program and VAO
		
		glBindVertexArray(m_tris[i]->GetVAO());

		//Send Vars to shaders via Uniform

		//Time
		GLint CurrentTimeLoc = glGetUniformLocation(m_shader, "CurrentTime");
		glUniform1f(CurrentTimeLoc, (GLfloat)glfwGetTime());

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
		
	}
	glUseProgram(0);
	glEnable(GL_CULL_FACE);
}

// ********************************************************************************
/// <summary>
/// Sets the Cloth for the ClothRenderer to use
/// </summary>
/// <param name="_cloth"></param>
// ********************************************************************************
void ClothRenderer::SetCloth(Cloth* _cloth)
{
	if (m_cloth != nullptr) delete m_cloth; //Delete any existing Cloth data then use the new one
	m_tris.clear();
	m_indices.clear();
	m_cloth = _cloth;
}

// ********************************************************************************
/// <summary>
/// Updates the Vertices of the Tri's used to draw the cloth
/// </summary>
/// <param name="_fDeltaTime"></param>
// ********************************************************************************
void ClothRenderer::OnUpdate(float _fDeltaTime)
{
	
	if (m_cloth != nullptr) //If the cloth is not null
	{
		m_cloth->Update(_fDeltaTime); //Update the cloth

		if (m_tris.size() == 0) //If there are no Tris, make them
		{
			//Create the Tris
			for (int gridY = 0; gridY < (m_cloth->m_particleDensity.y - 1); ++gridY)
			{
				for (int gridX = 0; gridX < (m_cloth->m_particleDensity.x - 1); ++gridX)
				{
					m_tris.push_back(std::unique_ptr<Tri3D>(new Tri3D()));
					m_tris.push_back(std::unique_ptr<Tri3D>(new Tri3D()));
				}
			}
			//Set the indices of the Tri's within the overall cloth
			int Element = 0;
			int IndexCount = (m_tris.size()) * 3;
			m_indices.resize(IndexCount);
			for (int gridY = 0; gridY < (m_cloth->m_particleDensity.y - 1); ++gridY)
			{
				for (int gridX = 0; gridX < (m_cloth->m_particleDensity.x - 1); ++gridX)
				{
					int start = gridY * (m_cloth->m_particleDensity.x) + gridX;
					// First triangle of the quad
					m_indices[Element++] = (int)start;
					m_indices[Element++] = (int)(start + (m_cloth->m_particleDensity.x));
					m_indices[Element++] = (int)(start + 1);

					// Second triangle of the quad
					m_indices[Element++] = (int)(start + 1);
					m_indices[Element++] = (int)(start + m_cloth->m_particleDensity.x);
					m_indices[Element++] = (int)(start + 1 + m_cloth->m_particleDensity.x);

				}
			}
		}
		else //Otherwise, if there are already Tris
		{
			
			//Update the position and normal of all vertices based on particle positions
			int Element = 0;
			for (int i = 0; i < (int)m_tris.size(); i++)
			{

				int indexA = m_indices[Element++];
				int indexB = m_indices[Element++];
				int indexC = m_indices[Element++];
				glm::vec3 a = m_cloth->GetParticlePositionAtIndex(indexA);
				glm::vec3 b = m_cloth->GetParticlePositionAtIndex(indexB);
				glm::vec3 c = m_cloth->GetParticlePositionAtIndex(indexC);
				m_tris[i]->UpdateVertices(
					a,
					b,
					c,
					m_cloth->GetTriNormal(indexA, indexB, indexC),
					glm::vec2((float)(indexA % m_cloth->m_particleDensity.x)/ m_cloth->m_particleDensity.x, 1.0f-(floor(indexA / m_cloth->m_particleDensity.x))/ m_cloth->m_particleDensity.y),
					glm::vec2((float)(indexB % m_cloth->m_particleDensity.x)/ m_cloth->m_particleDensity.x, 1.0f-(floor(indexB / m_cloth->m_particleDensity.x))/ m_cloth->m_particleDensity.y),
					glm::vec2((float)(indexC % m_cloth->m_particleDensity.x)/ m_cloth->m_particleDensity.x, 1.0f-(floor(indexC / m_cloth->m_particleDensity.x))/ m_cloth->m_particleDensity.y)
				);
			}
			
		}
	}
}

// ********************************************************************************
/// <summary>
/// Update the cloth at a fixed timestep
/// </summary>
// ********************************************************************************
void ClothRenderer::OnFixedUpdate()
{
	if (m_cloth != nullptr)
	{
		m_cloth->FixedUpdate();
	}
}

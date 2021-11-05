#pragma once
#include "Mesh3D.h"
#include "GameObject.h"
#include <vector>
#define _USE_MATH_DEFINES
#include <cmath>
#include <math.h>
#include "Lighting.h"
class Renderable3D:
	public CGameObject
{
private:
	glm::vec3 m_position;
	float m_fRotationX;
	float m_fRotationY;
	float m_fRotationZ;
	glm::vec3 m_scale;
	glm::vec3 m_color;
	int m_iTextureIndex;
	int m_iFadeIndex;
	std::vector<GLuint> m_textures;
	glm::mat4 m_translationMat;
	glm::mat4 m_rotationMat;
	glm::mat4 m_scaleMat;
	Mesh3D* m_mesh;
	Material* m_material;
public:

	//GETTERS AND SETTERS-----------
	glm::vec3 Position();
	glm::vec3 Rotation();
	glm::vec3 Scale();
	glm::vec3 Color();
	
	void Position(glm::vec3 _pos);
	void Rotation(glm::vec3 _rot);
	void Scale(glm::vec3 _scl);
	void Color(glm::vec3 _col);
	//-----------------------------


	// ********************************************************************************
	/// <summary>
	/// Renders the attached mesh
	/// </summary>
	/// <param name="_camera"></param>
	/// <param name="_program"></param>
	// ********************************************************************************
	void Render(Camera _camera, GLuint _program);


	// ********************************************************************************
	/// <summary>
	/// Constructor
	/// </summary>
	/// <returns></returns>
	// ********************************************************************************
	Renderable3D(Mesh3D* _mesh, Material* _material);
	
	// ********************************************************************************
	/// <summary>
	/// Adds a texture to the texture list
	/// </summary>
	/// <param name="_texture">The texture to add</param>
	// ********************************************************************************
	void AddTexture(GLuint _texture);

	// ********************************************************************************
	/// <summary>
	/// Adds multiple textures to the texture list
	/// </summary>
	/// <param name="_textures">A vector of textures to add</param>
	// ********************************************************************************
	void AddTextures(std::vector<GLuint>& _textures);

private:
	// ********************************************************************************
	/// <summary>
	/// Computes and returns the model matrix for the shape
	/// </summary>
	/// <returns>The model matrix for the shape as mat4</returns>
	// ********************************************************************************
	glm::mat4 GetModelMatrix();

};


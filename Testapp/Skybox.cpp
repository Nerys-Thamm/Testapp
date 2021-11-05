#include "Skybox.h"

// ********************************************************************************
/// <summary>
/// Constructor
/// </summary>
/// <param name="_camera">Camera to use to render the skybox</param>
/// <param name="_filepaths">File paths of the cubemap images</param>
/// <returns></returns>
// ********************************************************************************
Skybox::Skybox(Camera* _camera, std::string _filepaths[6])
{
	//Set camera and program
	m_Camera = _camera;
	m_Program = ShaderLoader::CreateProgram("Resources/Shaders/SkyBox.vs",
		"Resources/Shaders/SkyBox.fs");

	//Load the cubemap
	m_Cubemap = TextureLoader::LoadCubeMap(_filepaths);

	//Set vertices stuff
	m_verticesCount = sizeof(m_indices) / sizeof(int32_t);

	//Generate and bind vertex array to VAO
	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);

	//Generate and bind EBO
	glGenBuffers(1, &m_EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(m_indices), m_indices, GL_STATIC_DRAW);

	//Generate and bind VBO
	glGenBuffers(1, &m_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(m_vertices), m_vertices, GL_STATIC_DRAW);

	//Set attribute pointers
	glVertexAttribPointer(0, 3, GL_FLOAT, false, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	
}

// ********************************************************************************
/// <summary>
/// This is called every frame
/// </summary>
/// <param name="_fDeltaTime"></param>
// ********************************************************************************
void Skybox::Update(float _fDeltaTime)
{
	glm::mat4 ModelMat = glm::scale(glm::mat4(), glm::vec3(2000.0f, 2000.0f, 2000.0f));
	m_PVM = m_Camera->GetPVM(ModelMat);
}

// ********************************************************************************
/// <summary>
/// Renders the skybox
/// </summary>
// ********************************************************************************
void Skybox::Render()
{
	//Bind program and VAO
	glUseProgram(m_Program);
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

	glActiveTexture(GL_TEXTURE);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_Cubemap);


	//Texture and Matrix Uniforms
	GLint TexLoc = glGetUniformLocation(m_Program, "Texture");
	glUniform1i(TexLoc, m_Cubemap);
	GLint PVMLoc = glGetUniformLocation(m_Program, "PVM");
	glUniformMatrix4fv(PVMLoc, 1, GL_FALSE, glm::value_ptr(m_PVM));

	//Render the Shape
	glBindVertexArray(m_VAO);
	glDrawElements(GL_TRIANGLES, m_verticesCount, GL_UNSIGNED_INT, 0);
	

	//Unbind assets
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	glUseProgram(0);
}

// ********************************************************************************
/// <summary>
/// Getter for Cubemap texture
/// </summary>
/// <returns></returns>
// ********************************************************************************
GLuint Skybox::GetCubemap()
{
	return m_Cubemap;
}

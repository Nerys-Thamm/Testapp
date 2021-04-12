// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
// 
// (c) 2021 Media Design School
//
// File Name   : 
// Description : 
// Author      : Nerys Thamm
// Mail        : nerys.thamm@mds.ac.nz


//Lib includes
#include <iostream>
#include <glew.h>
#include <glfw3.h>
#include "ShaderLoader.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "Shape2D.h"
#include "Camera.h"

//Pointer to window
GLFWwindow* main_window = nullptr;

void InitialSetup();
void Update();
void Render();

GLuint program_texture;

GLuint program_worldspace;

GLuint program_texture_interpolation;

GLuint program_fixed_color;

float current_time;
float delta_time;
float timer;

Camera* camera = nullptr;

Hex2D* shape_hex = nullptr;

Quad2D* shape_quad = nullptr;

int main()
{
	//Init GLFW and setting ver to 4.6 with only core functionality
	glfwInit();
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);

	//Create a GLFW controlled context window
	main_window = glfwCreateWindow(800, 800, "First OpenGL Window", NULL, NULL);
	if (main_window == NULL)
	{
		std::cout << "GLFW failed to initialize properly. Terminating program." << std::endl;
		system("pause");

		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(main_window);

	//Init GLEW to populate OpenGL func pointers
	if (glewInit() != GLEW_OK)
	{
		std::cout << "GLEW failed to initialize properly. Terminating program." << std::endl;
		system("pause");

		glfwTerminate();
		return -1;
	}

	//Setup initial elements of program
	InitialSetup();

	//Main Loop
	while (glfwWindowShouldClose(main_window) == false)
	{
		//Update all objects and run the processes
		Update();

		//Render all the objects
		Render();
	}

	// Ensuring correct shutdown of GLFW
	glfwTerminate();
	return 0;
}

GLuint LoadTexture(std::string _filename)
{
	GLuint out;

	// Load the Image data
	int ImageWidth;
	int ImageHeight;
	int ImageComponents;
	unsigned char* ImageData = stbi_load(("Resources/Textures/" + _filename).c_str(), &ImageWidth, &ImageHeight, &ImageComponents, 0);

	//Create and bind a new texture template
	glGenTextures(1, &out);
	glBindTexture(GL_TEXTURE_2D, out);

	// Check how many components the loaded image has (rgb or rgba)
	GLint LoadedComponents = (ImageComponents == 4) ? GL_RGBA : GL_RGB;

	// Populate the texture with image data
	glTexImage2D(GL_TEXTURE_2D, 0, LoadedComponents, ImageWidth, ImageHeight, 0, LoadedComponents, GL_UNSIGNED_BYTE, ImageData);

	// Generate mipmaps, free memory and unbind texture
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(ImageData);
	glBindTexture(GL_TEXTURE_2D, 0);
	return out;
}

//Setup initial elements of program
void InitialSetup()
{
	stbi_set_flip_vertically_on_load(true);

	current_time = 0;
	delta_time = 0;
	timer = 1 / 24;

	// Set window colour for when buffer is cleared
	glClearColor(1.0f, 0.0f, 0.0f, 1.0f);

	// Map range of the window size to NDC (-1 -> 1)
	glViewport(0, 0, 800, 800);

	program_texture = ShaderLoader::CreateProgram("Resources/Shaders/ClipSpace.vs",
		"Resources/Shaders/Texture.fs");

	program_texture_interpolation = ShaderLoader::CreateProgram("Resources/Shaders/ClipSpace.vs",
		"Resources/Shaders/TextureMix.fs");

	program_fixed_color = ShaderLoader::CreateProgram("Resources/Shaders/ClipSpace.vs",
		"Resources/Shaders/FixedColor.fs");

	camera = new Camera(800, 800, current_time);

	shape_hex = new Hex2D();
	
	shape_quad = new Quad2D();

	shape_hex->SetTexture(LoadTexture("Rayman.jpg"));
	shape_hex->SetTexture(LoadTexture("AwesomeFace.png"));
	shape_hex->m_iFadeIndex = 1;

	shape_quad->SetTexture(LoadTexture("Walk1.png"));
	shape_quad->SetTexture(LoadTexture("Walk2.png"));
	shape_quad->SetTexture(LoadTexture("Walk3.png"));
	shape_quad->SetTexture(LoadTexture("Walk4.png"));
	shape_quad->SetTexture(LoadTexture("Walk5.png"));
	shape_quad->SetTexture(LoadTexture("Walk6.png"));
	shape_quad->SetTexture(LoadTexture("Walk7.png"));
	shape_quad->SetTexture(LoadTexture("Walk8.png"));

	shape_hex->m_position = glm::vec3(-200.0f, 0.0f, 0.0f);
	shape_hex->m_scale = glm::vec3(100.0f, 100.0f, 0.0f);
	
	
	shape_quad->m_position = glm::vec3(200.0f, 0.0f, 0.0f);
	shape_quad->m_scale = glm::vec3(100.0f, 200.0f, 0.0f);
}

//Update all objects and run the processes
void Update()
{
	glfwPollEvents();

	//Get the current time
	delta_time = current_time;
	current_time = (float)glfwGetTime();
	delta_time = current_time - delta_time;

	if (timer <= 0)
	{
		if ((size_t)shape_quad->m_iFadeIndex >= shape_quad->m_textures.size() - 1)
		{
			shape_quad->m_iTextureIndex = 0;
			shape_quad->m_iFadeIndex = 1;
		}
		else
		{
			shape_quad->m_iTextureIndex++;
			shape_quad->m_iFadeIndex++;
		}
		timer = 0.04f;
	}
	timer -= delta_time;

	glClearColor(((sin(current_time) + 1.0f) * 0.5f), ((sin(current_time + 2.0f) + 0.5f) * 0.5f), ((sin(current_time + 4.0f) + 1.0f) * 0.5f), 1.0f);
}

//Render all objects
void Render()
{
	//Clear the buffer
	glClear(GL_COLOR_BUFFER_BIT);
	shape_hex->m_position = glm::vec3(-200.0f, 0.0f, 0.0f);
	camera->Render(*shape_hex, program_texture_interpolation);
	shape_hex->m_position = glm::vec3(0.0f, 0.0f, 0.0f);
	camera->Render(*shape_hex, program_texture_interpolation);
	camera->Render(*shape_quad, program_texture_interpolation);

	//Push buffer to the screen
	glfwSwapBuffers(main_window);
}
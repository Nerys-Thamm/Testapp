// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
// 
// (c) 2021 Media Design School
//
// File Name   : Testapp.cpp
// Description : Main file for OpenGL application
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
#include "Config.h"
#include "TextLabel.h"

//Pointer to window
GLFWwindow* main_window = nullptr;

void InitialSetup();
void Update();
void Render();

GLuint program_texture;

GLuint program_worldspace;

GLuint program_texture_interpolation;

GLuint program_fixed_color;

GLuint program_texture_wave;

float current_time;
float delta_time;
float timer;

Camera* camera = nullptr;

Hex2D* shape_hex = nullptr;

Quad2D* shape_quad = nullptr;

Quad2D* shape_quad2 = nullptr;

TextLabel* text_message = nullptr;

int main()
{
	std::cout << "Program compiled " << __DATE__ << " | " << __TIME__ << std::endl;
	//Init GLFW and setting ver to 4.6 with only core functionality
	glfwInit();
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);

	//Create a GLFW controlled context window
	main_window = glfwCreateWindow(cfWINDOW_WIDTH(), cfWINDOW_HEIGHT(), "Nerys Thamm OpenGL Summative", NULL, NULL);
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
	glViewport(0, 0, cfWINDOW_WIDTH(), cfWINDOW_HEIGHT());

	//Create programs
	program_texture = ShaderLoader::CreateProgram("Resources/Shaders/ClipSpace.vs",
		"Resources/Shaders/Texture.fs");

	program_texture_interpolation = ShaderLoader::CreateProgram("Resources/Shaders/ClipSpace.vs",
		"Resources/Shaders/TextureMix.fs");

	program_fixed_color = ShaderLoader::CreateProgram("Resources/Shaders/ClipSpace.vs",
		"Resources/Shaders/FixedColor.fs");

	program_texture_wave = ShaderLoader::CreateProgram("Resources/Shaders/ClipSpace.vs",
		"Resources/Shaders/TextureWave.fs");

	//Create camera
	camera = new Camera(cfWINDOW_WIDTH(), cfWINDOW_HEIGHT(), current_time);

	//Create objects
	shape_hex = new Hex2D();
	
	shape_quad = new Quad2D();

	shape_quad2 = new Quad2D();

	//Create Text

	text_message = new TextLabel("Super spicy text!", "Resources/Fonts/ARIAL.ttf", glm::ivec2(0, 48), glm::vec2(100.0f, 100.0f));


	//Set textures of objects
	shape_hex->AddTexture(LoadTexture("Rayman.jpg"));
	shape_hex->AddTexture(LoadTexture("AwesomeFace.png"));
	shape_hex->m_iFadeIndex = 1;

	shape_quad->AddTexture(LoadTexture("Walk1.png"));
	shape_quad->AddTexture(LoadTexture("Walk2.png"));
	shape_quad->AddTexture(LoadTexture("Walk3.png"));
	shape_quad->AddTexture(LoadTexture("Walk4.png"));
	shape_quad->AddTexture(LoadTexture("Walk5.png"));
	shape_quad->AddTexture(LoadTexture("Walk6.png"));
	shape_quad->AddTexture(LoadTexture("Walk7.png"));
	shape_quad->AddTexture(LoadTexture("Walk8.png"));

	shape_quad2->AddTexture(LoadTexture("Rayman.jpg"));

	//Set initiaal position of objects
	shape_hex->m_position = glm::vec3(-200.0f, 0.0f, 0.0f);
	shape_hex->m_scale = glm::vec3(100.0f, 100.0f, 0.0f);
	
	
	shape_quad->m_position = glm::vec3(200.0f, 0.0f, 0.0f);
	shape_quad->m_scale = glm::vec3(100.0f, 200.0f, 0.0f);

	shape_quad2->m_position = glm::vec3(0.0f, -200.0f, 0.0f);
	shape_quad2->m_scale = glm::vec3(100.0f, 100.0f, 0.0f);
}

//Update all objects and run the processes
void Update()
{
	if (cfFLAG("Spam_me_please"))
	{
		std::cout << "SPAM ";
	}

	glfwPollEvents();

	//Get the current time
	delta_time = current_time;
	current_time = (float)glfwGetTime();
	delta_time = current_time - delta_time;

	//Animate the texture
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

	//Rainbow background
	glClearColor(((sin(current_time) + 1.0f) * 0.5f), ((sin(current_time + 2.0f) + 0.5f) * 0.5f), ((sin(current_time + 4.0f) + 1.0f) * 0.5f), 1.0f);
}

//Render all objects
void Render()
{
	//Clear the buffer
	glClear(GL_COLOR_BUFFER_BIT);
	//Set position of shape and render it
	shape_hex->m_position = glm::vec3(-200.0f, 0.0f, 0.0f);
	//Draw the shape
	camera->Render(*shape_hex, program_texture_interpolation);
	//Set position of shape to second position and render it
	shape_hex->m_position = glm::vec3(0.0f, 0.0f, 0.0f);
	//Draw the shape
	camera->Render(*shape_hex, program_texture_interpolation);
	//Render the animated quad
	camera->Render(*shape_quad, program_texture_interpolation);
	//Render the Quad with the distorted texture
	camera->Render(*shape_quad2, program_texture_wave);

	text_message->Render();

	//Push buffer to the screen
	glfwSwapBuffers(main_window);
}
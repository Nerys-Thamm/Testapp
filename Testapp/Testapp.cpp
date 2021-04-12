// Testapp.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

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
GLFWwindow* Window = nullptr;

void InitialSetup();
void Update();
void Render();

GLuint Program_Texture;

GLuint Program_WorldSpace;

GLuint Program_TwoTextureInterpolation;

GLuint Program_FixedColor;

float CurrentTime;
float DeltaTime;
float Timer;

Camera* camera = nullptr;

Hex2D* Hex1 = nullptr;

Quad2D* Quad1 = nullptr;

int main()
{
	//Init GLFW and setting ver to 4.6 with only core functionality
	glfwInit();
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);

	//Create a GLFW controlled context window
	Window = glfwCreateWindow(800, 800, "First OpenGL Window", NULL, NULL);
	if (Window == NULL)
	{
		std::cout << "GLFW failed to initialize properly. Terminating program." << std::endl;
		system("pause");

		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(Window);

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
	while (glfwWindowShouldClose(Window) == false)
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

	CurrentTime = 0;
	DeltaTime = 0;
	Timer = 1 / 24;

	// Set window colour for when buffer is cleared
	glClearColor(1.0f, 0.0f, 0.0f, 1.0f);

	// Map range of the window size to NDC (-1 -> 1)
	glViewport(0, 0, 800, 800);

	Program_Texture = ShaderLoader::CreateProgram("Resources/Shaders/ClipSpace.vs",
		"Resources/Shaders/Texture.fs");

	Program_TwoTextureInterpolation = ShaderLoader::CreateProgram("Resources/Shaders/ClipSpace.vs",
		"Resources/Shaders/TextureMix.fs");

	Program_FixedColor = ShaderLoader::CreateProgram("Resources/Shaders/ClipSpace.vs",
		"Resources/Shaders/FixedColor.fs");

	camera = new Camera(800, 800, CurrentTime);

	Hex1 = new Hex2D();
	
	Quad1 = new Quad2D();

	Hex1->SetTexture(LoadTexture("Rayman.jpg"));
	Hex1->SetTexture(LoadTexture("AwesomeFace.png"));
	Hex1->m_ifadeindex = 1;

	Quad1->SetTexture(LoadTexture("Walk1.png"));
	Quad1->SetTexture(LoadTexture("Walk2.png"));
	Quad1->SetTexture(LoadTexture("Walk3.png"));
	Quad1->SetTexture(LoadTexture("Walk4.png"));
	Quad1->SetTexture(LoadTexture("Walk5.png"));
	Quad1->SetTexture(LoadTexture("Walk6.png"));
	Quad1->SetTexture(LoadTexture("Walk7.png"));
	Quad1->SetTexture(LoadTexture("Walk8.png"));

	Hex1->m_position = glm::vec3(-200.0f, 0.0f, 0.0f);
	Hex1->m_scale = glm::vec3(100.0f, 100.0f, 0.0f);
	
	
	Quad1->m_position = glm::vec3(200.0f, 0.0f, 0.0f);
	Quad1->m_scale = glm::vec3(100.0f, 200.0f, 0.0f);
}

//Update all objects and run the processes
void Update()
{
	glfwPollEvents();

	//Get the current time
	DeltaTime = CurrentTime;
	CurrentTime = (float)glfwGetTime();
	DeltaTime = CurrentTime - DeltaTime;

	if (Timer <= 0)
	{
		if ((size_t)Quad1->m_ifadeindex >= Quad1->m_textures.size() - 1)
		{
			Quad1->m_itextureindex = 0;
			Quad1->m_ifadeindex = 1;
		}
		else
		{
			Quad1->m_itextureindex++;
			Quad1->m_ifadeindex++;
		}
		Timer = 0.04f;
	}
	Timer -= DeltaTime;

	glClearColor(((sin(CurrentTime) + 1.0f) * 0.5f), ((sin(CurrentTime + 2.0f) + 0.5f) * 0.5f), ((sin(CurrentTime + 4.0f) + 1.0f) * 0.5f), 1.0f);
}

//Render all objects
void Render()
{
	//Clear the buffer
	glClear(GL_COLOR_BUFFER_BIT);
	Hex1->m_position = glm::vec3(-200.0f, 0.0f, 0.0f);
	camera->Render(*Hex1, Program_TwoTextureInterpolation);
	Hex1->m_position = glm::vec3(0.0f, 0.0f, 0.0f);
	camera->Render(*Hex1, Program_TwoTextureInterpolation);
	camera->Render(*Quad1, Program_TwoTextureInterpolation);

	//Push buffer to the screen
	glfwSwapBuffers(Window);
}
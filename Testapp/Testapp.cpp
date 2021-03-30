// Testapp.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

//Lib includes
#include <iostream>
#include <glew.h>
#include <glfw3.h>
#include "ShaderLoader.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

//Pointer to window
GLFWwindow* Window = nullptr;

//Shader for fixed triangle
GLuint Program_FixedTri;

void InitialSetup();
void Update();
void Render();

//Vertices / Indices
GLfloat Vertices_Tri[] = {
	//Position          //Colour
   0.1f, 0.2f, 0.0f,	1.0f, 0.0f, 0.0f, 
   0.1f, -0.2f, 0.0f,	0.0f, 1.0f, 0.0f,
	0.5f, -0.2f, 0.0f,	0.0f, 0.0f, 1.0f,

   0.1f, 0.2f, 0.0f,	1.0f, 0.0f, 0.0f,
   0.5f, 0.2f, 0.0f,	1.0f, 0.0f, 1.0f,
   0.5f, -0.2f, 0.0f,	0.0f, 0.0f, 1.0f,
};

GLfloat Vertices_Tri2[] = {
	//Position          //Colour
    -0.5f, 0.2f, 0.0f,	1.0f, 0.0f, 0.0f,
   -0.5f, -0.2f, 0.0f,	0.0f, 1.0f, 0.0f,
	-0.1f, -0.2f, 0.0f,	0.0f, 0.0f, 1.0f,

   -0.5f, 0.2f, 0.0f,	1.0f, 0.0f, 0.0f,
   -0.1f, 0.2f, 0.0f,	1.0f, 0.0f, 1.0f,
   -0.1f, -0.2f, 0.0f,	0.0f, 0.0f, 1.0f,
};

GLfloat Vertices_Quad[] = {
	// Index   // Position         // Colour           // Texture Coords
	/* 1 */   -0.5f, 0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   0.0f, 1.0f,        // Top - Left
	/* 2 */   -0.5f,-0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   0.0f, 0.0f,        // Bot - Left
	/* 3 */    0.5f,-0.5f, 0.0f,   1.0f, 0.0f, 1.0f,   1.0f, 0.0f,        // Bot - Right
	/* 4 */    0.5f, 0.5f, 0.0f,   0.0f, 1.0f, 1.0f,   1.0f, 1.0f,        // Top - Right
};

GLfloat Vertices_Hex[] = {
	// Index   // Position         // Colour           // Texture Coords
	/* 1 */   -0.25f, 0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   0.25f, 1.0f,
	/* 2 */   -0.5f, 0.0f, 0.0f,   0.0f, 1.0f, 0.0f,   0.0f, 0.5f,
	/* 3 */   -0.25f,-0.5f, 0.0f,   1.0f, 0.0f, 1.0f,   0.25f, 0.0f,
	/* 4 */    0.25f, -0.5f, 0.0f,   0.0f, 1.0f, 1.0f,   0.75f, 0.0f,
	/* 5 */    0.5f, 0.0f, 0.0f,   0.0f, 1.0f, 1.0f,   1.0f, 0.5f,
	/* 6 */    0.25f, 0.5f, 0.0f,   0.0f, 1.0f, 1.0f,   0.75f, 1.0f,
};

GLuint Indices_Quad[] = {
	0, 1, 2, // First Triangle (TL -> BL -> BR)
	0, 2, 3, // Second Triangle (TL -> BR -> TR)
};

GLuint Indices_Hex[] = {
	0, 1, 2,
	0, 2, 5,
	5, 2, 4,
	4, 2, 3,
};

//Upside down tri
GLuint Program_PositionOnly;

//VBO and VAO definitions
GLuint VBO_Tri;

GLuint VAO_Tri;

GLuint VBO_Tri2;

GLuint VAO_Tri2;

GLuint VBO_Quad;

GLuint VAO_Quad;

GLuint VBO_Hex;

GLuint VAO_Hex;


//EBO definitions
GLuint EBO_Quad;
GLuint EBO_Hex;

//Texture Definitions

GLuint Texture_Rayman;

GLuint Texture_AwesomeFace;


//Gay Triangle
GLuint Program_GayTri;

GLuint Program_GayFadeTri;

GLuint Program_Texture;

GLuint Program_TextureMix;

float CurrentTime;

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

//Setup initial elements of program
void InitialSetup()
{
	stbi_set_flip_vertically_on_load(true);
	// Load the Image data
	int ImageWidth;
	int ImageHeight;
	int ImageComponents;
	unsigned char* ImageData = stbi_load("Resources/Textures/Rayman.jpg", &ImageWidth, &ImageHeight, &ImageComponents, 0);

	//Create and bind a new texture template
	glGenTextures(1, &Texture_Rayman);
	glBindTexture(GL_TEXTURE_2D, Texture_Rayman);

	// Check how many components the loaded image has (rgb or rgba)
	GLint LoadedComponents = (ImageComponents == 4) ? GL_RGBA : GL_RGB;

	// Populate the texture with image data
	glTexImage2D(GL_TEXTURE_2D, 0, LoadedComponents, ImageWidth, ImageHeight, 0, LoadedComponents, GL_UNSIGNED_BYTE, ImageData);

	// Generate mipmaps, free memory and unbind texture
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(ImageData);
	glBindTexture(GL_TEXTURE_2D, 0);

	ImageData = stbi_load("Resources/Textures/AwesomeFace.png", &ImageWidth, &ImageHeight, &ImageComponents, 0);

	//Create and bind a new texture template
	glGenTextures(1, &Texture_AwesomeFace);
	glBindTexture(GL_TEXTURE_2D, Texture_AwesomeFace);

	// Check how many components the loaded image has (rgb or rgba)
	LoadedComponents = (ImageComponents == 4) ? GL_RGBA : GL_RGB;

	// Populate the texture with image data
	glTexImage2D(GL_TEXTURE_2D, 0, LoadedComponents, ImageWidth, ImageHeight, 0, LoadedComponents, GL_UNSIGNED_BYTE, ImageData);

	// Generate mipmaps, free memory and unbind texture
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(ImageData);
	glBindTexture(GL_TEXTURE_2D, 0);

	// Set window colour for when buffer is cleared
	glClearColor(1.0f, 0.0f, 0.0f, 1.0f);

	// Map range of the window size to NDC (-1 -> 1)
	glViewport(0, 0, 800, 800);

	//Create shader program for fixed triangle
	Program_FixedTri = ShaderLoader::CreateProgram("Resources/Shaders/FixedTriangle.vs",
		"Resources/Shaders/FixedColor.fs");
	/*Program_PositionOnly = ShaderLoader::CreateProgram("Resources/Shaders/PositionOnly.vs",
		"Resources/Shaders/FixedColor.fs");*/
	Program_GayTri = ShaderLoader::CreateProgram("Resources/Shaders/VertexColor.vs",
		"Resources/Shaders/VertexColor.fs");
	Program_GayFadeTri = ShaderLoader::CreateProgram("Resources/Shaders/VertexColor.vs",
		"Resources/Shaders/VertexColorFade.fs");
	Program_Texture = ShaderLoader::CreateProgram("Resources/Shaders/NDC_Texture.vs",
		"Resources/Shaders/Texture.fs");

	Program_TextureMix = ShaderLoader::CreateProgram("Resources/Shaders/NDC_Texture.vs",
		"Resources/Shaders/TextureMix.fs");
	

	

	glGenVertexArrays(1, &VAO_Hex);
	glBindVertexArray(VAO_Hex);

	glGenBuffers(1, &EBO_Hex);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_Hex);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices_Hex), Indices_Hex, GL_STATIC_DRAW);

	glGenBuffers(1, &VBO_Hex);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_Hex);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices_Hex), Vertices_Hex, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, false, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, false, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, false, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);
}

//Update all objects and run the processes
void Update()
{
	glfwPollEvents();

	//Get the current time
	CurrentTime = (float)glfwGetTime();
}

//Render all objects
void Render()
{
	//Clear the buffer
	glClear(GL_COLOR_BUFFER_BIT);

	//DRAW IN HERE
	//----------------------------
	//Bind program and VAO
	glUseProgram(Program_TextureMix);
	glBindVertexArray(VAO_Hex);
	//Send Vars to shaders via Uniform
	GLint CurrentTimeLoc = glGetUniformLocation(Program_GayFadeTri, "CurrentTime");
	glUniform1f(CurrentTimeLoc, CurrentTime);
	
	// Activate and Bind the texture
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, Texture_Rayman);
	glUniform1i(glGetUniformLocation(Program_TextureMix, "ImageTexture"), 0);

	// Activate and Bind the texture
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, Texture_AwesomeFace);
	glUniform1i(glGetUniformLocation(Program_TextureMix, "ImageTexture1"), 1);

	//Render the Triangle
	/*glDrawArrays(GL_TRIANGLES, 0, 6);*/
	glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);

	//Unbind assets
	glBindVertexArray(0);
	glUseProgram(0);

	/*
	//Bind program and VAO
	glUseProgram(Program_GayFadeTri);
	glBindVertexArray(VAO_Tri2);
	//Send Vars to shaders via Uniform
	CurrentTimeLoc = glGetUniformLocation(Program_GayFadeTri, "CurrentTime");
	glUniform1f(CurrentTimeLoc, CurrentTime);

	//Render the Triangle
	glDrawArrays(GL_TRIANGLES, 0, 6);

	//Unbind assets
	glBindVertexArray(0);
	glUseProgram(0);
	*/
	//----------------------------

	//Push buffer to the screen
	glfwSwapBuffers(Window);

	
}


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
#include "Mesh3D.h"
#include <fmod.hpp>
#include "GameObject.h"
#include "Character.h"
#include "SceneManager.h"
#include "FadingRect.h"

//Pointer to window
GLFWwindow* main_window = nullptr;

void InitialSetup();
void Update();
void Render();

//FMOD init stuff
FMOD::System* AudioSystem;
FMOD::Sound* FX_Gunshot;
FMOD::Sound* Track_Dance;

SceneManager* manager = nullptr;

bool AudioInit()
{
	if (FMOD::System_Create(&AudioSystem) != FMOD_OK)
	{
		std::cout << "FMOD ERROR: Audio System failed to create." << std::endl;
		return false;
	}
	if (AudioSystem->init(100, FMOD_INIT_NORMAL | FMOD_INIT_3D_RIGHTHANDED, 0) != FMOD_OK)
	{
		std::cout << "FMOD ERROR: Audio System failed to create." << std::endl;
		return false;
	}

	return true;
}

GLuint program_texture;

GLuint program_worldspace;

GLuint program_texture_interpolation;

GLuint program_fixed_color;

GLuint program_texture_wave;

GLuint program_normals;

float current_time;
float delta_time;
float timer;

Camera* camera = nullptr;

Hex2D* shape_hex = nullptr;

Quad2D* shape_quad = nullptr;

Quad2D* shape_quad2 = nullptr;

TextLabel* text_message = nullptr;

Pyramid3D* shape_pyramid = nullptr;

Mesh3D* shape_cube = nullptr;

Mesh3D* shape_sphere = nullptr;

Mesh3D* shape_cube2 = nullptr;

Mesh3D* shape_floor = nullptr;

Character* char_test = nullptr;

TextLabel* text_cursorpos = nullptr;

TextLabel* text_username = nullptr;

FadingRect* faderect_test = nullptr;

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
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	CObjectController::SetMainWindow(main_window);
	manager = new SceneManager(main_window);

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

	program_worldspace = ShaderLoader::CreateProgram("Resources/Shaders/ClipSpace.vs",
		"Resources/Shaders/Texture.fs");

	program_normals = ShaderLoader::CreateProgram("Resources/Shaders/3D_Normals.vs",
		"Resources/Shaders/Texture.fs");

	//Cull poly not facing viewport
	glCullFace(GL_BACK);

	//Enable culling
	glEnable(GL_CULL_FACE);

	//Create camera
	camera = new Camera(cfWINDOW_WIDTH(), cfWINDOW_HEIGHT(), current_time, true);

	//Create objects
	shape_hex = new Hex2D();
	
	shape_quad = new Quad2D();

	shape_quad2 = new Quad2D();

	shape_pyramid = new Pyramid3D();

	shape_cube = new Cube3D();
	shape_cube2 = new Cube3D();

	shape_sphere = new Sphere3D(1.0f, 20);

	shape_floor = new Cube3D();

	//Create character
	char_test = new Character(main_window);

	//Create Text

	text_message = new TextLabel("Super spicy text!", "Resources/Fonts/ARIAL.ttf", glm::ivec2(0, 40), glm::vec2(200.0f, 100.0f), TextLabel::MARQUEE, glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(1.0f,1.0f), 200.0f, 600.0f);

	text_cursorpos = new TextLabel("Default", "Resources/Fonts/ARIAL.ttf", glm::ivec2(0, 40), glm::vec2(0.0f, 0.0f), TextLabel::NONE);

	text_username = new TextLabel("Press Enter to type!", "Resources/Fonts/ARIAL.ttf", glm::ivec2(0, 40), glm::vec2(0.0f, 700.0f), TextLabel::MARQUEE);

	//Create FadeRect

	faderect_test = new FadingRect(glm::vec3(0, 0, 0), glm::vec3(1, 1, 0), LoadTexture("Rayman.jpg"), LoadTexture("AwesomeFace.png"));

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

	shape_sphere->AddTexture(LoadTexture("Rayman.jpg"));
	shape_cube->AddTexture(LoadTexture("Rayman.jpg"));
	shape_cube2->AddTexture(LoadTexture("Rayman.jpg"));
	shape_floor->AddTexture(LoadTexture("grid.jpg"));
	
	shape_floor->Position(glm::vec3(0.0f, -0.8f, 0.0f));
	shape_floor->Scale(glm::vec3(14.0f, 0.1f, 14.0f));
	
	shape_cube->Position(glm::vec3(-0.5f, -0.5f, -0.5f));
	shape_sphere->Position(glm::vec3(0.5f, 0.5f, -0.5f));
	shape_sphere->Scale(glm::vec3(0.5f, 0.5f, 0.5f));

	camera->m_cameraPos = glm::vec3(0.0f, 0.0f, -8.0f);

	//Setup sound stuff
	if (AudioSystem->createSound("Resources/Audio/Gunshot.wav", FMOD_DEFAULT, 0, &FX_Gunshot) != FMOD_OK)
	{
		std::cout << "FMOD ERROR: Failed to load sound using createSound(...)" << std::endl;
	}
}

void ProcessInput(float _deltatime)
{
	if (glfwGetKey(main_window, GLFW_KEY_W) == GLFW_PRESS)
	{

	}
	if (glfwGetKey(main_window, GLFW_KEY_A) == GLFW_PRESS)
	{

	}
	if (glfwGetKey(main_window, GLFW_KEY_S) == GLFW_PRESS)
	{

	}
	if (glfwGetKey(main_window, GLFW_KEY_D) == GLFW_PRESS)
	{

	}
}

//Update all objects and run the processes
void Update()
{
	CObjectController::UpdateObjects();

	double xpos;
	double ypos;
	glfwGetCursorPos(main_window, &xpos, &ypos);
	text_cursorpos->SetText("Pos: ( " + std::to_string(xpos - cfWINDOW_WIDTH() / 2) + " , " + std::to_string(ypos - cfWINDOW_HEIGHT() / 2) + " )");

	if (cfFLAG("Spam_me_please"))
	{
		std::cout << "SPAM ";
	}
	if (cfFLAG("Test_Flag"))
	{
		text_message->SetText("Test flag is enabled!");
	}
	else
	{
		text_message->SetText("Test flag is disabled!");
	}

	AudioSystem->update();
	glfwPollEvents();

	text_username->SetText(SceneManager::GetTextInputBuffer());

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

	
	//shape_pyramid->Rotation(glm::vec3(delta_time * 10, 0.0f, 0.0f) + shape_pyramid->Rotation());
	shape_cube->Rotation(glm::vec3(0.0f, delta_time * 70, 0.0f) + shape_cube->Rotation());
	shape_cube2->Rotation(glm::vec3(0.0f, delta_time * 70, 0.0f) + shape_cube2->Rotation());
	//shape_sphere->Rotation(glm::vec3(delta_time * -10, delta_time * -10, 0.0f) + shape_sphere->Rotation());

	//camera->m_cameraPos = glm::vec3( 4 *cos(current_time*2), 0.0f, 4 * sin(current_time*2));
	camera->m_lookAtTarget = true;

	shape_cube->Position(glm::vec3(cos(current_time * 2), 0.0f,  sin(current_time * 2)));
	shape_cube2->Position(glm::vec3( cos((current_time+1.57) * 2), 0.0f,  sin((current_time+1.57) * 2)));

	//Rainbow background
	glClearColor(((sin(current_time) + 1.0f) * 0.5f), ((sin(current_time + 2.0f) + 0.5f) * 0.5f), ((sin(current_time + 4.0f) + 1.0f) * 0.5f), 1.0f);

	
}

//Render all objects
void Render()
{
	//Clear the buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//Set position of shape and render it
	
	
	//Draw the shape
	
	//shape_hex->Render(*camera, program_texture_interpolation);
	
	shape_floor->Render(*camera, program_worldspace);

	if (cfFLAG("Render_Cube"))
	{
		shape_cube->Render(*camera, program_worldspace);
	}
	shape_cube2->Render(*camera, program_worldspace);

	char_test->Render(*camera, program_fixed_color);

	text_message->Render();

	text_cursorpos->Render();

	text_username->Render();

	faderect_test->m_rect->Render(*camera, program_texture_interpolation);

	//Push buffer to the screen
	glfwSwapBuffers(main_window);
}
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
#include "UIButton.h"
#include "Audiosystem.h"
#include "Renderable3D.h"

//Pointer to window
GLFWwindow* main_window = nullptr;

void InitialSetup();
void Update();
void Render();

//SCENEMANAGER
SceneManager* manager = nullptr;

//PROGRAMS
GLuint program_texture;
GLuint program_worldspace;
GLuint program_texture_interpolation;
GLuint program_fixed_color;
GLuint program_texture_wave;
GLuint program_normals;

//TIME
float current_time;
float delta_time;
float timer;

//CAMERAS
Camera* camera = nullptr;
Camera* orthocamera = nullptr;

//MESHES
Renderable3D* shape_firstcube = nullptr;
Renderable3D* shape_secondcube = nullptr;
Renderable3D* shape_floor = nullptr;


//CONTROLLABLE CHARACTERS
Character* char_test = nullptr;

//TEXTLABELS
TextLabel* text_message = nullptr;
TextLabel* text_cursorpos = nullptr;
TextLabel* text_username = nullptr;
TextLabel* text_scalebounce = nullptr;

//BUTTONS
UIButton* button_SoundEffect_Airhorn = nullptr;
UIButton* button_SoundEffect_Bruh = nullptr;

//SHAPES
Renderable3D* shape_cube = nullptr;
Renderable3D* shape_sphere = nullptr;


//Audio
Audiosystem* audio_main = nullptr;

int main()
{
	std::cout << "Program compiled " << __DATE__ << " | " << __TIME__ << std::endl;
	//Init GLFW and setting ver to 4.6 with only core functionality
	glfwInit();
	//AudioInit();
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_SAMPLES, 4);

	//Create a GLFW controlled context window
	main_window = glfwCreateWindow((int)cfWINDOW_WIDTH(), (int)cfWINDOW_HEIGHT(), "Nerys Thamm OpenGL Summative", NULL, NULL);
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

void CreateMaterials()
{
	//DEFAULT
	Lighting::AddMaterial("Default");

	//ADD ADDITIONAL MATERIALS HERE:
}

//Setup initial elements of program
void InitialSetup()
{
	stbi_set_flip_vertically_on_load(true);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_MULTISAMPLE);
	glDepthFunc(GL_LESS);
	CObjectController::SetMainWindow(main_window);
	manager = new SceneManager(main_window);

	current_time = 0;
	delta_time = 0;
	timer = 1 / 24;

	// Set window colour for when buffer is cleared
	glClearColor(1.0f, 0.0f, 0.0f, 1.0f);

	// Map range of the window size to NDC (-1 -> 1)
	glViewport(0, 0, (GLsizei)cfWINDOW_WIDTH(), (GLsizei)cfWINDOW_HEIGHT());

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
		"Resources/Shaders/3DLight_Phong.fs");

	//Cull poly not facing viewport
	glCullFace(GL_BACK);

	//Enable culling
	glEnable(GL_CULL_FACE);


	CreateMaterials();

	//Create camera
	camera = new Camera(cfWINDOW_WIDTH(), cfWINDOW_HEIGHT(), current_time, true);
	orthocamera = new Camera(cfWINDOW_WIDTH(), cfWINDOW_HEIGHT(), current_time, false);

	//Create objects
	shape_firstcube = new Renderable3D(Cube3D::GetMesh(), Lighting::GetMaterial("Default"));
	shape_secondcube = new Renderable3D(Cube3D::GetMesh(), Lighting::GetMaterial("Default"));
	shape_floor = new Renderable3D(Cube3D::GetMesh(), Lighting::GetMaterial("Default"));
	shape_cube = new Renderable3D(Cube3D::GetMesh(), Lighting::GetMaterial("Default"));
	shape_sphere = new Renderable3D(Sphere3D::GetMesh(1, 10), Lighting::GetMaterial("Default"));

	shape_cube->Position(glm::vec3(-1.0f, 0.0f, 0.0f));
	shape_sphere->Position(glm::vec3(1.0f, 0.0f, 0.0f));

	//Create character
	char_test = new Character(main_window);

	//Create Text
	text_message = new TextLabel("Super spicy text!", "Resources/Fonts/ARIAL.ttf", glm::ivec2(0, 40), glm::vec2(200.0f, 100.0f), TextLabel::TextEffect::MARQUEE, glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 1.0f), 200.0f, 600.0f);
	text_cursorpos = new TextLabel("Default", "Resources/Fonts/ARIAL.ttf", glm::ivec2(0, 40), glm::vec2(0.0f, 0.0f), TextLabel::TextEffect::NONE);
	text_username = new TextLabel("Press Enter to type!", "Resources/Fonts/ARIAL.ttf", glm::ivec2(0, 40), glm::vec2(0.0f, 650.0f), TextLabel::TextEffect::MARQUEE, glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 1.0f), 200.0f, 600.0f);
	text_scalebounce = new TextLabel("Username:", "Resources/Fonts/ARIAL.ttf", glm::ivec2(0, 40), glm::vec2(300.0f, 750.0f), TextLabel::TextEffect::SCALE_BOUNCE);

	//Create Buttons
	button_SoundEffect_Airhorn = new UIButton(glm::vec3(300, 300, 4), glm::vec3(200, 200, 1), LoadTexture("Button_Default.png"), LoadTexture("Button_Hover.png"), LoadTexture("Button_Press.png"));
	button_SoundEffect_Bruh = new UIButton(glm::vec3(-300, 300, 4), glm::vec3(200, 200, 1), LoadTexture("Button_Default.png"), LoadTexture("Button_Hover.png"), LoadTexture("Button_Press.png"));

	//Set textures of objects
	shape_firstcube->AddTexture(LoadTexture("Rayman.jpg"));
	shape_secondcube->AddTexture(LoadTexture("Rayman.jpg"));
	shape_floor->AddTexture(LoadTexture("grid.jpg"));
	shape_cube->AddTexture(LoadTexture("Rayman.jpg"));
	shape_sphere->AddTexture(LoadTexture("Rayman.jpg"));

	//Set position and scale of Environment
	shape_floor->Position(glm::vec3(0.0f, -0.8f, 0.0f));
	shape_floor->Scale(glm::vec3(14.0f, 0.1f, 14.0f));

	//Set position of Cameras
	camera->m_cameraPos = glm::vec3(0.0f, 0.0f, 8.0f);
	orthocamera->m_cameraPos = glm::vec3(0.0f, 0.0f, 8.0f);

	//Setup Audio
	audio_main = Audiosystem::GetInstance();

	//Download sounds if they dont already exist
	audio_main->AddSoundFromYoutube("https://www.youtube.com/watch?v=OoDo7kMbOd8", "Track_ShowaGroove");
	audio_main->AddSoundFromYoutube("https://www.youtube.com/watch?v=TKfS5zVfGBc", "Track_Dreamscape");
	audio_main->AddSoundFromYoutube("https://www.youtube.com/watch?v=UaUa_0qPPgc", "SFX_Airhorn");
	audio_main->AddSoundFromYoutube("https://www.youtube.com/watch?v=D2_r4q2imnQ", "SFX_Bruh");
	audio_main->AddSound("Track_PolishCow.mp3", "Track_PolishCow");
	

	//Start playing background track
	audio_main->PlaySound("Track_Dreamscape", 0.1f, true);
}

//Update all objects and run the processes
void Update()
{
	//Update all GameObjects
	CObjectController::UpdateObjects();

	//Update Cursor position text
	double xpos;
	double ypos;
	glfwGetCursorPos(main_window, &xpos, &ypos);
	text_cursorpos->SetText("Pos: ( " + std::to_string(xpos - cfWINDOW_WIDTH() / 2) + " , " + std::to_string(ypos - cfWINDOW_HEIGHT() / 2) + " )");

	//Update Flag test text
	if (cfFLAG("Test_Flag"))
	{
		text_message->SetText("Test flag is enabled!");
	}
	else
	{
		text_message->SetText("Test flag is disabled!");
	}

	//Play sound if button is pressed
	if (button_SoundEffect_Airhorn->GetState() == UIButton::State::BUTTON_PRESSED)
	{
		audio_main->PlaySound("SFX_Airhorn");
	}
	if (button_SoundEffect_Bruh->GetState() == UIButton::State::BUTTON_PRESSED)
	{
		audio_main->PlaySound("SFX_Bruh");
	}

	//Poll events for GLFW input
	glfwPollEvents();


	Lighting::GetMaterial("Default")->Smoothness = ((sin(current_time) + 1) / 2) * 32;
	Lighting::Global_Illumination_Color = glm::vec3(((sin(current_time) + 1.0f) * 0.5f), ((sin(current_time + 2.0f) + 0.5f) * 0.5f), ((sin(current_time + 4.0f) + 1.0f) * 0.5f));
	Lighting::Global_Illumination_Strength = ((sin(current_time) + 1) / 2);

	//Update Username test from the Input Buffer
	text_username->SetText("\"" + SceneManager::GetTextInputBuffer() + "\"");

	//Get the current time
	delta_time = current_time;
	current_time = (float)glfwGetTime();
	delta_time = current_time - delta_time;

	//Rotate and move Cubes
	shape_firstcube->Rotation(glm::vec3(0.0f, delta_time * 70, 0.0f) + shape_firstcube->Rotation());
	shape_secondcube->Rotation(glm::vec3(0.0f, delta_time * 70, 0.0f) + shape_secondcube->Rotation());

	shape_firstcube->Position(glm::vec3(cos(current_time * 2), 0.0f, sin(current_time * 2)));
	shape_secondcube->Position(glm::vec3(cos((current_time + 1.57) * 2), 0.0f, sin((current_time + 1.57) * 2)));

	shape_cube->Rotation(glm::vec3(delta_time * 70, delta_time * 70, delta_time * 70) + shape_cube->Rotation());

	//Rainbow background
	glClearColor(((sin(current_time) + 1.0f) * 0.5f), ((sin(current_time + 2.0f) + 0.5f) * 0.5f), ((sin(current_time + 4.0f) + 1.0f) * 0.5f), 1.0f);
}

//Render all objects
void Render()
{
	//Clear the buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Render the floor
	/*shape_floor->Render(*camera, program_worldspace);*/

	//Render the cubes
	//if (cfFLAG("Render_First_Cube"))//Check config file
	//{
	//	shape_firstcube->Render(*camera, program_worldspace);
	//}
	//if (cfFLAG("Render_Second_Cube"))//Check config file
	//{
	//	shape_secondcube->Render(*camera, program_worldspace);
	//}

	////Render the buttons
	//button_SoundEffect_Airhorn->Render(*orthocamera, program_texture);
	//button_SoundEffect_Bruh->Render(*orthocamera, program_texture);

	shape_cube->Render(*camera, program_normals);
	shape_sphere->Render(*camera, program_normals);

	//Render the Character
	/*char_test->Render(*camera, program_fixed_color);*/

	//Render the textlabels
	/*text_message->Render();
	text_cursorpos->Render();
	text_username->Render();
	text_scalebounce->Render();*/

	//Push buffer to the screen
	glfwSwapBuffers(main_window);
}
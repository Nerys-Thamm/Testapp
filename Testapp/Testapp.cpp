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
#include "Skybox.h"
#include "TextureLoader.h"
#include "FreeCam.h"
#include "PointLightObj.h"


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
GLuint program_blinnphong;
GLuint program_blinnphongrim;
GLuint program_blinnphongfog;
GLuint program_reflective;
GLuint program_reflectivefog;
GLuint program_reflectiverim;

//TIME
float current_time;
float delta_time;
float timer;

//CAMERAS
Camera* camera = nullptr;
FreeCam* freecam = nullptr;
Camera* orthocamera = nullptr;



//MESHES
Renderable3D* shape_firstcube = nullptr;
Renderable3D* shape_secondcube = nullptr;
Renderable3D* shape_floor = nullptr;

Renderable3D* shape_seafloor = nullptr;
Renderable3D* shape_sea = nullptr;


//CONTROLLABLE CHARACTERS
Character* char_test = nullptr;

//TEXTLABELS
TextLabel* text_message = nullptr;
TextLabel* text_cursorpos = nullptr;
TextLabel* text_username = nullptr;
TextLabel* text_scalebounce = nullptr;

//BUTTONS
UIButton* button_cameraforward = nullptr;
UIButton* button_cameraback = nullptr;

//SHAPES
Renderable3D* shape_cube = nullptr;
Renderable3D* shape_stencilcube = nullptr;






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



void CreateMaterials()
{
	//DEFAULT
	Lighting::AddMaterial("Default");
	Lighting::GetMaterial("Default")->Smoothness = 60;
	Lighting::GetMaterial("Default")->Reflectivity = 0.0f;
	//ADD ADDITIONAL MATERIALS HERE:
	Lighting::AddMaterial("Glossy");
	Lighting::GetMaterial("Glossy")->Smoothness = 60;
	Lighting::GetMaterial("Glossy")->Reflectivity = 0.5f;
	Lighting::AddMaterial("Chrome");
	Lighting::GetMaterial("Chrome")->Smoothness = 80;
	Lighting::GetMaterial("Chrome")->Reflectivity = 0.9f;

	
}

//Setup initial elements of program
void InitialSetup()
{
	
	glEnable(GL_DEPTH_TEST);
	
	glEnable(GL_MULTISAMPLE);
	glEnable(GL_SCISSOR_TEST);
	glScissor(0, 100, 800, 600);
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

	program_blinnphong = ShaderLoader::CreateProgram("Resources/Shaders/3D_Normals.vs",
		"Resources/Shaders/3DLight_BlinnPhong.fs");

	program_blinnphongrim = ShaderLoader::CreateProgram("Resources/Shaders/3D_Normals.vs",
		"Resources/Shaders/3DLight_BlinnPhongRim.fs");

	program_reflective = ShaderLoader::CreateProgram("Resources/Shaders/3D_Normals.vs",
		"Resources/Shaders/3DLight_Reflective.fs");

	program_reflectiverim = ShaderLoader::CreateProgram("Resources/Shaders/3D_Normals.vs",
		"Resources/Shaders/3DLight_ReflectiveRim.fs");

	program_blinnphongfog = ShaderLoader::CreateProgram("Resources/Shaders/3D_Normals.vs",
		"Resources/Shaders/3DLight_BlinnPhongFog.fs");

	program_reflectivefog = ShaderLoader::CreateProgram("Resources/Shaders/3D_Normals.vs",
		"Resources/Shaders/3DLight_ReflectiveFog.fs");

	//Cull poly not facing viewport
	glCullFace(GL_BACK);

	//Enable culling
	glEnable(GL_CULL_FACE);


	CreateMaterials();

	//Create camera
	camera = new Camera(cfWINDOW_WIDTH(), cfWINDOW_HEIGHT(), current_time, true);
	freecam = new FreeCam(main_window, cfWINDOW_WIDTH(), cfWINDOW_HEIGHT(), current_time, true);
	orthocamera = new Camera(cfWINDOW_WIDTH(), cfWINDOW_HEIGHT(), current_time, false);



	//Create Skybox
	std::string SkyboxFilepaths[] = {"MountainOutpost/Right.jpg","MountainOutpost/Left.jpg","MountainOutpost/Up.jpg","MountainOutpost/Down.jpg","MountainOutpost/Back.jpg","MountainOutpost/Front.jpg"};
	
	SceneManager::SetCurrentSkybox(new Skybox(camera, SkyboxFilepaths));



	//Create objects
	shape_floor = new Renderable3D(Cube3D::GetMesh(), Lighting::GetMaterial("Default"));
	shape_cube = new Renderable3D(Cube3D::GetMesh(), Lighting::GetMaterial("Chrome"));
	shape_stencilcube = new Renderable3D(Cube3D::GetMesh(), Lighting::GetMaterial("Glossy"));
	

	shape_cube->Position(glm::vec3(0.0f, 5.0f, 2.0f));
	shape_cube->Scale(glm::vec3(2.0f, 2.0f, 2.0f));
	
	

	shape_seafloor = new Renderable3D(Quad3D::GetMesh(), Lighting::GetMaterial("Default"));
	shape_sea = new Renderable3D(Quad3D::GetMesh(), Lighting::GetMaterial("Glossy"));

	shape_seafloor->Position(glm::vec3(0.0f, 3.5f, 0.0f));
	shape_seafloor->Scale(glm::vec3(100.0f, 100.0f, 1.0f));
	shape_seafloor->Rotation(glm::vec3(270.0f, 0.0f, 0.0f));

	shape_sea->Position(glm::vec3(0.0f, 4.0f, 0.0f));
	shape_sea->Scale(glm::vec3(100.0f, 100.0f, 1.0f));
	shape_sea->Rotation(glm::vec3(270.0f, 0.0f, 0.0f));

	

	

	//Create character
	char_test = new Character(main_window);



	



	


	//Set textures of objects
	shape_floor->AddTexture(TextureLoader::LoadTexture("grid.jpg"));
	shape_cube->AddTexture(TextureLoader::LoadTexture("SciFi_Albedo.jpg"));
	shape_cube->AddTexture(TextureLoader::LoadTexture("SciFi_Metallic.jpg"));
	shape_stencilcube->AddTexture(TextureLoader::LoadTexture("Yellow.jpg"));
	shape_stencilcube->AddTexture(TextureLoader::LoadTexture("Grey.jpg"));
	

	shape_seafloor->AddTexture(TextureLoader::LoadTexture("beachsand.jpg"));
	shape_sea->AddTexture(TextureLoader::LoadTexture("WaterTransparent2.png"));
	shape_sea->AddTexture(TextureLoader::LoadTexture("WaterSpecular.png"));



	//Set position and scale of Environment
	shape_floor->Position(glm::vec3(0.0f, -0.8f, 0.0f));
	shape_floor->Scale(glm::vec3(14.0f, 0.1f, 14.0f));



	//Set position of Cameras
	camera->m_cameraPos = glm::vec3(0.0f, 7.0f, 35.0f);
	camera->m_cameraTargetPos = shape_cube->Position();
	camera->m_lookAtTarget = true;
	orthocamera->m_cameraPos = glm::vec3(0.0f, 0.0f, 8.0f);

	freecam->GetCamera()->m_cameraPos.y = 5.0f;
	



	//Setup Audio
	audio_main = Audiosystem::GetInstance();



	//Download sounds if they dont already exist
	audio_main->AddSoundFromYoutube("https://www.youtube.com/watch?v=OoDo7kMbOd8", "Track_ShowaGroove");
	
	
	


	//Start playing background track
	audio_main->PlaySound("Track_ShowaGroove", 0.1f, true);



	
	
	Lighting::DirectionalLights[0].Direction = glm::vec3(1.0f, -1.0f, 1.0f);
	Lighting::DirectionalLights[0].Color = glm::vec3(1.0f, 1.0f, 1.0f);
	Lighting::DirectionalLights[0].AmbientStrength = 0.15f;
	Lighting::DirectionalLights[0].SpecularStrength = 1.0f;
}



//Update all objects and run the processes
void Update()
{
	//Update all GameObjects
	CObjectController::UpdateObjects();

	
	
	////Update Flag test text
	//if (cfFLAG("Test_Flag"))
	//{
	//	text_message->SetText("Test flag is enabled!");
	//}
	//else
	//{
	//	text_message->SetText("Test flag is disabled!");
	//}

	

	//Poll events for GLFW input
	glfwPollEvents();



	

	//Get the current time
	delta_time = current_time;
	current_time = (float)glfwGetTime();
	delta_time = current_time - delta_time;

	camera->m_cameraTargetPos = shape_cube->Position();

	shape_cube->Rotation(shape_cube->Rotation() + glm::vec3(0.0f, delta_time * 2, 0.0f));

	shape_stencilcube->Position(shape_cube->Position());
	shape_stencilcube->Scale(shape_cube->Scale() + glm::vec3(0.5f, 0.5f, 0.5f));
	shape_stencilcube->Rotation(shape_cube->Rotation());

	
	

	//Rainbow background
	glClearColor(((sin(current_time) + 1.0f) * 0.5f), ((sin(current_time + 2.0f) + 0.5f) * 0.5f), ((sin(current_time + 4.0f) + 1.0f) * 0.5f), 1.0f);
}

//Render all objects
void Render()
{
	//Clear the buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	

	//Render the skybox
	SceneManager::GetCurrentSkybox()->Render();

	

	//Render the floor
	
	shape_seafloor->Render(*camera, program_blinnphongfog);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	shape_sea->Render(*camera, program_reflectivefog);
	glDisable(GL_BLEND);

	//Render objects

	glEnable(GL_STENCIL_TEST);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
	glStencilFunc(GL_ALWAYS, 1, 0xFF);
	glStencilMask(0xFF);
	shape_cube->Render(*camera, program_blinnphongfog);
	glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
	glStencilMask(0x00);
	shape_stencilcube->Render(*camera, program_reflective);
	glStencilMask(0x00);
	glDisable(GL_STENCIL_TEST);
	glStencilMask(0xFF);
	

	

	

	

	//Push buffer to the screen
	glfwSwapBuffers(main_window);
}
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
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "Entity.h"
#include "MeshRenderer.h"
#include "TestBehaviour.h"
#include "Cloth.h"
#include "ClothRenderer.h"
#include "GeometryRenderer.h"


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
GLuint program_postprocess;
GLuint program_geostar;

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

Renderable3D* terrain_auckland = nullptr;



//BUTTONS
Renderable3D* shape_3Dbutton_fwd = nullptr;
Renderable3D* shape_3Dbutton_bck = nullptr;

//SHAPES
Renderable3D* shape_cube = nullptr;
Renderable3D* shape_stencilcube = nullptr;
Renderable3D* shape_renderquad = nullptr;

Shape2D* ppQuad = nullptr;

//Audio
Audiosystem* audio_main = nullptr;

//PostProcessing
GLuint renderTexture;
GLuint frameBuffer;

std::vector<Renderable3D> testcubes;

CEntity* entityTest = nullptr;
CEntity* childEntityTest = nullptr;
CEntity* otherEntityTest = nullptr;
CEntity* rootBone = nullptr;
CEntity* secondBone = nullptr;
CEntity* thirdBone = nullptr;

CEntity* clothEntity = nullptr;

//---------------------------------------------------------------
//GUI variables
bool bWireFrameMode = false;
float clothLength = 60.0f;
float clothWidth = 60.0f;

int numberOfHooks = 3;
float hookDistance = 20.0f;
float clothStiffness = 0.5f;

const char* mouseModeItems[]{ "Pull", "Push", "Tear", "Fire", "Pin" };
int selectedMouseMode = 0;

const char* collisionItems[]{ "No Object", "Sphere", "Capsule", "Pyramid" };
int selectedCollision = 0;

float windDirection = 0.0f;
float windStrength = 10.0f;

float timeOfDay = 0.0f;

//----------------------------------------------------

void ShutDown()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

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
		ShutDown();
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(main_window);

	//Init GLEW to populate OpenGL func pointers
	if (glewInit() != GLEW_OK)
	{
		std::cout << "GLEW failed to initialize properly. Terminating program." << std::endl;
		system("pause");
		ShutDown();
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
	ShutDown();
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

	Lighting::AddMaterial("EntityTest");
	Lighting::GetMaterial("EntityTest")->Smoothness = 60;
	Lighting::GetMaterial("EntityTest")->Reflectivity = 0.0f;
	//Lighting::GetMaterial("EntityTest")->AlbedoMap = TextureLoader::LoadTexture("SciFi_Albedo.jpg");

	
}

//Setup initial elements of program
void InitialSetup()
{
	
	glEnable(GL_DEPTH_TEST);
	
	glEnable(GL_MULTISAMPLE);
	glEnable(GL_SCISSOR_TEST);
	glScissor(0, 100, (GLsizei)cfWINDOW_WIDTH(), (GLsizei)cfWINDOW_HEIGHT()-200);
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
	
	program_postprocess = ShaderLoader::CreateProgram("Resources/Shaders/NDC_Texture.vs",
		"Resources/Shaders/Texture.fs");

	program_geostar = ShaderLoader::CreateProgram("Resources/Shaders/GeoShader.vs",
		"Resources/Shaders/GeoShader.fs", "Resources/Shaders/StarGeoShader.gs");

	//Cull poly not facing viewport
	glCullFace(GL_BACK);

	//Enable culling
	glEnable(GL_CULL_FACE);


	CreateMaterials();

	//Create camera
	camera = new Camera(cfWINDOW_WIDTH(), cfWINDOW_HEIGHT(), current_time, true);
	freecam = new FreeCam(main_window, cfWINDOW_WIDTH(), cfWINDOW_HEIGHT(), current_time, true);
	orthocamera = new Camera(cfWINDOW_WIDTH(), cfWINDOW_HEIGHT(), current_time, false);

	manager->SetMainCamera(freecam->GetCamera());

	//Create Skybox
	std::string SkyboxFilepaths[] = {"MountainOutpost/Right.jpg","MountainOutpost/Left.jpg","MountainOutpost/Up.jpg","MountainOutpost/Down.jpg","MountainOutpost/Back.jpg","MountainOutpost/Front.jpg"};
	
	SceneManager::SetCurrentSkybox(new Skybox(freecam->GetCamera(), SkyboxFilepaths));



	//Create objects
	
	/*rootBone = new CEntity();
	secondBone = new CEntity(rootBone);
	thirdBone = new CEntity(secondBone);

	entityTest = new CEntity(rootBone);
	entityTest->AddBehaviour<MeshRenderer>();
	entityTest->GetBehaviour<MeshRenderer>()->SetMesh(Cube3D::GetMesh());
	entityTest->GetBehaviour<MeshRenderer>()->SetMaterial(Lighting::GetMaterial("EntityTest"));
	entityTest->GetBehaviour<MeshRenderer>()->SetShader(program_blinnphong);
	entityTest->GetBehaviour<MeshRenderer>()->SetTexture(TextureLoader::LoadTexture("Yellow.jpg"));

	

	childEntityTest = new CEntity(secondBone);
	childEntityTest->AddBehaviour<MeshRenderer>();
	childEntityTest->GetBehaviour<MeshRenderer>()->SetMesh(Cube3D::GetMesh());
	childEntityTest->GetBehaviour<MeshRenderer>()->SetMaterial(Lighting::GetMaterial("EntityTest"));
	childEntityTest->GetBehaviour<MeshRenderer>()->SetShader(program_blinnphong);
	childEntityTest->GetBehaviour<MeshRenderer>()->SetTexture(TextureLoader::LoadTexture("Yellow.jpg"));

	otherEntityTest = new CEntity(thirdBone);
	otherEntityTest->AddBehaviour<MeshRenderer>();
	otherEntityTest->GetBehaviour<MeshRenderer>()->SetMesh(Cube3D::GetMesh());
	otherEntityTest->GetBehaviour<MeshRenderer>()->SetMaterial(Lighting::GetMaterial("EntityTest"));
	otherEntityTest->GetBehaviour<MeshRenderer>()->SetShader(program_blinnphong);
	otherEntityTest->GetBehaviour<MeshRenderer>()->SetTexture(TextureLoader::LoadTexture("Yellow.jpg"));

	rootBone->AddBehaviour<TestBehaviour>();
	secondBone->AddBehaviour<TestBehaviour>();
	thirdBone->AddBehaviour<TestBehaviour>();*/

	entityTest = new CEntity();
	entityTest->AddBehaviour<GeometryRenderer>();
	entityTest->GetBehaviour<GeometryRenderer>()->SetShader(program_geostar);
	entityTest->m_transform.position = glm::vec3(0.0f, 3.0f, 0.0f);
	

	clothEntity = new CEntity();
	std::shared_ptr<ClothRenderer> clothrenderer = clothEntity->AddBehaviour<ClothRenderer>();
	clothrenderer->SetCloth(new Cloth(glm::vec2(5.0f, 5.0f), glm::ivec2(4, 4)));
	clothrenderer->SetMaterial(Lighting::GetMaterial("EntityTest"));
	clothrenderer->SetShader(program_blinnphong);
	clothrenderer->SetTexture(TextureLoader::LoadTexture("Yellow.jpg"));
	clothEntity->m_transform.position = glm::vec3(6.0f, 10.0f, 8.0f);

	shape_cube = new Renderable3D(Cube3D::GetMesh(), Lighting::GetMaterial("Chrome"));
	shape_stencilcube = new Renderable3D(Cube3D::GetMesh(), Lighting::GetMaterial("Glossy"));
	shape_3Dbutton_fwd = new Renderable3D(Cube3D::GetMesh(), Lighting::GetMaterial("Default"));
	shape_3Dbutton_bck = new Renderable3D(Cube3D::GetMesh(), Lighting::GetMaterial("Default"));
	shape_renderquad = new Renderable3D(Quad3D::GetMesh(), Lighting::GetMaterial("Default"));

	Terrain3D::LoadFromRaw("AucklandHarbor2.raw", 1081, 0.1f, 0.025f);

	terrain_auckland = new Renderable3D(Terrain3D::GetTerrainMesh("AucklandHarbor2.raw"), Lighting::GetMaterial("Default"));
	

	shape_cube->Position(glm::vec3(0.0f, 3.8f, 2.0f));
	shape_cube->Scale(glm::vec3(2.0f, 2.0f, 2.0f));

	shape_3Dbutton_fwd->Position(glm::vec3(-4.0f, 5.0f, 2.0f));
	shape_3Dbutton_fwd->Scale(glm::vec3(2.0f, 2.0f, 2.0f));
	shape_3Dbutton_bck->Position(glm::vec3(4.0f, 5.0f, 2.0f));
	shape_3Dbutton_bck->Scale(glm::vec3(2.0f, 2.0f, 2.0f));
	
	/*
	
	entityTest->m_transform.scale = glm::vec3(5.0f, 2.0f, 2.0f);
	childEntityTest->m_transform.scale = glm::vec3(5.0f, 1.5f, 1.5f);
	childEntityTest->m_transform.position = glm::vec3(2.5f, 0.0f, 0.0f);
	otherEntityTest->m_transform.scale = glm::vec3(5.0f, 1.0f, 1.0f);
	otherEntityTest->m_transform.position = glm::vec3(2.5f, 0.0f, 0.0f);
	secondBone->m_transform.position = glm::vec3(2.5f, 0.0f, 0.0f);
	thirdBone->m_transform.position = glm::vec3(5.0f, 0.0f, 0.0f);
	rootBone->m_transform.position = glm::vec3(6.0f, 4.0f, 8.0f);
	*/

	shape_seafloor = new Renderable3D(Quad3D::GetMesh(), Lighting::GetMaterial("Default"));
	shape_sea = new Renderable3D(Quad3D::GetMesh(), Lighting::GetMaterial("Glossy"));

	shape_seafloor->Position(glm::vec3(0.0f, 3.5f, 0.0f));
	shape_seafloor->Scale(glm::vec3(100.0f, 100.0f, 1.0f));
	shape_seafloor->Rotation(glm::vec3(271.0f, 0.0f, 0.0f));

	shape_sea->Position(glm::vec3(0.0f, 0.01f, 0.0f));
	shape_sea->Scale(glm::vec3(500.0f, 500.0f, 1.0f));
	shape_sea->Rotation(glm::vec3(270.0f, 0.0f, 0.0f));

	
	terrain_auckland->Position(glm::vec3(0.0f,0.0f, 0.0f));
	terrain_auckland->Scale(glm::vec3(1.0f, 1.0f, 1.0f));
	terrain_auckland->Rotation(glm::vec3(0.0f, 0.0f, 0.0f));
	
	/*for (int i = 0; i < 100; i++)
	{
		for (int j = 0; j < 100; j++)
		{
			Renderable3D cube(Cube3D::GetMesh(), Lighting::GetMaterial("Default"));
			float groundHeight = Terrain3D::GetTerrain("AucklandHarbor2.raw")->GetHeightFromWorldPos(terrain_auckland->Position(), terrain_auckland->Rotation(), glm::vec3(i, 0.0f, j));
			cube.Position(glm::vec3(i, groundHeight, j));
			testcubes.push_back(cube);
		}
	}*/

	


	//Set textures of objects
	shape_cube->AddTexture(TextureLoader::LoadTexture("SciFi_Albedo.jpg"));
	shape_cube->AddTexture(TextureLoader::LoadTexture("SciFi_Metallic.jpg"));
	shape_stencilcube->AddTexture(TextureLoader::LoadTexture("Yellow.jpg"));
	shape_stencilcube->AddTexture(TextureLoader::LoadTexture("Grey.jpg"));
	shape_3Dbutton_fwd->AddTexture(TextureLoader::LoadTexture("Yellow.jpg"));
	shape_3Dbutton_bck->AddTexture(TextureLoader::LoadTexture("Grey.jpg"));
	terrain_auckland->AddTexture(TextureLoader::LoadTexture("map.png"));
	
	

	shape_seafloor->AddTexture(TextureLoader::LoadTexture("beachsand.jpg"));
	shape_sea->AddTexture(TextureLoader::LoadTexture("WaterTransparent2.png"));
	shape_sea->AddTexture(TextureLoader::LoadTexture("WaterSpecular.png"));




	//Set position of Cameras
	camera->m_cameraPos = glm::vec3(0.0f, 0.0f, 1.0f);
	camera->m_cameraTargetPos = shape_renderquad->Position();
	//camera->m_lookAtTarget = true;
	orthocamera->m_cameraPos = glm::vec3(0.0f, 0.0f, 1.0f);
	freecam->GetCamera()->m_cameraPos.y = 5.0f;
	



	//Setup Audio
	audio_main = Audiosystem::GetInstance();



	//Download sounds if they dont already exist
	audio_main->AddSound("Track_BeachAmbience.mp3", "Track_BeachAmbience");
	
	
	//Start playing background track
	audio_main->PlaySound("Track_BeachAmbience", 0.5f, true);


	
	Lighting::DirectionalLights[0].Direction = glm::vec3(-1.0f, -1.0f, -1.0f);
	Lighting::DirectionalLights[0].Color = glm::vec3(1.0f, 0.8f, 0.8f);
	Lighting::DirectionalLights[0].AmbientStrength = 0.15f;
	Lighting::DirectionalLights[0].SpecularStrength = 1.0f;

	//Post Processing
	TextureLoader::CreateFrameBuffer(cfWINDOW_WIDTH(), cfWINDOW_HEIGHT(), renderTexture, frameBuffer);
	ppQuad = new Quad2D();
	ppQuad->AddTexture(TextureLoader::LoadTexture("Yellow.jpg"));
	ppQuad->Position(glm::vec3{ 300.0f,300.0f,4.0f });
	ppQuad->Scale(glm::vec3{ 200.0f,200.0f,1.0f });
	ppQuad->Rotation(glm::vec3{ 0.0f,0.0f,0.0f });
	shape_renderquad->AddTexture(renderTexture);
	shape_renderquad->Scale(glm::vec3{ 2.0f, 1.0f, 1.0f });
	//shape_renderquad->Position(glm::vec3{ 0.0f, 0.0f, -1.0f });


	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(main_window, true);
	ImGui_ImplOpenGL3_Init("#version 330");
}

//Resets the scene
void ResetScene()
{
	//Delete objects
	delete shape_cube;
	delete shape_stencilcube;
	delete shape_3Dbutton_fwd;
	delete shape_3Dbutton_bck;
	delete shape_seafloor;
	delete shape_sea;
	

	//Create new objects and set position + orientation

	shape_cube = new Renderable3D(Cube3D::GetMesh(), Lighting::GetMaterial("Chrome"));
	shape_stencilcube = new Renderable3D(Cube3D::GetMesh(), Lighting::GetMaterial("Glossy"));
	shape_3Dbutton_fwd = new Renderable3D(Cube3D::GetMesh(), Lighting::GetMaterial("Default"));;
	shape_3Dbutton_bck = new Renderable3D(Cube3D::GetMesh(), Lighting::GetMaterial("Default"));;
	shape_cube->Position(glm::vec3(0.0f, 3.8f, 2.0f));
	shape_cube->Scale(glm::vec3(2.0f, 2.0f, 2.0f));
	shape_3Dbutton_fwd->Position(glm::vec3(-4.0f, 5.0f, 2.0f));
	shape_3Dbutton_fwd->Scale(glm::vec3(2.0f, 2.0f, 2.0f));
	shape_3Dbutton_bck->Position(glm::vec3(4.0f, 5.0f, 2.0f));
	shape_3Dbutton_bck->Scale(glm::vec3(2.0f, 2.0f, 2.0f));
	shape_seafloor = new Renderable3D(Quad3D::GetMesh(), Lighting::GetMaterial("Default"));
	shape_sea = new Renderable3D(Quad3D::GetMesh(), Lighting::GetMaterial("Glossy"));
	shape_seafloor->Position(glm::vec3(0.0f, 3.5f, 0.0f));
	shape_seafloor->Scale(glm::vec3(100.0f, 100.0f, 1.0f));
	shape_seafloor->Rotation(glm::vec3(271.0f, 0.0f, 0.0f));
	shape_sea->Position(glm::vec3(0.0f, 4.0f, 0.0f));
	shape_sea->Scale(glm::vec3(100.0f, 100.0f, 1.0f));
	shape_sea->Rotation(glm::vec3(270.0f, 0.0f, 0.0f));


	//Set textures of objects
	shape_cube->AddTexture(TextureLoader::LoadTexture("SciFi_Albedo.jpg"));
	shape_cube->AddTexture(TextureLoader::LoadTexture("SciFi_Metallic.jpg"));
	shape_stencilcube->AddTexture(TextureLoader::LoadTexture("Yellow.jpg"));
	shape_stencilcube->AddTexture(TextureLoader::LoadTexture("Grey.jpg"));
	shape_3Dbutton_fwd->AddTexture(TextureLoader::LoadTexture("Yellow.jpg"));
	shape_3Dbutton_bck->AddTexture(TextureLoader::LoadTexture("Grey.jpg"));
	shape_seafloor->AddTexture(TextureLoader::LoadTexture("beachsand.jpg"));
	shape_sea->AddTexture(TextureLoader::LoadTexture("WaterTransparent2.png"));
	shape_sea->AddTexture(TextureLoader::LoadTexture("WaterSpecular.png"));

	//Set position of Cameras
	camera->m_cameraPos = glm::vec3(0.0f, 0.0f, 0.0f);
	camera->m_cameraTargetPos = shape_renderquad->Position();
	camera->m_lookAtTarget = true;

	//Reset Audio
	audio_main = Audiosystem::GetInstance();
	//Start playing background track
	audio_main->PlaySound("Track_BeachAmbience", 0.5f, true);

	//Setup Lighting
	Lighting::DirectionalLights[0].Direction = glm::vec3(-1.0f, -1.0f, -1.0f);
	Lighting::DirectionalLights[0].Color = glm::vec3(1.0f, 1.0f, 1.0f);
	Lighting::DirectionalLights[0].AmbientStrength = 0.1f;
	Lighting::DirectionalLights[0].SpecularStrength = 1.0f;
}

float mouseY, mouseX;
glm::vec3 rayDirection;

//Get cursor position relative to window
void MousePassive()
{
	double x, y;
	glfwGetCursorPos(CObjectController::GetMainWindow(), &x, &y);
	mouseX = (2.0f * x) / cfWINDOW_WIDTH() - 1.0f;
	mouseY = 1.0f - (2.0f * y) / cfWINDOW_HEIGHT();
}

//Update mouse picking ray
void UpdateMousePicking()
{
	//screen pos
	glm::vec2 normalizedScreenPos = glm::vec2(mouseX, mouseY);

	//screenpos to Proj space
	glm::vec4 clipCoords = glm::vec4(normalizedScreenPos.x, normalizedScreenPos.y, -1.0f, 1.0f);

	//Proj space to eye space
	glm::mat4 invProjMat = glm::inverse(camera->m_projectionMat);
	glm::vec4 eyeCoords = invProjMat * clipCoords;
	eyeCoords = glm::vec4(eyeCoords.x, eyeCoords.y, -1.0f, 0.0f);

	//Eye space to world space
	glm::mat4 invViewMat = glm::inverse(camera->m_viewMat);
	glm::vec4 rayWorld = invViewMat * eyeCoords;
	rayDirection = glm::normalize(glm::vec3(rayWorld));


}

//Check if ray intersects with Axis Aligned Bounding Box
bool CheckAABBIntersect(glm::vec3 _origin, glm::vec3 _direction, glm::vec3 _boxPosition, glm::vec3 _boxExtents)
{
	glm::vec3 boxMin = _boxPosition - (_boxExtents/2.0f);
	glm::vec3 boxMax = _boxPosition + (_boxExtents / 2.0f);
	glm::vec3 min = (boxMin - _origin) / _direction;
	glm::vec3 max = (boxMax - _origin) / _direction;
	glm::vec3 high = glm::min(min, max);
	glm::vec3 low = glm::max(min, max);
	float near = std::max(std::max(high.x, high.y), high.z);
	float far = std::min(std::min(low.x, low.y), low.z);
	return near > far;
}

bool stencilEnabled = true, scissorEnabled = true, cullingEnabled = true, pressedLastFrame = false;;


//Update all objects and run the processes
void Update()
{
	//Update all GameObjects
	CObjectController::UpdateObjects();
	CEntityManager::UpdateEntities();
	clothEntity->GetBehaviour<ClothRenderer>()->GetCloth()->AddForce(glm::vec3(0.0f, -0.2f, 0.0f));
	clothEntity->GetBehaviour<ClothRenderer>()->GetCloth()->AddWind(glm::vec3(0.2f, 0.0f, 0.2f));
	//Poll events for GLFW input
	glfwPollEvents();
	MousePassive();
	UpdateMousePicking();


	//Get the current time
	delta_time = current_time;
	current_time = (float)glfwGetTime();
	delta_time = current_time - delta_time;

	//Process mouse input and Mouse picking
	if (glfwGetMouseButton(CObjectController::GetMainWindow(), GLFW_MOUSE_BUTTON_1) == GLFW_PRESS)
	{
		if (CheckAABBIntersect(camera->m_cameraPos, rayDirection, shape_3Dbutton_fwd->Position(), shape_3Dbutton_fwd->Scale()))
		{
			camera->m_cameraPos.z += delta_time * 2.0f;
		}
		if (CheckAABBIntersect(camera->m_cameraPos, rayDirection, shape_3Dbutton_bck->Position(), shape_3Dbutton_bck->Scale()))
		{
			camera->m_cameraPos.z -= delta_time * 2.0f;
		}
	}
	//Perform actions on key press
	if (glfwGetKey(CObjectController::GetMainWindow(), GLFW_KEY_R) && !pressedLastFrame) ResetScene();
	if (glfwGetKey(CObjectController::GetMainWindow(), GLFW_KEY_T) && !pressedLastFrame)
	{
		stencilEnabled = !stencilEnabled;
	}
	if (glfwGetKey(CObjectController::GetMainWindow(), GLFW_KEY_Y) && !pressedLastFrame)
	{
		scissorEnabled = !scissorEnabled;
		if (scissorEnabled)
		{
			glClear(GL_COLOR_BUFFER_BIT);
			glEnable(GL_SCISSOR_TEST);
			glScissor(0, 100, (GLsizei)cfWINDOW_WIDTH(), (GLsizei)cfWINDOW_HEIGHT() - 200);
		}
		else
		{
			glDisable(GL_SCISSOR_TEST);
		}
	}
	if (glfwGetKey(CObjectController::GetMainWindow(), GLFW_KEY_U) && !pressedLastFrame)
	{
		cullingEnabled = !cullingEnabled;
		if (cullingEnabled)
		{
			glEnable(GL_CULL_FACE);
		}
		else
		{
			glDisable(GL_CULL_FACE);
		}
	}
	pressedLastFrame = (glfwGetKey(CObjectController::GetMainWindow(), GLFW_KEY_R) || glfwGetKey(CObjectController::GetMainWindow(), GLFW_KEY_T) || glfwGetKey(CObjectController::GetMainWindow(), GLFW_KEY_Y) || glfwGetKey(CObjectController::GetMainWindow(), GLFW_KEY_U));

	//Make camera follow cube
	camera->m_cameraTargetPos = ppQuad->Position();

	shape_cube->Rotation(shape_cube->Rotation() + glm::vec3(0.0f, delta_time * 2, 0.0f));
	shape_cube->Position(shape_cube->Position() + glm::vec3(0.0f, ((sin(current_time)/4) * delta_time), 0.0f));

	shape_sea->Rotation(shape_sea->Rotation() + glm::vec3(0.0f, 0.0f, delta_time));

	shape_stencilcube->Position(shape_cube->Position());
	shape_stencilcube->Scale(shape_cube->Scale() + glm::vec3(0.2f, 0.2f, 0.2f));
	shape_stencilcube->Rotation(shape_cube->Rotation());

	float groundHeight = Terrain3D::GetTerrain("AucklandHarbor2.raw")->GetHeightFromWorldPos(terrain_auckland->Position(), terrain_auckland->Rotation(), freecam->GetCamera()->m_cameraPos);
	freecam->GetCamera()->m_cameraPos.y = groundHeight + 1.0f;

	

	Lighting::DirectionalLights[0].Direction = glm::vec3(sin(timeOfDay), cos(timeOfDay), 0.0f);
	Lighting::DirectionalLights[0].Color = glm::vec3(1.0f, 0.5f + (((sin(timeOfDay)+1.0f)/2.0f)*0.5f), 0.5f + (((sin(timeOfDay) + 1.0f) / 2.0f) * 0.5f));
	

	//Set BG color
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
}

void RenderGUI()
{
	// ImGUI window creation

	ImGui::Begin("Physics Framework");

	ImGui::Text(std::to_string(freecam->GetCamera()->m_cameraPos.y).c_str());

	ImGui::Text("General Controls:");

	ImGui::Checkbox("Wireframe Mode", &bWireFrameMode);

	ImGui::Combo("Mouse Mode", &selectedMouseMode, mouseModeItems, IM_ARRAYSIZE(mouseModeItems));

	ImGui::Text("Environment");

	ImGui::SliderAngle("Time of Day", &timeOfDay);

	ImGui::Text("Cloth Shape:");

	ImGui::SliderFloat("Cloth Length", &clothLength, 1.0f, 200.0f);

	ImGui::SliderFloat("Cloth Width", &clothWidth, 1.0f, 200.0f);

	ImGui::SliderInt("Number Of Hooks", &numberOfHooks, 0, 20);

	ImGui::SliderFloat("Hook Distance", &hookDistance, 1.0f, 100.0f);

	ImGui::SliderFloat("Cloth Stiffness", &clothStiffness, 0.0f, 1.0f);

	
		if (ImGui::Button("Reset Cloth"))

		{

			clothLength = 60.0f;

			clothWidth = 60.0f;

			numberOfHooks = 3;

			hookDistance = 20.0f;

			clothStiffness = 0.5f;

		}



	ImGui::Text("Object Interation:");

	ImGui::Combo("Selected Object: ", &selectedCollision, collisionItems, IM_ARRAYSIZE(collisionItems));



	ImGui::Text("Wind:");

	ImGui::SliderFloat("Wind Direction (Degrees):", &windDirection, 0.0f, 360.0f);

	ImGui::SliderFloat("Wind Strength:", &windStrength, 0.0f, 100.0f);

		if (ImGui::Button("Reset Wind"))

		{

			windDirection = 0.0f;

			windStrength = 10.0f;

		}



	// Closes the window

	ImGui::End();



	// Render the ImGUI elements

	ImGui::Render();

	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

}

//Render all objects
void Render()
{
	//Clear the buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
	
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
	glEnable(GL_DEPTH_TEST);
	glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Render the skybox
	SceneManager::GetCurrentSkybox()->Render();


	//Render objects

	

	if (stencilEnabled)
	{
		glEnable(GL_STENCIL_TEST);
		glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
		glStencilFunc(GL_ALWAYS, 1, 0xFF);
		glStencilMask(0xFF);
		shape_cube->Render(*freecam->GetCamera(), program_blinnphongfog);
		glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
		glStencilMask(0x00);
		shape_stencilcube->Render(*freecam->GetCamera(), program_reflective);
		glStencilMask(0x00);
		glDisable(GL_STENCIL_TEST);
		glStencilMask(0xFF);
	}
	else
	{
		shape_cube->Render(*freecam->GetCamera(), program_blinnphongfog);
	}
	

	shape_3Dbutton_bck->Render(*freecam->GetCamera(), program_blinnphong);
	shape_3Dbutton_fwd->Render(*freecam->GetCamera(), program_blinnphong);

	
	/*entityTest->GetBehaviour<MeshRenderer>()->Render(freecam->GetCamera());
	childEntityTest->GetBehaviour<MeshRenderer>()->Render(freecam->GetCamera());
	otherEntityTest->GetBehaviour<MeshRenderer>()->Render(freecam->GetCamera());*/
	
	clothEntity->GetBehaviour<ClothRenderer>()->Render(freecam->GetCamera());
	entityTest->GetBehaviour<GeometryRenderer>()->Render(freecam->GetCamera());
	
	//Render the floor

	//shape_seafloor->Render(*freecam->GetCamera(), program_blinnphongfog);
	terrain_auckland->Render(*freecam->GetCamera(), program_blinnphongfog);
	//std::for_each(testcubes.begin(), testcubes.end(), [](Renderable3D r) {r.Render(*freecam->GetCamera(), program_blinnphong); });
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	shape_sea->Render(*freecam->GetCamera(), program_reflectivefog);
	glDisable(GL_BLEND);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glDisable(GL_DEPTH_TEST);
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	//ppQuad->Render(*freecam->GetCamera(), program_texture);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	shape_renderquad->Render(*camera, program_normals);
	if (SceneManager::m_isWireframe)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	else
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	RenderGUI();

	

	//Push buffer to the screen
	glfwSwapBuffers(main_window);
}
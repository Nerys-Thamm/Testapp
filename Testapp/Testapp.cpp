// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
// 
// (c) 2021 Media Design School
//
// File Name   : Testapp.cpp
// Description : Implementation file
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
#include "CameraHolder.h"
#include "MouseLook.h"
#include "CharacterMotor.h"
#include "TessRenderer.h"
#include "ParticleSystem.h"


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
GLuint program_tesselationtest;

//TIME
float current_time;
float delta_time;
float timer;

//CAMERAS
Camera* camera = nullptr;
Camera* playerCam = nullptr;
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

CEntity* geostarEntity = nullptr;

CEntity* tessEntity = nullptr;

CEntity* tessEntityLOD = nullptr;

CEntity* playerEntity = nullptr, *cameraGimbal = nullptr, *cameraHolder = nullptr;
CEntity* snowEntity = nullptr;

//---------------------------------------------------------------
//GUI variables

float timeOfDay = 2.7f;

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
	program_texture = ShaderLoader::CreateProgram("Resources/Shaders/ClipSpace.vert",
		"Resources/Shaders/Texture.frag");

	program_texture_interpolation = ShaderLoader::CreateProgram("Resources/Shaders/ClipSpace.vert",
		"Resources/Shaders/TextureMix.frag");

	program_fixed_color = ShaderLoader::CreateProgram("Resources/Shaders/ClipSpace.vert",
		"Resources/Shaders/FixedColor.frag");

	program_texture_wave = ShaderLoader::CreateProgram("Resources/Shaders/ClipSpace.vert",
		"Resources/Shaders/TextureWave.frag");

	program_worldspace = ShaderLoader::CreateProgram("Resources/Shaders/ClipSpace.vert",
		"Resources/Shaders/Texture.frag");

	program_normals = ShaderLoader::CreateProgram("Resources/Shaders/3D_Normals.vert",
		"Resources/Shaders/3DLight_Phong.frag");

	program_blinnphong = ShaderLoader::CreateProgram("Resources/Shaders/3D_Normals.vert",
		"Resources/Shaders/3DLight_BlinnPhong.frag");

	program_blinnphongrim = ShaderLoader::CreateProgram("Resources/Shaders/3D_Normals.vert",
		"Resources/Shaders/3DLight_BlinnPhongRim.frag");

	program_reflective = ShaderLoader::CreateProgram("Resources/Shaders/3D_Normals.vert",
		"Resources/Shaders/3DLight_Reflective.frag");

	program_reflectiverim = ShaderLoader::CreateProgram("Resources/Shaders/3D_Normals.vert",
		"Resources/Shaders/3DLight_ReflectiveRim.frag");

	program_blinnphongfog = ShaderLoader::CreateProgram("Resources/Shaders/3D_Normals.vert",
		"Resources/Shaders/3DLight_BlinnPhongFog.frag");

	program_reflectivefog = ShaderLoader::CreateProgram("Resources/Shaders/3D_Normals.vert",
		"Resources/Shaders/3DLight_ReflectiveFog.frag");
	
	program_postprocess = ShaderLoader::CreateProgram("Resources/Shaders/3D_Normals.vert",
		"Resources/Shaders/PostProcessing.frag");

	program_geostar = ShaderLoader::CreateProgram("Resources/Shaders/GeoShader.vert",
		"Resources/Shaders/GeoShader.frag", "Resources/Shaders/StarGeoShader.geom");

	program_tesselationtest = ShaderLoader::CreateProgram({
		ShaderLoader::ShaderFile{"Resources/Shaders/TessVert.vert", GL_VERTEX_SHADER},
		ShaderLoader::ShaderFile{"Resources/Shaders/TessFrag.frag", GL_FRAGMENT_SHADER},
		ShaderLoader::ShaderFile{"Resources/Shaders/TessEval.tese", GL_TESS_EVALUATION_SHADER},
		ShaderLoader::ShaderFile{"Resources/Shaders/TessControl.tesc", GL_TESS_CONTROL_SHADER}
		});

	//Cull poly not facing viewport
	glCullFace(GL_BACK);

	//Enable culling
	glEnable(GL_CULL_FACE);


	CreateMaterials();

	//Create camera
	camera = new Camera(cfWINDOW_WIDTH(), cfWINDOW_HEIGHT(), current_time, true);
	playerCam = new Camera(cfWINDOW_WIDTH(), cfWINDOW_HEIGHT(), current_time, true);
	freecam = new FreeCam(main_window, cfWINDOW_WIDTH(), cfWINDOW_HEIGHT(), current_time, true);
	orthocamera = new Camera(cfWINDOW_WIDTH(), cfWINDOW_HEIGHT(), current_time, false);

	manager->SetMainCamera(freecam->GetCamera());

	//Create Skybox
	std::string SkyboxFilepaths[] = {"CloudySky/Right.jpg","CloudySky/Left.jpg","CloudySky/Up.jpg","CloudySky/Down.jpg","CloudySky/Back.jpg","CloudySky/Front.jpg"};
	
	SceneManager::SetCurrentSkybox(new Skybox(playerCam, SkyboxFilepaths));



	//Create Entities

	//Create Geometry Shader Star
	geostarEntity = new CEntity();
	geostarEntity->AddBehaviour<GeometryRenderer>();
	geostarEntity->GetBehaviour<GeometryRenderer>()->SetShader(program_geostar);
	geostarEntity->m_transform.position = glm::vec3(0.0f, 5.5f, 0.0f);
	geostarEntity->AddBehaviour<TestBehaviour>();
	

	//Create Player Entity
	playerEntity = new CEntity();
	playerEntity->AddBehaviour<MeshRenderer>(); //Add a mesh
	playerEntity->GetBehaviour<MeshRenderer>()->SetMesh(Sphere3D::GetMesh(0.5f, 20));
	playerEntity->GetBehaviour<MeshRenderer>()->SetMaterial(Lighting::GetMaterial("EntityTest"));
	playerEntity->GetBehaviour<MeshRenderer>()->SetShader(program_blinnphong);
	playerEntity->GetBehaviour<MeshRenderer>()->SetTexture(TextureLoader::LoadTexture("FlushedNew.png"));
	playerEntity->AddBehaviour<CharacterMotor>(); //Add a Character motor
	playerEntity->GetBehaviour<CharacterMotor>()->SetWindow(main_window);
	playerEntity->AddBehaviour<MouseLook>(); //Add horizontal mouselook rotation
	playerEntity->GetBehaviour<MouseLook>()->SetWindow(main_window, glm::vec2(cfWINDOW_WIDTH(), cfWINDOW_HEIGHT()));
	playerEntity->GetBehaviour<MouseLook>()->SetAxisLockState(false, true);
	
	playerEntity->AddBehaviour<ParticleSystem>();
	playerEntity->GetBehaviour<ParticleSystem>()->SetParticleTexture(TextureLoader::LoadTexture("fire.png"));
	playerEntity->GetBehaviour<ParticleSystem>()->SetParticleCount(4000);
	playerEntity->GetBehaviour<ParticleSystem>()->SetParticleLifetime(1.0f);
	playerEntity->GetBehaviour<ParticleSystem>()->SetParticleSpeed(0.3f);
	playerEntity->GetBehaviour<ParticleSystem>()->SetParticleSize(0.7f);
	playerEntity->GetBehaviour<ParticleSystem>()->SetParticleEndSize(0.0f);

	playerEntity->GetBehaviour<ParticleSystem>()->SetParticleColour(glm::vec4(1.0f, 1.0f, 1.0f, 0.3f));
	playerEntity->GetBehaviour<ParticleSystem>()->SetParticleEndColour(glm::vec4(0.3f, 0.0f, 0.0f, 0.0f));
	playerEntity->GetBehaviour<ParticleSystem>()->SetGravity(-9.0f);
	playerEntity->GetBehaviour<ParticleSystem>()->SetEmissionRate(50.0f);
	playerEntity->GetBehaviour<ParticleSystem>()->Init();
	playerEntity->GetBehaviour<ParticleSystem>()->Start();
	
	//Create a Camera gimbal for vertical third person camera rotation
	cameraGimbal = new CEntity(playerEntity);
	cameraGimbal->AddBehaviour<MouseLook>(); //Add vertical mouselook rotation
	cameraGimbal->GetBehaviour<MouseLook>()->SetWindow(main_window, glm::vec2(cfWINDOW_WIDTH(), cfWINDOW_HEIGHT()));
	cameraGimbal->GetBehaviour<MouseLook>()->SetAxisLockState(true, false);
	cameraGimbal->GetBehaviour<MouseLook>()->SetSensitivity(0.5f);

	//Create a camera holder to hold a camera
	cameraHolder = new CEntity(cameraGimbal);
	cameraHolder->AddBehaviour<CameraHolder>()->SetCamera(playerCam);
	cameraHolder->m_transform.position = glm::vec3(0.0f, 1.0f, -10.0f);

	//Create the Tesselated Quad with LOD
	tessEntityLOD = new CEntity();
	tessEntityLOD->AddBehaviour<TessRenderer>()->SetShader(program_tesselationtest);
	tessEntityLOD->GetBehaviour<TessRenderer>()->SetUseLOD(true);
	tessEntityLOD->m_transform.position = glm::vec3(4.0f, 1.0f, 0.0f);

	//Create the Tesselated Quad without LOD
	tessEntity = new CEntity();
	tessEntity->AddBehaviour<TessRenderer>()->SetShader(program_tesselationtest);
	tessEntity->m_transform.position = glm::vec3(2.0f, 1.0f, 0.0f);
	
	//Create the snow particles

	snowEntity = new CEntity();
	snowEntity->AddBehaviour<GPUParticleSystem>()->Init();


	shape_renderquad = new Renderable3D(Quad3D::GetMesh(), Lighting::GetMaterial("Glossy"));

	//Terrain3D::LoadFromRaw("AucklandHarbor2.raw", 1081, 0.1f, 0.025f); //Load the terrain
	Terrain3D::LoadFromNoise("NoiseTerrain", 1000, 0.5f, 0.5f); //Load the terrain

	terrain_auckland = new Renderable3D(Terrain3D::GetTerrainMesh("NoiseTerrain"), Lighting::GetMaterial("Default"));
	

	


	shape_seafloor = new Renderable3D(Quad3D::GetMesh(), Lighting::GetMaterial("Default"));
	shape_sea = new Renderable3D(Quad3D::GetMesh(), Lighting::GetMaterial("Glossy"));

	shape_seafloor->Position(glm::vec3(0.0f, 3.5f, 0.0f));
	shape_seafloor->Scale(glm::vec3(100.0f, 100.0f, 1.0f));
	shape_seafloor->Rotation(glm::vec3(271.0f, 0.0f, 0.0f));

	shape_sea->Position(glm::vec3(0.0f, 1.1f, 0.0f));
	shape_sea->Scale(glm::vec3(500.0f, 500.0f, 1.0f));
	shape_sea->Rotation(glm::vec3(270.0f, 0.0f, 0.0f));

	
	terrain_auckland->Position(glm::vec3(0.0f,0.0f, 0.0f));
	terrain_auckland->Scale(glm::vec3(1.0f, 1.0f, 1.0f));
	terrain_auckland->Rotation(glm::vec3(0.0f, 0.0f, 0.0f));
	


	


	//Set textures of objects

	terrain_auckland->AddTexture(TextureLoader::LoadTexture("snow2.jpg"));
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


	
	Lighting::DirectionalLights[0].Direction = glm::vec3(-0.5f, 0.1f, 0.5f);
	Lighting::DirectionalLights[0].Color = glm::vec3(1.0f, 1.0f, 1.0f);
	Lighting::DirectionalLights[0].AmbientStrength = 0.55f;
	Lighting::DirectionalLights[0].SpecularStrength = 0.4f;

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

	
	shape_seafloor = new Renderable3D(Quad3D::GetMesh(), Lighting::GetMaterial("Default"));
	shape_sea = new Renderable3D(Quad3D::GetMesh(), Lighting::GetMaterial("Glossy"));
	shape_seafloor->Position(glm::vec3(0.0f, 3.5f, 0.0f));
	shape_seafloor->Scale(glm::vec3(100.0f, 100.0f, 1.0f));
	shape_seafloor->Rotation(glm::vec3(271.0f, 0.0f, 0.0f));
	shape_sea->Position(glm::vec3(0.0f, 4.0f, 0.0f));
	shape_sea->Scale(glm::vec3(100.0f, 100.0f, 1.0f));
	shape_sea->Rotation(glm::vec3(270.0f, 0.0f, 0.0f));


	//Set textures of objects
	
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
	
	//Update all Entities in the new Entity System
	CEntityManager::UpdateEntities();
	
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
		/*if (CheckAABBIntersect(camera->m_cameraPos, rayDirection, shape_3Dbutton_fwd->Position(), shape_3Dbutton_fwd->Scale()))
		{
			camera->m_cameraPos.z += delta_time * 2.0f;
		}
		if (CheckAABBIntersect(camera->m_cameraPos, rayDirection, shape_3Dbutton_bck->Position(), shape_3Dbutton_bck->Scale()))
		{
			camera->m_cameraPos.z -= delta_time * 2.0f;
		}*/
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
	shape_sea->Rotation(shape_sea->Rotation() + glm::vec3(0.0f, 0.0f, delta_time));

	
	//Make Players y position conform to terrain
	float groundHeight = Terrain3D::GetTerrain("NoiseTerrain")->GetHeightFromWorldPos(terrain_auckland->Position(), terrain_auckland->Rotation(), playerEntity->m_transform.position);
	playerEntity->m_transform.position.y = groundHeight + 0.5f;

	//Set lighting
	Lighting::DirectionalLights[0].Direction = glm::vec3(sin(timeOfDay), cos(timeOfDay), 0.0f);
	Lighting::DirectionalLights[0].Color = glm::vec3(1.0f, 0.5f + (((sin(timeOfDay)+1.0f)/2.0f)*0.5f), 0.5f + (((sin(timeOfDay) + 1.0f) / 2.0f) * 0.5f));
	

	//Set BG color
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
}

void RenderGUI()
{
	// ImGUI window creation

	ImGui::Begin("Terrain Summative");

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
	
	//Bind the framebuffer so its drawn to
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
	glEnable(GL_DEPTH_TEST);
	glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Render the skybox
	SceneManager::GetCurrentSkybox()->Render();


	//Render objects
	

	playerEntity->GetBehaviour<MeshRenderer>()->Render(playerCam);
	
	terrain_auckland->Render(*playerCam, program_blinnphongfog);
	
	snowEntity->GetBehaviour<GPUParticleSystem>()->Render(*playerCam);
	
	
	
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	shape_sea->Render(*playerCam, program_reflectivefog);
	glDisable(GL_BLEND);
	playerEntity->GetBehaviour<ParticleSystem>()->Render(*playerCam);
	glBindFramebuffer(GL_FRAMEBUFFER, 0); //Unbind the framebuffer
	glDisable(GL_DEPTH_TEST);
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	shape_renderquad->Render(*camera, program_postprocess); //Render the quad with the post processing
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
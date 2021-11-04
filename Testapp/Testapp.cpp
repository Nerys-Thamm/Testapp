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
Camera* clothCam = nullptr;



//MESHES



//BUTTONS


//SHAPES
Renderable3D* shape_renderquad = nullptr;

Shape2D* ppQuad = nullptr;

//Audio
Audiosystem* audio_main = nullptr;

//PostProcessing
GLuint renderTexture;
GLuint frameBuffer;


CEntity* clothEntity = nullptr;
CEntity* ballEntity = nullptr;

CEntity* floorQuadEntity = nullptr;
CEntity* railEntity = nullptr;

//---------------------------------------------------------------
//GUI variables
bool bWireFrameMode = false;
float clothLength = 60.0f;
float clothWidth = 60.0f;
float clothRotation = 0.0f;

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
bool dropped = false;

//----------------------------------------------------

void ShutDown()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	delete clothEntity;
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
	clothCam = new Camera(cfWINDOW_WIDTH(), cfWINDOW_HEIGHT(), current_time, true);
	freecam = new FreeCam(main_window, cfWINDOW_WIDTH(), cfWINDOW_HEIGHT(), current_time, true);
	orthocamera = new Camera(cfWINDOW_WIDTH(), cfWINDOW_HEIGHT(), current_time, false);

	manager->SetMainCamera(camera);

	//Create Skybox
	std::string SkyboxFilepaths[] = {"MountainOutpost/Right.jpg","MountainOutpost/Left.jpg","MountainOutpost/Up.jpg","MountainOutpost/Down.jpg","MountainOutpost/Back.jpg","MountainOutpost/Front.jpg"};
	
	SceneManager::SetCurrentSkybox(new Skybox(freecam->GetCamera(), SkyboxFilepaths));



	//Create objects
	clothEntity = new CEntity();
	std::shared_ptr<ClothRenderer> clothrenderer = clothEntity->AddBehaviour<ClothRenderer>();
	clothrenderer->SetCloth(new Cloth(glm::vec2(clothWidth, clothLength), glm::ivec2(50, 50), 1000, 4, clothStiffness));
	clothrenderer->SetMaterial(Lighting::GetMaterial("EntityTest"));
	clothrenderer->SetShader(program_blinnphong);
	clothrenderer->SetTexture(TextureLoader::LoadTexture("tartan.jpg"));
	clothEntity->m_transform.position = glm::vec3(-50.0f, 30.0f, -100.0f);
	clothEntity->m_transform.rotation = glm::vec3(0.0f, 0.0f, 0.0f);

	shape_renderquad = new Renderable3D(Quad3D::GetMesh(), Lighting::GetMaterial("Default"));

	ballEntity = new CEntity();
	std::shared_ptr<MeshRenderer> meshrenderer = ballEntity->AddBehaviour<MeshRenderer>();
	meshrenderer->SetMaterial(Lighting::GetMaterial("EntityTest"));
	meshrenderer->SetShader(program_blinnphong);
	meshrenderer->SetTexture(TextureLoader::LoadTexture("Rayman.jpg"));
	meshrenderer->SetMesh(Sphere3D::GetMesh(10.0f, 10));
	ballEntity->m_transform.position = glm::vec3(0.0f, 0.0f, -120.0f);

	floorQuadEntity = new CEntity();
	floorQuadEntity->AddBehaviour<MeshRenderer>();
	floorQuadEntity->GetBehaviour<MeshRenderer>()->SetMesh(Quad3D::GetMesh());
	floorQuadEntity->GetBehaviour<MeshRenderer>()->SetMaterial(Lighting::GetMaterial("Default"));
	floorQuadEntity->GetBehaviour<MeshRenderer>()->SetShader(program_blinnphong);
	floorQuadEntity->GetBehaviour<MeshRenderer>()->SetTexture(TextureLoader::LoadTexture("ShowerTiles.jpg"));
	floorQuadEntity->m_transform.rotation = glm::vec3(-90.0f, 0.0f, 0.0f);
	floorQuadEntity->m_transform.scale = glm::vec3(100.0f, 100.0f, 1.0f);
	floorQuadEntity->m_transform.position = glm::vec3(0.0f, -20.1f, -100.0f);

	railEntity = new CEntity();
	railEntity->AddBehaviour<MeshRenderer>();
	railEntity->GetBehaviour<MeshRenderer>()->SetMesh(Quad3D::GetMesh());
	railEntity->GetBehaviour<MeshRenderer>()->SetMaterial(Lighting::GetMaterial("Default"));
	railEntity->GetBehaviour<MeshRenderer>()->SetShader(program_blinnphong);
	railEntity->GetBehaviour<MeshRenderer>()->SetTexture(TextureLoader::LoadTexture("ShowerTiles.jpg"));
	railEntity->m_transform.scale = glm::vec3(100.0f, 1.0f, 1.0f);
	railEntity->m_transform.position = glm::vec3(0.0f, 30.0f, -99.0f);





	//Set position of Cameras
	camera->m_cameraPos = glm::vec3(0.0f, 0.0f, 1.0f);
	clothCam->m_cameraPos = glm::vec3(0.0f, 0.0f, 1.0f);
	
	//camera->m_cameraTargetPos = shape_renderquad->Position();
	//camera->m_lookAtTarget = true;
	orthocamera->m_cameraPos = glm::vec3(0.0f, 0.0f, 1.0f);
	freecam->GetCamera()->m_cameraPos.y = 5.0f;
	



	//Setup Audio
	audio_main = Audiosystem::GetInstance();



	//Download sounds if they dont already exist
	audio_main->AddSound("Track_BeachAmbience.mp3", "Track_BeachAmbience");
	
	
	//Start playing background track
	audio_main->PlaySound("Track_BeachAmbience", 0.5f, true);


	
	Lighting::DirectionalLights[0].Direction = glm::vec3(0.0f, -1.0f, -1.0f);
	Lighting::DirectionalLights[0].Color = glm::vec3(0.5f, 0.5f, 0.5f);
	Lighting::DirectionalLights[0].AmbientStrength = 0.25f;
	Lighting::DirectionalLights[0].SpecularStrength = 1.0f;

	//Post Processing
	TextureLoader::CreateFrameBuffer((int)cfWINDOW_WIDTH(), (int)cfWINDOW_HEIGHT(), renderTexture, frameBuffer);
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
	Lighting::DirectionalLights[0].SpecularStrength = 0.3f;
}

float mouseY, mouseX;
glm::vec3 rayDirection;

//Get cursor position relative to window
void MousePassive()
{
	double x, y;
	glfwGetCursorPos(CObjectController::GetMainWindow(), &x, &y);
	mouseX = (2.0f * (float)x) / (float)cfWINDOW_WIDTH() - 1.0f;
	mouseY = 1.0f - (2.0f * (float)y) / (float)cfWINDOW_HEIGHT();
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
	if(!dropped)clothEntity->GetBehaviour<ClothRenderer>()->GetCloth()->SetPegDistance(hookDistance);
	
	clothEntity->GetBehaviour<ClothRenderer>()->GetCloth()->AddWind(glm::vec3(sin(windDirection * (M_PI/180.0f)) * windStrength, 0.0f, sin(windDirection * (M_PI / 180.0f)) * windStrength));
	CEntityManager::UpdateEntities();
	clothEntity->GetBehaviour<ClothRenderer>()->GetCloth()->SphereCollision(ballEntity->m_transform.position, 10.0f);
	
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
	//camera->m_cameraTargetPos = ppQuad->Position();



	

	//Lighting::DirectionalLights[0].Direction = glm::vec3(sin(timeOfDay), cos(timeOfDay), 0.0f);
	//Lighting::DirectionalLights[0].Color = glm::vec3(1.0f, 0.5f + (((sin(timeOfDay)+1.0f)/2.0f)*0.5f), 0.5f + (((sin(timeOfDay) + 1.0f) / 2.0f) * 0.5f));
	

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

	ImGui::SliderFloat("Cloth Length", &clothLength, 1.0f, 50.0f);

	ImGui::SliderFloat("Cloth Width", &clothWidth, 1.0f, 200.0f);
	ImGui::SliderFloat("Cloth Rotation", &clothRotation, 0.0f, 360.0f);

	clothEntity->m_transform.rotation = glm::vec3(0.0f, clothRotation, 0.0f);

	ImGui::SliderInt("Number Of Hooks", &numberOfHooks, 0, (int)floorf(clothWidth));

	ImGui::SliderFloat("Hook Distance", &hookDistance, 1.0f, 100.0f);

	ImGui::SliderFloat("Cloth Stiffness", &clothStiffness, 0.0f, 1.0f);

	
		if (ImGui::Button("Reset Cloth Params"))

		{

			clothLength = 30.0f;

			clothWidth = 30.0f;

			numberOfHooks = 4;

			hookDistance = 60.0f;

			clothStiffness = 0.5f;

			
		}
		if (ImGui::Button("Create Cloth with Params"))
		{
			clothEntity->GetBehaviour<ClothRenderer>()->SetCloth(new Cloth(glm::vec2(clothWidth, clothLength), glm::ivec2(50, 50), 1000, numberOfHooks, clothStiffness));
			dropped = false;
		}
		if (ImGui::Button("Drop the Cloth"))
		{
			clothEntity->GetBehaviour<ClothRenderer>()->GetCloth()->DropCloth();
			dropped = true;
		}


	ImGui::Text("Object Interation:");

	ImGui::Combo("Selected Object: ", &selectedCollision, collisionItems, IM_ARRAYSIZE(collisionItems));



	ImGui::Text("Wind:");

	ImGui::SliderFloat("Wind Direction (Degrees):", &windDirection, 0.0f, 360.0f);

	ImGui::SliderFloat("Wind Strength:", &windStrength, 0.0f, 10.0f);

		if (ImGui::Button("Reset Wind"))

		{

			windDirection = 0.0f;

			windStrength = 1.0f;

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
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Render the skybox
	//SceneManager::GetCurrentSkybox()->Render();


	//Render objects

	

	floorQuadEntity->GetBehaviour<MeshRenderer>()->Render(camera);
	clothEntity->GetBehaviour<ClothRenderer>()->Render(camera);
	ballEntity->GetBehaviour<MeshRenderer>()->Render(camera);
	railEntity->GetBehaviour<MeshRenderer>()->Render(camera);
	
	//Render the floor

	

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
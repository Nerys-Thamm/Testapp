#pragma once
#include <iostream>
#include <glew.h>
#include <glfw3.h>
#include <vector>

class CGameObject
{
	friend class CObjectController;
public:
	// ********************************************************************************
	/// <summary>
	/// This method is called once every frame if it is defined in a derived class
	/// </summary>
	/// <param name="_fDeltaTime">Type: Float, Time since last frame</param>
	// ********************************************************************************
	virtual void Update(float _fDeltaTime) {};

	// ********************************************************************************
	/// <summary>
	/// This method is called at fixed intervals if it is defined in a derived class
	/// </summary>
	// ********************************************************************************
	virtual void FixedUpdate() {};

	// ********************************************************************************
	/// <summary>
	/// This method is called after all other update methods are complete if it is defined in a derived class
	/// </summary>
	/// <param name="_fDeltaTime">Type: Float, Time since last frame</param>
	// ********************************************************************************
	virtual void LateUpdate(float _fDeltaTime) {};

	/// <summary>Stores whether the derived object is enabled</summary>
	bool bIsEnabled = true;
protected:
	// ********************************************************************************
	/// <summary>
	/// <para>
	/// Constructor for Class: CGameObject
	/// Adds the instance to the pool of updatable objects
	/// </para>
	/// </summary>
	/// <returns></returns>
	// ********************************************************************************
	CGameObject();

	// ********************************************************************************
	/// <summary>
	/// <para>
	/// Destructor for Class: CGameObject
	/// Removes the instance from the pool of updatable objects
	/// </para>
	/// </summary>
	/// <returns></returns>
	// ********************************************************************************
	virtual ~CGameObject();
private:
	/// <summary>Pointer to the next CGameObject in the linked list of updatable objects</summary>
	CGameObject* m_pNext;

	/// <summary>Pointer to the previous CGameObject in the linked list of updatable objects</summary>
	CGameObject* m_pPrev;
};
// --------------------------------------------------------------------------------
/// <summary>
/// A static class that controls the functionality of CGameObject derived classes
/// </summary>
// --------------------------------------------------------------------------------
class CObjectController
{
	friend class CGameObject;
public:

	// ********************************************************************************
	/// <summary>
	/// <para>
	/// Function name: UpdateObjects
	/// Function is part of Class: CObjectController
	/// Calls the update methods defined by classes inheriting from CGameObject
	/// Must be called within the game loop once every frame.
	/// </para>
	/// </summary>
	// ********************************************************************************
	static void UpdateObjects();

	// ********************************************************************************
	/// <summary>
	/// <para>
	/// Function name: Update
	/// Function is part of Class: CObjectController
	/// Calls the Update method on all instances of classes that inherit from CGameObject and define the method
	/// Called once every frame
	/// </para>
	/// </summary>
	/// <param name="_fDeltaTime"><para>
	/// Type: float, time since last frame
	/// </para></param>
	// ********************************************************************************
	static void Update(float _fDeltaTime);

	// ********************************************************************************
	/// <summary>
	/// <para>
	/// Function name: FixedUpdate
	/// Function is part of Class: CObjectController
	/// Calls the FixedUpdate method on instances of classes that inherit from CGameObject and define the method
	/// Called at fixed intervals independant of framerate
	/// </para>
	/// </summary>
	// ********************************************************************************
	static void FixedUpdate();

	// ********************************************************************************
	/// <summary>
	/// <para>
	/// Function name: LateUpdate
	/// Function is part of Class: CObjectController
	/// Calls the LateUpdate method on instances of classes that inherit from CGameObject and define the method
	/// Called once every frame, after all other update methods have finished
	/// </para>
	/// </summary>
	/// <param name="_fDeltaTime"><para>
	/// Type: float, time since last frame
	/// </para></param>
	// ********************************************************************************
	static void LateUpdate(float _fDeltaTime);

	static void SetMainWindow(GLFWwindow* _window);

	static GLFWwindow* GetMainWindow();
private:

	/// <summary>The amount of time since the last frame</summary>
	static float deltaTime;

	static float currentTime;

	/// <summary>The fixed interval to call FixedUpdate on</summary>
	static float fixedTime;

	// ********************************************************************************
	/// <summary>
	/// <para>
	/// Function name: AddUpdater
	/// Function is part of Class: CObjectController
	/// Adds a CGameObject derived class instance to the pool of instances to call update methods on
	/// </para>
	/// </summary>
	/// <param name="_updateable"><para>
	/// Type: CGameObject Pointer
	/// </para></param>
	// ********************************************************************************
	static void AddUpdater(CGameObject* _updateable);

	// ********************************************************************************
	/// <summary>
	/// <para>
	/// Function name: RemoveUpdater
	/// Function is part of Class: CObjectController
	/// Removes an instance from the pool of updated objects, called in the destructor
	/// </para>
	/// </summary>
	/// <param name="_updateable"><para>
	/// Type: CGameObject Pointer
	/// </para></param>
	// ********************************************************************************
	static void RemoveUpdater(CGameObject* _updateable);

	// ********************************************************************************
	/// <summary>
	/// <para>
	/// Function name: GetHead
	/// Function is part of Class: CObjectController
	/// </para>
	/// </summary>
	/// <returns>Value of type: CGameObject Pointer</returns>
	// ********************************************************************************
	static CGameObject** GetHead();

	static GLFWwindow* m_mainwindow;
};
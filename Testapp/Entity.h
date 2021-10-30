// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2021 Media Design School
//
// File Name   : GameObject.h
// Description : Header file for CGameObject and CObjectController classes
// Author      : Nerys Thamm
// Mail        : nerys.thamm@mds.ac.nz

#pragma once
#include <glew.h>
#include <glfw3.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <vector>
#define _USE_MATH_DEFINES
#include <cmath>
#include <math.h>
#include <iostream>
#include <algorithm>
#include <string>
#include <type_traits>

class CEntity;

class IBehaviour
{
	friend class CEntity;
public:
	bool m_isEnabled = true;
	const CEntity* m_entity;
protected:
	IBehaviour(CEntity* _entity);
	virtual ~IBehaviour();
private:
	void OnAwake();
	void OnUpdate(float _fDeltaTime);
	void OnFixedUpdate();
	void OnLateUpdate(float _fDeltaTime);
};

class CEntity
{
	friend class CEntityManager;
public:

	struct Transform
	{
		glm::vec3 position;
		glm::vec3 rotation;
		glm::vec3 scale;
		Transform& operator+=(const Transform& rhs)
		{
			position += rhs.position;
			rotation += rhs.rotation;
			if (rotation.x > 360.0f) rotation.x -= 360.0f;
			if (rotation.y > 360.0f) rotation.y -= 360.0f;
			if (rotation.z > 360.0f) rotation.z -= 360.0f;
			if (rotation.x < 0.0f) rotation.x += 360.0f;
			if (rotation.y < 0.0f) rotation.y += 360.0f;
			if (rotation.z < 0.0f) rotation.z += 360.0f;
			scale *= rhs.scale;
		}
		Transform& operator-=(const Transform& rhs)
		{
			position -= rhs.position;
			rotation -= rhs.rotation;
			if (rotation.x > 360.0f) rotation.x -= 360.0f;
			if (rotation.y > 360.0f) rotation.y -= 360.0f;
			if (rotation.z > 360.0f) rotation.z -= 360.0f;
			if (rotation.x < 0.0f) rotation.x += 360.0f;
			if (rotation.y < 0.0f) rotation.y += 360.0f;
			if (rotation.z < 0.0f) rotation.z += 360.0f;
			scale *= (glm::vec3{ 1.0f,1.0f,1.0f } / rhs.scale);
		}
		friend Transform operator+(Transform lhs, const Transform& rhs)
		{
			lhs.position += rhs.position;
			lhs.rotation += rhs.rotation;
			if (lhs.rotation.x > 360.0f) lhs.rotation.x -= 360.0f;
			if (lhs.rotation.y > 360.0f) lhs.rotation.y -= 360.0f;
			if (lhs.rotation.z > 360.0f) lhs.rotation.z -= 360.0f;
			if (lhs.rotation.x < 0.0f) lhs.rotation.x += 360.0f;
			if (lhs.rotation.y < 0.0f) lhs.rotation.y += 360.0f;
			if (lhs.rotation.z < 0.0f) lhs.rotation.z += 360.0f;
			lhs.scale *= rhs.scale;
			return lhs;
		}
		friend Transform operator-(Transform lhs, const Transform& rhs)
		{
			lhs.position -= rhs.position;
			lhs.rotation -= rhs.rotation;
			if (lhs.rotation.x > 360.0f)lhs.rotation.x -= 360.0f;
			if (lhs.rotation.y > 360.0f)lhs.rotation.y -= 360.0f;
			if (lhs.rotation.z > 360.0f)lhs.rotation.z -= 360.0f;
			if (lhs.rotation.x < 0.0f)lhs.rotation.x += 360.0f;
			if (lhs.rotation.y < 0.0f)lhs.rotation.y += 360.0f;
			if (lhs.rotation.z < 0.0f)lhs.rotation.z += 360.0f;
			lhs.scale *= (glm::vec3{ 1.0f,1.0f,1.0f } / rhs.scale);
			return lhs;
		}
	};

	/// <summary>Stores whether the derived object is enabled</summary>
	bool m_isEnabled = true;

	Transform m_transform;



	template <typename B, typename std::enable_if<std::is_base_of<IBehaviour, B>::value>::type* = nullptr>
	B* GetBehaviour()
	{
		for (int i = 0; i < m_behaviours.size(); i++)
		{
			if (static_cast<B*>(m_behaviours[i].get()))
			{
				return static_cast<B*>(m_behaviours[i].get());
			}
		}
		return nullptr;
	}
	template <typename B, typename std::enable_if<std::is_base_of<IBehaviour, B>::value>::type* = nullptr>
	std::shared_ptr<B> GetBehaviourInChildren(int _maxDepth = 1)
	{
		if (_maxDepth == 0) return nullptr;
		std::shared_ptr<B> val = GetBehaviour<B>();
		if (val) return val;
		for (int i = 0; i < m_children.size(); i++)
		{
			val = m_children[i]->GetBehaviourInChildren<B>(_maxDepth-1);
			if (val) return val;
		}
		return nullptr;
	}
	template <typename B, typename std::enable_if<std::is_base_of<IBehaviour, B>::value>::type* = nullptr>
	std::shared_ptr<B> AddBehaviour()
	{
		if (!GetBehaviour<B>())
		{
			std::shared_ptr<B> newBehaviour(new B(this));
			m_behaviours.push_back(newBehaviour);
			return newBehaviour;
		}
		else
		{
			std::cout << "ERROR! Entity already has Behaviour of Type: " << typeid(B).name() << std::endl;
			return nullptr;
		}
	}



	// ********************************************************************************
	/// <summary>
	/// <para>
	/// Constructor for Class: CEntity
	/// Adds the instance to the pool of updatable objects
	/// </para>
	/// </summary>
	/// <returns></returns>
	// ********************************************************************************
	CEntity();

	CEntity(CEntity* _parent);

	// ********************************************************************************
	/// <summary>
	/// <para>
	/// Destructor for Class: CGameObject
	/// Removes the instance from the pool of updatable objects
	/// </para>
	/// </summary>
	/// <returns></returns>
	// ********************************************************************************
	~CEntity();

	Transform m_globalTransform;
	
protected:

private:
	// ********************************************************************************
	/// <summary>
	/// This method is called once every frame if it is defined in a derived class
	/// </summary>
	/// <param name="_fDeltaTime">Type: Float, Time since last frame</param>
	// ********************************************************************************
	void Update(float _fDeltaTime);

	// ********************************************************************************
	/// <summary>
	/// This method is called at fixed intervals if it is defined in a derived class
	/// </summary>
	// ********************************************************************************
	void FixedUpdate();

	// ********************************************************************************
	/// <summary>
	/// This method is called after all other update methods are complete if it is defined in a derived class
	/// </summary>
	/// <param name="_fDeltaTime">Type: Float, Time since last frame</param>
	// ********************************************************************************
	void LateUpdate(float _fDeltaTime);

	

	/// <summary>Pointer to the next CGameObject in the linked list of updatable objects</summary>
	CEntity* m_pNext;

	/// <summary>Pointer to the previous CGameObject in the linked list of updatable objects</summary>
	CEntity* m_pPrev;

	CEntity* m_parent = nullptr;

	std::vector<std::shared_ptr<CEntity>> m_children;

	std::vector<std::shared_ptr<IBehaviour>> m_behaviours;
};
// --------------------------------------------------------------------------------
/// <summary>
/// A static class that controls the functionality of CGameObject derived classes
/// </summary>
// --------------------------------------------------------------------------------
class CEntityManager
{
	friend class CEntity;
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
	static void UpdateEntities();

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
	static void AddUpdater(CEntity* _updateable);

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
	static void RemoveUpdater(CEntity* _updateable);

	// ********************************************************************************
	/// <summary>
	/// <para>
	/// Function name: GetHead
	/// Function is part of Class: CObjectController
	/// </para>
	/// </summary>
	/// <returns>Value of type: CGameObject Pointer</returns>
	// ********************************************************************************
	static CEntity** GetHead();

};
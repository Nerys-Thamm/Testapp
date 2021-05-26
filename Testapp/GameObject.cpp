#include "GameObject.h"

CGameObject::CGameObject() : m_pNext(nullptr), m_pPrev(nullptr), bIsEnabled(true)
{
	CObjectController::AddUpdater(this); //Add the object to the list of Updatables
}

CGameObject::~CGameObject()
{
	CObjectController::RemoveUpdater(this); //Remove the object from the list of Updatables
}

//CObjectController


float CObjectController::deltaTime = 0.0f;
float CObjectController::fixedTime = 0.0f;
float CObjectController::currentTime = 0.0f;

void CObjectController::UpdateObjects()
{

	//Get the current time
	deltaTime = currentTime;
	currentTime = (float)glfwGetTime();
	deltaTime = currentTime - deltaTime;

	deltaTime = 
	fixedTime += deltaTime; //Add the delta time to fixedTime.
	Update(deltaTime); //Call the Update method on derived classes that have implemented it
	while (fixedTime >= (1.0f / 60.0f)) //Call FixedUpdate a number of times determined by fixedTime
	{
		FixedUpdate();
		fixedTime -= (1.0f / 60.0f);
	}
	LateUpdate(deltaTime + ((float)glfwGetTime()-currentTime)); //Call LastUpdate on classes that have implemented it
}

void CObjectController::Update(float _fDeltaTime)
{
	//Traverse until the starting node is reached
	CGameObject* pHead = *GetHead();
	CGameObject* pCurrent = pHead;
	if (pCurrent)
	{
		do
		{
			if (pCurrent->bIsEnabled)
			{
				pCurrent->Update(_fDeltaTime); //Call update on each object
			}
			pCurrent = pCurrent->m_pNext;
		} while (pCurrent != pHead);
	}
}

void CObjectController::FixedUpdate()
{
	//Traverse until the starting node is reached
	CGameObject* pHead = *GetHead();
	CGameObject* pCurrent = pHead;
	if (pCurrent)
	{
		do
		{
			if (pCurrent->bIsEnabled)
			{
				pCurrent->FixedUpdate(); //Call FixedUpdate on each object
			}
			pCurrent = pCurrent->m_pNext;
		} while (pCurrent != pHead);
	}
}

void CObjectController::LateUpdate(float _fDeltaTime)
{
	//Traverse until the starting node is reached
	CGameObject* pHead = *GetHead();
	CGameObject* pCurrent = pHead;
	if (pCurrent)
	{
		do
		{
			if (pCurrent->bIsEnabled)
			{
				pCurrent->LateUpdate(_fDeltaTime); //Call LateUpdate on each object
			}
			pCurrent = pCurrent->m_pNext;
		} while (pCurrent != pHead);
	}
}

void CObjectController::AddUpdater(CGameObject* _updateable)
{
	CGameObject** m_pHead = GetHead(); //get the head node
	if (!*m_pHead) //if the linked list is empty
	{
		_updateable->m_pNext = _updateable; //link the new node to itself
		_updateable->m_pPrev = _updateable;
	}
	else
	{
		(*m_pHead)->m_pPrev->m_pNext = _updateable; //Link the new node behind the Head
		_updateable->m_pPrev = (*m_pHead)->m_pPrev;
		(*m_pHead)->m_pPrev = _updateable;
		_updateable->m_pNext = (*m_pHead);
	}
	*m_pHead = _updateable; //make the new node into the new Head
}

void CObjectController::RemoveUpdater(CGameObject* _updateable)
{
	CGameObject** m_pHead = GetHead(); //get the head node
	*m_pHead = _updateable->m_pNext; //make the node in front of the one being removed the head node
	if (*m_pHead != _updateable) //if the node being removed isnt the last one
	{
		_updateable->m_pPrev->m_pNext = _updateable->m_pNext; //link the nodes before and after the node being removed to eachother
		_updateable->m_pNext->m_pPrev = _updateable->m_pPrev;
	}
	else
	{
		*m_pHead = nullptr; //make the head node null pointer
	}
	_updateable->m_pNext = nullptr; //Delink the removed object
	_updateable->m_pPrev = nullptr;
}

CGameObject** CObjectController::GetHead()
{
	static CGameObject* pHead = nullptr;
	return &pHead; //Return a reference to the current head node
}

GLFWwindow* CObjectController::m_mainwindow;

void CObjectController::SetMainWindow(GLFWwindow* _window)
{
	m_mainwindow = _window;
}

GLFWwindow* CObjectController::GetMainWindow()
{
	return m_mainwindow;
}

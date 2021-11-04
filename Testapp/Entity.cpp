#include "Entity.h"

void CEntityManager::AddUpdater(CEntity* _updateable)
{
	CEntity** m_pHead = GetHead(); //get the head node
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

void CEntityManager::RemoveUpdater(CEntity* _updateable)
{
	CEntity** m_pHead = GetHead(); //get the head node
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



CEntity::CEntity() : m_pNext(nullptr), m_pPrev(nullptr), m_isEnabled(true)
{
	CEntityManager::AddUpdater(this);
}

CEntity::CEntity(CEntity* _parent) : m_parent(_parent), m_pNext(nullptr), m_pPrev(nullptr)
{
	m_parent->m_children.push_back(std::shared_ptr<CEntity>(this));
}

CEntity::~CEntity()
{
	m_children.clear();
	m_behaviours.clear();
	if (m_parent == nullptr)
	{
		CEntityManager::RemoveUpdater(this);
	}
	else
	{
		m_parent->m_children.erase(std::remove_if(m_parent->m_children.begin(), m_parent->m_children.end(), [&](std::shared_ptr<CEntity> e) {return e == std::shared_ptr<CEntity>(this); }));
	}
}

void CEntity::Update(float _fDeltaTime)
{
	if (m_parent == nullptr)
	{
		m_globalTransform = m_transform;
	}
	else
	{
		m_globalTransform = m_transform + m_parent->m_globalTransform;
	}
	for (int i = 0; i < (int)m_behaviours.size(); i++)
	{
		m_behaviours[i]->OnUpdate(_fDeltaTime);
	}
	for (int i = 0; i < (int)m_children.size(); i++)
	{
		m_children[i]->Update(_fDeltaTime);
	}
}

void CEntity::FixedUpdate()
{
	for (int i = 0; i < (int)m_behaviours.size(); i++)
	{
		m_behaviours[i]->OnFixedUpdate();
	}
	for (int i = 0; i < (int)m_children.size(); i++)
	{
		m_children[i]->FixedUpdate();
	}
	
}

void CEntity::LateUpdate(float _fDeltaTime)
{
	for (int i = 0; i < (int)m_behaviours.size(); i++)
	{
		m_behaviours[i]->OnLateUpdate(_fDeltaTime);
	}
}

IBehaviour::IBehaviour(CEntity& _entity) : m_entity(_entity)
{
	
}

IBehaviour::~IBehaviour()
{

}

void IBehaviour::OnAwake()
{
}

void IBehaviour::OnUpdate(float _fDeltaTime)
{
}

void IBehaviour::OnFixedUpdate()
{
}

void IBehaviour::OnLateUpdate(float _fDeltaTime)
{
}

void CEntityManager::UpdateEntities()
{
	//Get the current time
	deltaTime = currentTime;
	currentTime = (float)glfwGetTime();
	deltaTime = currentTime - deltaTime;

	fixedTime += deltaTime; //Add the delta time to fixedTime.
	Update(deltaTime); //Call the Update method on derived classes that have implemented it
	while (fixedTime >= (1.0f / 60.0f)) //Call FixedUpdate a number of times determined by fixedTime
	{
		FixedUpdate();
		fixedTime -= (1.0f / 60.0f);
	}
	LateUpdate(deltaTime + ((float)glfwGetTime() - currentTime)); //Call LastUpdate on classes that have implemented it
}

void CEntityManager::Update(float _fDeltaTime)
{
	//Traverse until the starting node is reached
	CEntity* pHead = *GetHead();
	CEntity* pCurrent = pHead;
	if (pCurrent)
	{
		do
		{
			if (pCurrent->m_isEnabled)
			{
				pCurrent->Update(_fDeltaTime); //Call update on each object
			}
			pCurrent = pCurrent->m_pNext;
		} while (pCurrent != pHead);
	}
}

void CEntityManager::FixedUpdate()
{
	//Traverse until the starting node is reached
	CEntity* pHead = *GetHead();
	CEntity* pCurrent = pHead;
	if (pCurrent)
	{
		do
		{
			if (pCurrent->m_isEnabled)
			{
				pCurrent->FixedUpdate(); //Call update on each object
			}
			pCurrent = pCurrent->m_pNext;
		} while (pCurrent != pHead);
	}
}

void CEntityManager::LateUpdate(float _fDeltaTime)
{
	//Traverse until the starting node is reached
	CEntity* pHead = *GetHead();
	CEntity* pCurrent = pHead;
	if (pCurrent)
	{
		do
		{
			if (pCurrent->m_isEnabled)
			{
				pCurrent->LateUpdate(_fDeltaTime); //Call update on each object
			}
			pCurrent = pCurrent->m_pNext;
		} while (pCurrent != pHead);
	}
}



CEntity** CEntityManager::GetHead()
{
	static CEntity* pHead = nullptr;
	return &pHead; //Return a reference to the current head node
}

float CEntityManager::deltaTime;

float CEntityManager::currentTime;


float CEntityManager::fixedTime;

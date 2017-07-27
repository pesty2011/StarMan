#include <assert.h>

#include "EntityManager.h"




EntityManager::EntityManager()
{
}


EntityManager::~EntityManager()
{
	Clear();
}



/* ----------------------------------------------------------------------------
	Summary:
	This returns a instance of the entity manager.  this is a singleton
	pattern so there is only one instance of the entity manager class.

---------------------------------------------------------------------------- */
EntityManager* EntityManager::Instance()
{
	static EntityManager instance;
	return &instance;
}



/* ----------------------------------------------------------------------------
	Summary:
	clear out all of the entities from the entity manager

---------------------------------------------------------------------------- */
void EntityManager::Clear()
{
	EntityMap::iterator it;

	for (it = m_EntityMap.begin(); it != m_EntityMap.end(); it++)
	{
		CBaseEntity* entity = it->second;
		delete entity;
	}
	m_EntityMap.clear();
}





/* ----------------------------------------------------------------------------
	Summary:
	search the entity list and return the pointer to the entity back
	to the caller.

	Parameters:
	[in] name : string containing name of the entity

	Output:
	[ret] pointer to BaseEntity, otherwise NULL.

---------------------------------------------------------------------------- */
CBaseEntity* EntityManager::FindEntity(const string& name)
{
	EntityMap::iterator it;

	for (it = m_EntityMap.begin(); it != m_EntityMap.end(); it++)
	{
		string n = it->second->GetName();
		if (name == n)
		{
			return it->second;
		}
	}
	return NULL;
}



/* ----------------------------------------------------------------------------
	Summary:
	Search the entity list and return the pointer to the entity back
	to the caller.

	Parameters:
	[in] id : unique id associated with the entity


	Output:
	[ret] pointer to BaseEntity, otherwise NULL.

---------------------------------------------------------------------------- */
CBaseEntity* EntityManager::FindEntity(int id)
{
	EntityMap::const_iterator it = m_EntityMap.find(id);
	
	if (it != m_EntityMap.end())
	{
		return it->second;
	}
	return NULL;
}


/* ----------------------------------------------------------------------------
	Summary:
	delete an entity from the manager

	Parameters:
	[in] entity : pointer to the entity to be removed

---------------------------------------------------------------------------- */
bool EntityManager::AddEntity(CBaseEntity* entity)
{
	m_EntityMap.insert(std::make_pair(entity->GetID(), entity));

	return true;
}



/* ----------------------------------------------------------------------------
	Summary:
	delete an entity from the manager

	Parameters:
	[in] entity : pointer to the entity to be removed

---------------------------------------------------------------------------- */
void EntityManager::DelEntity(CBaseEntity* entity)
{
	m_EntityMap.erase(m_EntityMap.find(entity->GetID()));
}




/* ----------------------------------------------------------------------------
	Summary:
	delete an entity from the manager

	Parameters:
	[in] name : string containing the name of the entity to remove

---------------------------------------------------------------------------- */
void EntityManager::DelEntity(const string& name)
{
	EntityMap::iterator it;

	for (it = m_EntityMap.begin(); it != m_EntityMap.end(); it++)
	{
		string n = it->second->GetName();
		if (name == n)
		{
			DelEntity(it->second);
			return;
		}
	}
}



/* ----------------------------------------------------------------------------
	Summary:
	Updates all of the entities inside the entity manager

	Parameters:
	[in] dTime : delta time since last frame in milliseconds


---------------------------------------------------------------------------- */
void EntityManager::Update(float dTime)
{
	EntityMap::iterator it;
	for (it = m_EntityMap.begin(); it != m_EntityMap.end(); it++)
	{
		CBaseEntity* entity = it->second;
		entity->Update(dTime);
	}
}



/* ----------------------------------------------------------------------------
	Summary:
	Displays all of the entities contained in the entity map

---------------------------------------------------------------------------- */
void EntityManager::Display()
{
	EntityMap::iterator it;
	for (it = m_EntityMap.begin(); it != m_EntityMap.end(); it++)
	{
		it->second->Display();
	}
}



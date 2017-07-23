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
	this returns a instance of the entity manager.  this is a singleton
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
	// loop through all of the entities and delete them from the system
	for (std::vector<CBaseEntity*>::iterator it = entities.begin(); it != entities.end(); it++)
	{
		CBaseEntity* entity = *it;
		delete entity;
	}
	entities.clear();
}




/* ----------------------------------------------------------------------------
	Summary:
	search the entity list and return the pointer to the entity back
	to the caller.

	Parameters:
	[in] name : string containing name of the entity

---------------------------------------------------------------------------- */
CBaseEntity* EntityManager::FindEntity(const string& name)
{
	for (std::vector<CBaseEntity*>::iterator it = entities.begin(); it != entities.end(); it++)
	{
		CBaseEntity* entity = *it;

		if (entity->GetName() == name)
		{
			return entity;
		}
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
	entities.push_back(entity);

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
	for (std::vector<CBaseEntity*>::iterator it = entities.begin(); it != entities.end(); it++)
	{
		CBaseEntity* en = *it;

		if (en == entity)
		{
			entities.erase(it);
		}
	}
}




/* ----------------------------------------------------------------------------
	Summary:
	delete an entity from the manager

	Parameters:
	[in] name : string containing the name of the entity to remove

---------------------------------------------------------------------------- */
void EntityManager::DelEntity(const string& name)
{
	for (std::vector<CBaseEntity*>::iterator it = entities.begin(); it != entities.end(); it++)
	{
		CBaseEntity* entity = *it;

		if (entity->GetName() == name)
		{
			entities.erase(it);
		}
	}
}



/* ----------------------------------------------------------------------------
	Summary:
	update feature for the entity

	Parameters:
	[in] dTime : delta time since last frame in milliseconds


---------------------------------------------------------------------------- */
void EntityManager::Update(float dTime)
{
	for (std::vector<CBaseEntity*>::iterator it = entities.begin(); it != entities.end(); it++)
	{
		CBaseEntity* entity = *it;
		entity->Update(dTime);
	}
}


void EntityManager::Display()
{
	for (std::vector<CBaseEntity*>::iterator it = entities.begin(); it != entities.end(); it++)
	{
		CBaseEntity* entity = *it;
		entity->Display();
	}
}
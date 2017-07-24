#pragma once

#include <vector>
#include <map>
#include "..\BaseEntity.h"


// short-cut to get the entity manager class
#define EntityMgr		EntityManager::Instance()


class EntityManager
{
	typedef std::map<int, CBaseEntity*> EntityMap;

private:
	EntityMap			m_EntityMap;

public:
	EntityManager();
	~EntityManager();

	CBaseEntity* FindEntity(const string& name);
	CBaseEntity* FindEntity(int id);
	bool AddEntity(CBaseEntity* entity);
	void DelEntity(CBaseEntity* entity);
	void DelEntity(const string& name);

	void Update(float dTime);
	void Display();
	void Clear();


	static EntityManager* Instance();
};


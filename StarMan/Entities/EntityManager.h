#pragma once

#include <vector>
#include "..\BaseEntity.h"


// short-cut to get the entity manager class
#define EntityMgr		EntityManager::Instance()


class EntityManager
{
	std::vector<CBaseEntity*> entities;

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


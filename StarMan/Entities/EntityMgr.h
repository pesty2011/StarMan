#pragma once

#include <vector>
#include "..\BaseEntity.h"

class EntityMgr
{
	std::vector<CBaseEntity*> entities;

public:
	EntityMgr();
	~EntityMgr();

	CBaseEntity* FindEntity(const string& name);
	bool AddEntity(CBaseEntity* entity);
	void DelEntity(CBaseEntity* entity);
	void DelEntity(const string& name);

	void Update(float dTime);
	void Clear();
};


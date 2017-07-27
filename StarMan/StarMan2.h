#pragma once
#include "BaseEntity.h"
class CStarMan2 :
	public CBaseEntity
{
public:
	CStarMan2();
	CStarMan2(int id);
	~CStarMan2();
	virtual void Init();
	virtual void Update(float dTime);
};


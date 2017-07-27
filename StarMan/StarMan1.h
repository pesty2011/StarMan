#pragma once
#include "BaseEntity.h"
class CStarMan1 :
	public CBaseEntity
{
public:
	CStarMan1();
	CStarMan1(int id);

	~CStarMan1();
	virtual void Init();
	virtual void Update(float dTime);
};


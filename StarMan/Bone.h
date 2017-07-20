#pragma once
#include "BaseEntity.h"

class CBone :
	public CBaseEntity
{
public:
	CBone();
	~CBone();

	float		mOffset[3];
	float		mAngle[3];
	float		mQuat[3];

};


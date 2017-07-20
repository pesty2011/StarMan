#pragma once

#include "BaseEntity.h"
#include "Bone.h"
#include <vector>

class CSkeleton :
	public CBaseEntity
{
public:

	float					mOffset[3];
	float					mAngle[3];

	std::vector<CBone*>		mBones;


	CSkeleton();
	~CSkeleton();

	void Init();

	virtual CBaseEntity*		GetChild(int i);
	virtual CBaseEntity*		GetChild(string name);

	virtual void			InsertChild(CBaseEntity* child);
};


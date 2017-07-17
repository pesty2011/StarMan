#pragma once

#include "BaseEntity.h"
#include "Bone.h"

#include <d3d9.h>
#include <d3d9types.h>
#include <d3d9helper.h>
#include <vector>

class CSkeleton :
	public CBaseEntity
{
public:

	D3DVECTOR				mOffset;
	D3DVECTOR				mAngle;

	std::vector<CBone*>		mBones;


	CSkeleton();
	~CSkeleton();

	void Init();

	virtual CBaseEntity*		GetChild(int i);
	virtual CBaseEntity*		GetChild(string name);

	virtual void			InsertChild(CBaseEntity* child);
};


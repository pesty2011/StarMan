#pragma once
#include "BaseEntity.h"
#include <d3dx9math.h>

class CBone :
	public CBaseEntity
{
public:
	CBone();
	~CBone();

	D3DVECTOR		mOffset;
	D3DVECTOR		mAngle;
	D3DXQUATERNION	mQuat;

};


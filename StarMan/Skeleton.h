#pragma once

#include "AnimBase.h"
#include "Bone.h"

#include <d3d9.h>
#include <d3d9types.h>
#include <d3d9helper.h>
#include <vector>

class CSkeleton :
	public CAnimBase
{
public:

	D3DVECTOR				mOffset;
	D3DVECTOR				mAngle;

	std::vector<CBone*>		mBones;


	CSkeleton();
	~CSkeleton();
};


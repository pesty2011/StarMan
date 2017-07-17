#include "Skeleton.h"



CSkeleton::CSkeleton()
{
	Init();
}



CSkeleton::~CSkeleton()
{
	for (int i = 0; i < mBones.size(); i++)
	{
		CBone* bone = mBones[i];
		delete bone;
	}
	mBones.clear();
}



void CSkeleton::Init()
{
	mType = EN_Skeleton;
	memset(&mOffset, 0, sizeof(D3DVECTOR));
	memset(&mAngle, 0, sizeof(D3DVECTOR));
	mParent = NULL;
}



/* ----------------------------------------------------------------------------
	given an index into the bone vector, return the bone otherwise
	NULL is returned.

---------------------------------------------------------------------------- */
CBaseEntity* CSkeleton::GetChild(int i)
{
	if (mBones.size() <= i)
	{
		return mBones[i];
	}
	return NULL;
}




/* ============================================================================
	given a name of a specific bone, return the bone call back to caller,
	otherwise NULL.

============================================================================ */
CBaseEntity* CSkeleton::GetChild(string name)
{
	for (std::vector<CBone*>::iterator it = mBones.begin(); it != mBones.end(); it++)
	{
		CBone* bone = *it;

		if (bone->GetName() == name)
		{
			return bone;
		}
	}
	return NULL;
}




void CSkeleton::InsertChild(CBaseEntity* child)
{
	CBone* bone = (CBone*)child;
	if (bone->mType == EN_Bone)
		mBones.push_back(bone);
	else
	{
		// error, cannot add non-bones to skeleton
	}
}


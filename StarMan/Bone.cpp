#include "Bone.h"


CBone::CBone()
{
	mType = EN_Bone;
	memset(&mOffset, 0, sizeof(D3DVECTOR));
	memset(&mAngle, 0, sizeof(D3DVECTOR));
	mParent = NULL;
}


CBone::~CBone()
{

}

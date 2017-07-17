#include "AnimBase.h"

CAnimBase::CAnimBase()
{
	mName = "";						// no name

	mParent = NULL;					// No parent class at this point
	mType = ABT_Unknown;			// unknown type;
}


CAnimBase::~CAnimBase()
{
}


string CAnimBase::GetName()
{
	return mName;
}


int CAnimBase::NumChildren()
{
	return 0;
}


CAnimBase* CAnimBase::GetChild(int i)
{
	return NULL;
}


void CAnimBase::InsertChild(CAnimBase* child)
{

}



void CAnimBase::DeleteChild(CAnimBase* child)
{
}
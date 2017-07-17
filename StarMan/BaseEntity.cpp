#include "BaseEntity.h"

CBaseEntity::CBaseEntity()
{
	mName = "";						// no name

	mParent = NULL;					// No parent class at this point
	mType = EN_Unknown;			// unknown type;
}


CBaseEntity::~CBaseEntity()
{
}


string CBaseEntity::GetName()
{
	return mName;
}


int CBaseEntity::NumChildren()
{
	return 0;
}


CBaseEntity* CBaseEntity::GetChild(int i)
{
	return NULL;
}


void CBaseEntity::InsertChild(CBaseEntity* child)
{

}



void CBaseEntity::DeleteChild(CBaseEntity* child)
{
}
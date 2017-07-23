#include <string>
#include <iostream>

#include "BaseEntity.h"
#include "EntityNames.h"
#include "FSMSystem.h"



CBaseEntity::CBaseEntity(int id)
{
	mID = id;
	mName = GetNameOfEntity(id);

	mParent = NULL;					// No parent class at this point
	mType = EN_Unknown;				// unknown type;

	m_pAnim = new CAnimPlayer();
	m_pFSM = new FSMSystem(this);
}

CBaseEntity::CBaseEntity()
{
	mName = "Unknown";

	mParent = NULL;					// No parent class at this point
	mType = EN_Unknown;				// unknown type;
}



CBaseEntity::~CBaseEntity()
{
}


string CBaseEntity::GetName()
{
	return mName;
}



void CBaseEntity::SetName(string name)
{
	mName = name;
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



void CBaseEntity::Play(string animName)
{
	if (m_pAnim)
	{
		m_pAnim->Play(animName);
	}
}

void CBaseEntity::Update(float dTime)
{
//	cout << "Updating: " << mName << endl;
//	m_pFSM->Update(dTime);

	if (m_pAnim)
		m_pAnim->Update(dTime);
}




void CBaseEntity::Display()
{
	if (m_pAnim)
		m_pAnim->Display();
}
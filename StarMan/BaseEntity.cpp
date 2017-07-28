#include <string>
#include <iostream>

#include "BaseEntity.h"
#include "EntityNames.h"
#include "FSMSystem.h"

#include "States\Karate01State.h"
#include "KarateStates.h"

CBaseEntity::CBaseEntity(int id)
{
	mID = id;
	mName = GetNameOfEntity(id);

	mParent = NULL;					// No parent class at this point
	mType = EN_Unknown;				// unknown type;

	m_pAnim = new CAnimPlayer(this);
	m_pFSM = new FSMSystem(this);

	m_pTarget = NULL;
}

CBaseEntity::CBaseEntity()
{
	mName = "Unknown";

	mParent = NULL;					// No parent class at this point
	mType = EN_Unknown;				// unknown type;
	m_pTarget = NULL;

	m_pAnim = new CAnimPlayer(this);
	m_pFSM = new FSMSystem(this);

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


void CBaseEntity::Init()
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
	m_pFSM->Update(dTime);

	if (m_pAnim)
		m_pAnim->Update(dTime);
}




void CBaseEntity::Display()
{
	if (m_pAnim)
		m_pAnim->Display();
}



bool CBaseEntity::OnHandleMessage(const EntityMessage& msg)
{
	int type = MSG_MASK_TYPE(msg.Msg);
	int evt = MSG_MASK_EVENT(msg.Msg);




	switch (type)
	{
	case MSGTYPE_EVENT:
		break;

	case MSGTYPE_TRANSITION:
		m_pFSM->RaiseTransition(evt);
		break;

	case MSGTYPE_PLAYBACK:
		// done due to the states rely on animation events to 
		// perform transitions
		m_pFSM->RaiseTransition(evt);			
		break;
	
	case MSGTYPE_COLLISION:
		cout << "MSGTYPE_COLLISION received: " << evt << endl;
		tCollisionEvent* evt = (tCollisionEvent*)msg.Info;

		cout << "From: " << evt->From << endl;
		cout << "To: " << evt->From << endl;
		cout << "BoneName: " << evt->BoneName << endl;

		if (m_pAnim)
		{
			m_pAnim->SetBoneColour(evt->BoneName, 1.0f, 1.0f, 1.0f, 1.0f);
		}
		break;
	}

	return true;
}



bool CBaseEntity::GetBonePos(std::string name, t3Point* pt)
{
	if (m_pAnim)
	{
		return m_pAnim->GetBonePos(name, pt);
	}
	return false;
}
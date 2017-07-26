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

	Init();
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


/*
	Summary:
	Lets hard code a FSM controller for the animation system


*/
void CBaseEntity::Init()
{
	FSMState*	state;

	state = new CKarate01State(this);
	state->SetID(STATEID_KARATE_FORWARD_KICK);
	state->AddTransition(MSGEVENT_ANIMPLAYBACK_DONE, STATEID_KARATE_02);
	m_pFSM->AddState(state);

	state = new CKarate02State(this);
	state->SetID(STATEID_KARATE_02);
	state->AddTransition(MSGEVENT_ANIMPLAYBACK_DONE, STATEID_KARATE_FORWARD_KICK);
	m_pFSM->AddState(state);


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
		cout << "MSGTYPE_EVENT received: " << evt << endl;
		break;

	case MSGTYPE_TRANSITION:
		cout << "MSGTYPE_TRANSITION received: " << evt << endl;
		break;

	case MSGTYPE_PLAYBACK:
		cout << "MSGTYPE_PLAYBACK received: " << evt << endl;
		m_pFSM->RaiseTransition(evt);
		break;
	}

	return true;
}
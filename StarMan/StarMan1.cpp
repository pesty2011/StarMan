#include "KarateStates.h"
#include "StarMan1.h"
#include "FSMSystem.h"


CStarMan1::CStarMan1()
	: CBaseEntity()
{
	Init();
}

CStarMan1::CStarMan1(int id)
	: CBaseEntity(id)
{
	Init();
}


CStarMan1::~CStarMan1()
{
}


void CStarMan1::Init()
{
	FSMState*	state;

	mPosition = t3Point(2.0f, 0.0f, 0.0f);
	mColour = t3Point(0.0f, 1.0f, 0.5f);
	mDir = t3Point(0.0f, -90.0f, 0.0f);

	state = new CKarate01State(this);
	state->SetID(STATEID_KARATE_FORWARD_KICK);
	state->AddTransition(MSGEVENT_ANIMPLAYBACK_DONE, STATEID_KARATE_02);
	m_pFSM->AddState(state);

	state = new CKarate02State(this);
	state->SetID(STATEID_KARATE_02);
	state->AddTransition(MSGEVENT_ANIMPLAYBACK_DONE, STATEID_KARATE_FORWARD_KICK);
	m_pFSM->AddState(state);

}


void CStarMan1::Update(float dTime)
{
	CBaseEntity::Update(dTime);
}

#include "StarMan2.h"
#include "KarateStates.h"
#include "FSMSystem.h"

CStarMan2::CStarMan2()
	: CBaseEntity()
{
	Init();
}

CStarMan2::CStarMan2(int id)
	: CBaseEntity(id)
{
	Init();
}

CStarMan2::~CStarMan2()
{
}


void CStarMan2::Init()
{
	FSMState*	state;

	mPosition = t3Point(-5.0f, 0.0f, 0.0f);
	mColour = t3Point(1.0f, 0.5f, 0.0f);
	mDir = t3Point(0.0f, 90.0f, 0.0f);

	state = new CKarate04State(this);
	state->SetID(STATEID_KARATE_04);
	state->AddTransition(MSGEVENT_ANIMPLAYBACK_DONE, STATEID_KARATE_05);
	m_pFSM->AddState(state);

	state = new CKarate05State(this);
	state->SetID(STATEID_KARATE_05);
	state->AddTransition(MSGEVENT_ANIMPLAYBACK_DONE, STATEID_KARATE_04);
	m_pFSM->AddState(state);

}


void CStarMan2::Update(float dTime)
{
	// do some logic to turn and face target ...





	CBaseEntity::Update(dTime);
}


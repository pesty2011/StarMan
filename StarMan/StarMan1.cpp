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

	mPosition = glm::vec3(1.5f, 0.0f, 0.0f);
	mColour = glm::vec3(0.0f, 1.0f, 0.5f);
	mDir = glm::vec3(0.0f, -90.0f, 0.0f);

	// some kicks
	state = new CKarate01State(this);
	state->SetID(STATEID_KARATE_FORWARD_KICK);
	state->AddTransition(MSGEVENT_ANIMPLAYBACK_DONE, STATEID_KARATE_02);
	state->AddTransition(MSGEVENT_PUNCH_SEQUENCES, STATEID_KARATE_11);
	state->AddTransition(MSGEVENT_GUARD_SEQUENCES, STATEID_KARATE_17);
	m_pFSM->AddState(state);

	state = new CKarate02State(this);
	state->SetID(STATEID_KARATE_02);
	state->AddTransition(MSGEVENT_ANIMPLAYBACK_DONE, STATEID_KARATE_FORWARD_KICK);
	state->AddTransition(MSGEVENT_PUNCH_SEQUENCES, STATEID_KARATE_11);
	state->AddTransition(MSGEVENT_GUARD_SEQUENCES, STATEID_KARATE_17);
	m_pFSM->AddState(state);


	// some punches
	state = new CKarate11State(this);
	state->SetID(STATEID_KARATE_11);
	state->AddTransition(MSGEVENT_ANIMPLAYBACK_DONE, STATEID_KARATE_12);
	state->AddTransition(MSGEVENT_KICK_SEQUENCES, STATEID_KARATE_02);
	state->AddTransition(MSGEVENT_GUARD_SEQUENCES, STATEID_KARATE_17);
	m_pFSM->AddState(state);

	state = new CKarate12State(this);
	state->SetID(STATEID_KARATE_12);
	state->AddTransition(MSGEVENT_ANIMPLAYBACK_DONE, STATEID_KARATE_13);
	state->AddTransition(MSGEVENT_KICK_SEQUENCES, STATEID_KARATE_02);
	state->AddTransition(MSGEVENT_GUARD_SEQUENCES, STATEID_KARATE_17);
	m_pFSM->AddState(state);

	state = new CKarate13State(this);
	state->SetID(STATEID_KARATE_13);
	state->AddTransition(MSGEVENT_ANIMPLAYBACK_DONE, STATEID_KARATE_14);
	state->AddTransition(MSGEVENT_KICK_SEQUENCES, STATEID_KARATE_02);
	state->AddTransition(MSGEVENT_GUARD_SEQUENCES, STATEID_KARATE_17);
	m_pFSM->AddState(state);

	state = new CKarate14State(this);
	state->SetID(STATEID_KARATE_14);
	state->AddTransition(MSGEVENT_ANIMPLAYBACK_DONE, STATEID_KARATE_12);
	state->AddTransition(MSGEVENT_KICK_SEQUENCES, STATEID_KARATE_02);
	state->AddTransition(MSGEVENT_GUARD_SEQUENCES, STATEID_KARATE_17);
	m_pFSM->AddState(state);


	// some guards
	state = new CKarate17State(this);
	state->SetID(STATEID_KARATE_17);
	state->AddTransition(MSGEVENT_ANIMPLAYBACK_DONE, STATEID_KARATE_18);
	state->AddTransition(MSGEVENT_PUNCH_SEQUENCES, STATEID_KARATE_11);
	state->AddTransition(MSGEVENT_KICK_SEQUENCES, STATEID_KARATE_02);
	m_pFSM->AddState(state);

	state = new CKarate18State(this);
	state->SetID(STATEID_KARATE_18);
	state->AddTransition(MSGEVENT_ANIMPLAYBACK_DONE, STATEID_KARATE_19);
	state->AddTransition(MSGEVENT_PUNCH_SEQUENCES, STATEID_KARATE_11);
	state->AddTransition(MSGEVENT_KICK_SEQUENCES, STATEID_KARATE_02);
	m_pFSM->AddState(state);

	state = new CKarate19State(this);
	state->SetID(STATEID_KARATE_19);
	state->AddTransition(MSGEVENT_ANIMPLAYBACK_DONE, STATEID_KARATE_17);
	state->AddTransition(MSGEVENT_PUNCH_SEQUENCES, STATEID_KARATE_11);
	state->AddTransition(MSGEVENT_KICK_SEQUENCES, STATEID_KARATE_02);
	m_pFSM->AddState(state);


}


void CStarMan1::Update(float dTime)
{
	CBaseEntity::Update(dTime);
}

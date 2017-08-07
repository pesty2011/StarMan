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

	mPosition = glm::vec3(-1.5f, 0.0f, 0.0f);
	mColour = glm::vec3(1.0f, 0.5f, 0.0f);
	mDir = glm::vec3(0.0f, 90.0f, 0.0f);


	// some kicks 04-05
	state = new CKarate04State(this);
	state->SetID(STATEID_KARATE_04);
	state->AddTransition(MSGEVENT_ANIMPLAYBACK_DONE, STATEID_KARATE_05);
	state->AddTransition(MSGEVENT_PUNCH_SEQUENCES, STATEID_KARATE_08);
	state->AddTransition(MSGEVENT_GUARD_SEQUENCES, STATEID_KARATE_16);
	m_pFSM->AddState(state);

	state = new CKarate05State(this);
	state->SetID(STATEID_KARATE_05);
	state->AddTransition(MSGEVENT_ANIMPLAYBACK_DONE, STATEID_KARATE_04);
	state->AddTransition(MSGEVENT_PUNCH_SEQUENCES, STATEID_KARATE_08);
	state->AddTransition(MSGEVENT_GUARD_SEQUENCES, STATEID_KARATE_16);
	m_pFSM->AddState(state);


	// some punches 08-10
	state = new CKarate08State(this);
	state->SetID(STATEID_KARATE_08);
	state->AddTransition(MSGEVENT_ANIMPLAYBACK_DONE, STATEID_KARATE_09);
	state->AddTransition(MSGEVENT_GUARD_SEQUENCES, STATEID_KARATE_16);
	state->AddTransition(MSGEVENT_KICK_SEQUENCES, STATEID_KARATE_05);
	m_pFSM->AddState(state);

	state = new CKarate09State(this);
	state->SetID(STATEID_KARATE_09);
	state->AddTransition(MSGEVENT_ANIMPLAYBACK_DONE, STATEID_KARATE_10);
	state->AddTransition(MSGEVENT_GUARD_SEQUENCES, STATEID_KARATE_16);
	state->AddTransition(MSGEVENT_KICK_SEQUENCES, STATEID_KARATE_05);
	m_pFSM->AddState(state);


	state = new CKarate10State(this);
	state->SetID(STATEID_KARATE_10);
	state->AddTransition(MSGEVENT_ANIMPLAYBACK_DONE, STATEID_KARATE_08);
	state->AddTransition(MSGEVENT_GUARD_SEQUENCES, STATEID_KARATE_16);
	state->AddTransition(MSGEVENT_KICK_SEQUENCES, STATEID_KARATE_05);
	m_pFSM->AddState(state);


	// some guards 15-16
	state = new CKarate15State(this);
	state->SetID(STATEID_KARATE_15);
	state->AddTransition(MSGEVENT_ANIMPLAYBACK_DONE, STATEID_KARATE_16);
	state->AddTransition(MSGEVENT_PUNCH_SEQUENCES, STATEID_KARATE_08);
	state->AddTransition(MSGEVENT_KICK_SEQUENCES, STATEID_KARATE_05);
	m_pFSM->AddState(state);

	state = new CKarate16State(this);
	state->SetID(STATEID_KARATE_16);
	state->AddTransition(MSGEVENT_ANIMPLAYBACK_DONE, STATEID_KARATE_15);
	state->AddTransition(MSGEVENT_PUNCH_SEQUENCES, STATEID_KARATE_08);
	state->AddTransition(MSGEVENT_KICK_SEQUENCES, STATEID_KARATE_05);
	m_pFSM->AddState(state);


}


void CStarMan2::Update(float dTime)
{
	// do some logic to turn and face target ...





	CBaseEntity::Update(dTime);
}


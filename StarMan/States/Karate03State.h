#pragma once
#include "..\FSMState.h"
#include "..\KarateStates.h"

class CKarate03State :
	public FSMState
{
public:
	CKarate03State();
	CKarate03State(CBaseEntity* owner);
	~CKarate03State();


	virtual void Enter();
	virtual void Execute(float dTime);
	virtual void Exit();
};


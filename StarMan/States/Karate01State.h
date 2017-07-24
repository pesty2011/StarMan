#pragma once
#include "..\FSMState.h"
#include "..\KarateStates.h"

class CKarate01State :
	public FSMState
{
public:
	CKarate01State();
	CKarate01State(CBaseEntity* owner);
	~CKarate01State();

	virtual void Enter();
	virtual void Execute(float dTime);
	virtual void Exit();
};


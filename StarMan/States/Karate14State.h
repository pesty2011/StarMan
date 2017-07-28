#pragma once
#include "..\FSMState.h"
#include "..\KarateStates.h"

class CKarate14State :
	public FSMState
{
public:
	CKarate14State();
	CKarate14State(CBaseEntity* owner);
	~CKarate14State();


	virtual void Enter();
	virtual void Execute(float dTime);
	virtual void Exit();
};


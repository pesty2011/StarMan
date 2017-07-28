#pragma once
#include "..\FSMState.h"
#include "..\KarateStates.h"

class CKarate15State :
	public FSMState
{
public:
	CKarate15State();
	CKarate15State(CBaseEntity* owner);
	~CKarate15State();


	virtual void Enter();
	virtual void Execute(float dTime);
	virtual void Exit();
};


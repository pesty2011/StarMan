#pragma once
#include "..\FSMState.h"
#include "..\KarateStates.h"

class CKarate18State :
	public FSMState
{
public:
	CKarate18State();
	CKarate18State(CBaseEntity* owner);
	~CKarate18State();


	virtual void Enter();
	virtual void Execute(float dTime);
	virtual void Exit();
};


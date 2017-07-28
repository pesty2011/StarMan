#pragma once
#include "..\FSMState.h"
#include "..\KarateStates.h"

class CKarate16State :
	public FSMState
{
public:
	CKarate16State();
	CKarate16State(CBaseEntity* owner);
	~CKarate16State();


	virtual void Enter();
	virtual void Execute(float dTime);
	virtual void Exit();
};


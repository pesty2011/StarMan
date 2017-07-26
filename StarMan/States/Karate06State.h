#pragma once
#include "..\FSMState.h"
#include "..\KarateStates.h"

class CKarate06State :
	public FSMState
{
public:
	CKarate06State();
	CKarate06State(CBaseEntity* owner);
	~CKarate06State();


	virtual void Enter();
	virtual void Execute(float dTime);
	virtual void Exit();
};


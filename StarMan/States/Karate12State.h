#pragma once
#include "..\FSMState.h"
#include "..\KarateStates.h"

class CKarate12State :
	public FSMState
{
public:
	CKarate12State();
	CKarate12State(CBaseEntity* owner);
	~CKarate12State();


	virtual void Enter();
	virtual void Execute(float dTime);
	virtual void Exit();
};


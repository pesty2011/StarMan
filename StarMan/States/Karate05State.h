#pragma once
#include "..\FSMState.h"
#include "..\KarateStates.h"

class CKarate05State :
	public FSMState
{
public:
	CKarate05State();
	CKarate05State(CBaseEntity* owner);
	~CKarate05State();


	virtual void Enter();
	virtual void Execute(float dTime);
	virtual void Exit();
};


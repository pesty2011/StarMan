#pragma once
#include "..\FSMState.h"
#include "..\KarateStates.h"

class CKarate19State :
	public FSMState
{
public:
	CKarate19State();
	CKarate19State(CBaseEntity* owner);
	~CKarate19State();


	virtual void Enter();
	virtual void Execute(float dTime);
	virtual void Exit();
};


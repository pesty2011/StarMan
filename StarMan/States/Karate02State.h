#pragma once
#include "..\FSMState.h"
#include "..\KarateStates.h"

class CKarate02State :
	public FSMState
{
public:
	CKarate02State();
	CKarate02State(CBaseEntity* owner);
	~CKarate02State();


	virtual void Enter();
	virtual void Execute(float dTime);
	virtual void Exit();
};


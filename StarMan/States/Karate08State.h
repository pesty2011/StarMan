#pragma once
#include "..\FSMState.h"
#include "..\KarateStates.h"

class CKarate08State :
	public FSMState
{
public:
	CKarate08State();
	CKarate08State(CBaseEntity* owner);
	~CKarate08State();


	virtual void Enter();
	virtual void Execute(float dTime);
	virtual void Exit();
};


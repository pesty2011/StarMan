#pragma once
#include "..\FSMState.h"
#include "..\KarateStates.h"

class CKarate04State :
	public FSMState
{
public:
	CKarate04State();
	CKarate04State(CBaseEntity* owner);
	~CKarate04State();


	virtual void Enter();
	virtual void Execute(float dTime);
	virtual void Exit();
};


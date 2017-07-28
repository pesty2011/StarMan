#pragma once
#include "..\FSMState.h"
#include "..\KarateStates.h"

class CKarate11State :
	public FSMState
{
public:
	CKarate11State();
	CKarate11State(CBaseEntity* owner);
	~CKarate11State();


	virtual void Enter();
	virtual void Execute(float dTime);
	virtual void Exit();
};


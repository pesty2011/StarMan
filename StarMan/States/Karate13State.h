#pragma once
#include "..\FSMState.h"
#include "..\KarateStates.h"

class CKarate13State :
	public FSMState
{
public:
	CKarate13State();
	CKarate13State(CBaseEntity* owner);
	~CKarate13State();


	virtual void Enter();
	virtual void Execute(float dTime);
	virtual void Exit();
};


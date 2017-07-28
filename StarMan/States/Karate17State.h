#pragma once
#include "..\FSMState.h"
#include "..\KarateStates.h"

class CKarate17State :
	public FSMState
{
public:
	CKarate17State();
	CKarate17State(CBaseEntity* owner);
	~CKarate17State();


	virtual void Enter();
	virtual void Execute(float dTime);
	virtual void Exit();
};


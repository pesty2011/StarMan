#pragma once
#include "..\FSMState.h"
#include "..\KarateStates.h"

class CKarate09State :
	public FSMState
{
public:
	CKarate09State();
	CKarate09State(CBaseEntity* owner);
	~CKarate09State();


	virtual void Enter();
	virtual void Execute(float dTime);
	virtual void Exit();
};


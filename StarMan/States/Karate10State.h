#pragma once
#include "..\FSMState.h"
#include "..\KarateStates.h"

class CKarate10State :
	public FSMState
{
public:
	CKarate10State();
	CKarate10State(CBaseEntity* owner);
	~CKarate10State();


	virtual void Enter();
	virtual void Execute(float dTime);
	virtual void Exit();
};


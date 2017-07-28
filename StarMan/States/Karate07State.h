#pragma once
#include "..\FSMState.h"
#include "..\KarateStates.h"

class CKarate07State :
	public FSMState
{
public:
	CKarate07State();
	CKarate07State(CBaseEntity* owner);
	~CKarate07State();


	virtual void Enter();
	virtual void Execute(float dTime);
	virtual void Exit();
};


#pragma once
#include <map>
#include <windows.h>

#include "FSMState.h"
#include "BaseEntity.h"

class FSMSystem
{
protected:
	std::map<int, FSMState*> states;
	int mCurrentStateID;
	FSMState* mCurrentState;

	CBaseEntity* Owner;
public:
	FSMSystem();
	FSMSystem(CBaseEntity* owner);
	~FSMSystem();


	int GetCurrentStateID() { return mCurrentStateID; }
	FSMState* GetCurrentState() { return mCurrentState; }


	bool AddState(FSMState* state);
	void DelState(FSMState* state);
	void DelState(int stateID);
	void ChangeState(int stateID);
	void RaiseTransition(int transition);

	void Update(float dTime);

};


#pragma once
#include <vector>
#include <map>
#include "BaseEntity.h"


/*
	Summary:
	State Design Pattern

	Each of a game agent's states is implemented as a unique class and
	each agent holds a pointer to an instance of its current state.  An
	agent also implements a ChangeState member function that can be
	called to facilitate the switching of states whenever a state transition
	is required.  The logic for determining any state transitions is
	contained within each State class.  All state classes are derived
	from an abstract base class, thereby defining a common interface.

	It is favourable for each states to have associated enter and exit
	actions.  This permits the programmer to write logic that is only
	executed once at state entry and exit and increases the flexibility
	of an FSM a great deal.
*/
class FSMState
{
protected:
	std::map<int, int> transitions;

	int mStateID;
	bool mLocked;
	CBaseEntity* mOwner;

public:
	FSMState();
	FSMState(CBaseEntity* owner);
	~FSMState();

	int GetID() { return mStateID; }

	void AddTransition(int trans, int stateID);
	void DelTransition(int trans, int stateID);
	int GetTransitionStateID(int transition);

	virtual void Enter();
	virtual void Execute(float dTime);
	virtual void Exit();


};


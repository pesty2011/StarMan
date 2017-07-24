#include "FSMSystem.h"
#include "KarateStates.h"


FSMSystem::FSMSystem()
{
}

FSMSystem::FSMSystem(CBaseEntity* owner)
{
	Owner = owner;
}


FSMSystem::~FSMSystem()
{
}



/* ----------------------------------------------------------------------------
	Summary:
	Add a state into the FSM system.  if this is the first state, it will
	be setup as the default state which will start executing.

	Parameters:
	[in] state : pointer to a FSMState class to add into the state machine

---------------------------------------------------------------------------- */
bool FSMSystem::AddState(FSMState* state)
{
	if (state != NULL)
	{
		bool defaultState = states.empty();
		states.insert(std::make_pair(state->GetID(), state));

		if (defaultState == true)
		{
			mCurrentState = state;
			mCurrentStateID = state->GetID();
			mCurrentState->Enter();
		}
	}

	return true;
}



void FSMSystem::DelState(FSMState* state)
{

}



void FSMSystem::DelState(int stateID)
{

}


void FSMSystem::ChangeState(int stateID)
{
	if (stateID != STATEID_UNKNOWN && mCurrentState && mCurrentState->GetID() != stateID)
	{
		FSMState* state = states.at(stateID);
		if (state != NULL)
		{
			mCurrentState->Exit();
			mCurrentState = state;
			mCurrentState->Enter();
		}
		else
		{
			cout << "[FSM ERROR] stateID not found for change: " << stateID << endl;
		}
	}
}



/* ----------------------------------------------------------------------------
	Summary:
	Raise transition event to trigger a change in the state machine

	Parameters:
	[in] transition : the transition value to change the state

---------------------------------------------------------------------------- */
void FSMSystem::RaiseTransition(int transition)
{
	if (transition == MSGEVENT_UNKNOWN)
	{
		cout << "[FSM ERROR] Unknown transition" << endl;
		return;
	}


	if (mCurrentState == NULL)
	{
		cout << "[FSM ERROR] transition cannot act on a NULL state" << endl;
		return;
	}

	int id = mCurrentState->GetTransitionStateID(transition);
	ChangeState(id);
}




void FSMSystem::Update(float dTime)
{
	if (mCurrentState != NULL)
	{
		mCurrentState->Execute(dTime);
	}
}

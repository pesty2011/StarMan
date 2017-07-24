#include "FSMState.h"
#include <iostream>


FSMState::FSMState()
{
	mStateID = 0;
	mLocked = false;
	mOwner = NULL;
}


FSMState::FSMState(CBaseEntity* owner)
{
	mStateID = 0;
	mLocked = false;
	mOwner = owner;
}


FSMState::~FSMState()
{
	transitions.clear();
}



void FSMState::AddTransition(int trans, int stateID)
{
	// create a pair that we can use to check if inserting a new object is possible
	std::pair<std::map<int, int>::iterator, bool> ret;
	
	ret = transitions.insert(std::pair<int, int>(trans, stateID));
	if (ret.second == false)
	{
		cout << "Transistion: " << trans << " already exists" << endl;
		cout << " with a value of: " << ret.first->second << endl;
	}
}



void FSMState::DelTransition(int trans, int stateID)
{
	transitions.erase(trans);
}



int FSMState::GetTransitionStateID(int transition)
{
	int stateID = transitions.at(transition);
	return stateID;
}



void FSMState::Enter()
{
	cout << "FSMState: Enter" << endl;
}



void FSMState::Execute(float dTime)
{
//	cout << "FSMState: Execute" << endl;
}



void FSMState::Exit()
{
	cout << "FSMState: Exit" << endl;

}

#include "FSMSystem.h"



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




bool FSMSystem::AddState(FSMState* state)
{
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

}


void FSMSystem::RaiseTransition(int transition)
{

}



void FSMSystem::Update(float dTime)
{
	if (mCurrentState != NULL)
	{

	}
}

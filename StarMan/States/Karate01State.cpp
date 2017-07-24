#include "Karate01State.h"

#include "..\BaseEntity.h"

CKarate01State::CKarate01State()
{
}

CKarate01State::CKarate01State(CBaseEntity* owner) 
	: FSMState(owner)
{
	cout << "Create new CKarate01State" << endl;
}


CKarate01State::~CKarate01State()
{

}


void CKarate01State::Enter()
{
	cout << "CKarate01State: Enter" << endl;
}



void CKarate01State::Execute(float dTime) 
{
	cout << "CKarate01State: Execute" << endl;
	FSMState::Execute(dTime);
}



void CKarate01State::Exit()
{
	cout << "CKarate01State: Exit" << endl;

}





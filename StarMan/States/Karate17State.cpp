#include "Karate17State.h"


CKarate17State::CKarate17State()
	: FSMState()
{
	cout << "Create new CKarate17State" << endl;
}


CKarate17State::CKarate17State(CBaseEntity* owner)
	: FSMState(owner)
{
}



CKarate17State::~CKarate17State()
{
}


void CKarate17State::Enter()
{
	mOwner->Play("karate-17");
}



void CKarate17State::Execute(float dTime)
{
}



void CKarate17State::Exit()
{
}







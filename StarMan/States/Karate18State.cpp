#include "Karate18State.h"


CKarate18State::CKarate18State()
	: FSMState()
{
	cout << "Create new CKarate18State" << endl;
}


CKarate18State::CKarate18State(CBaseEntity* owner)
	: FSMState(owner)
{
}



CKarate18State::~CKarate18State()
{
}


void CKarate18State::Enter()
{
	mOwner->Play("karate-18");

}



void CKarate18State::Execute(float dTime)
{
}



void CKarate18State::Exit()
{
}







#include "Karate14State.h"


CKarate14State::CKarate14State()
	: FSMState()
{
	cout << "Create new CKarate14State" << endl;
}


CKarate14State::CKarate14State(CBaseEntity* owner)
	: FSMState(owner)
{
}



CKarate14State::~CKarate14State()
{
}


void CKarate14State::Enter()
{
	mOwner->Play("karate-14");

}



void CKarate14State::Execute(float dTime)
{
}



void CKarate14State::Exit()
{
}







#include "Karate05State.h"


CKarate05State::CKarate05State()
	: FSMState()
{
	cout << "Create new CKarate02State" << endl;
}


CKarate05State::CKarate05State(CBaseEntity* owner)
	: FSMState(owner)
{
}



CKarate05State::~CKarate05State()
{
}


void CKarate05State::Enter()
{
	mOwner->Play("karate-05");

}



void CKarate05State::Execute(float dTime)
{
}



void CKarate05State::Exit()
{
}







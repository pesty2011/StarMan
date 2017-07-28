#include "Karate19State.h"


CKarate19State::CKarate19State()
	: FSMState()
{
	cout << "Create new CKarate19State" << endl;
}


CKarate19State::CKarate19State(CBaseEntity* owner)
	: FSMState(owner)
{
}



CKarate19State::~CKarate19State()
{
}


void CKarate19State::Enter()
{
	mOwner->Play("karate-19");

}



void CKarate19State::Execute(float dTime)
{
}



void CKarate19State::Exit()
{
}







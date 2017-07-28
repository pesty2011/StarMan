#include "Karate11State.h"


CKarate11State::CKarate11State()
	: FSMState()
{
	cout << "Create new CKarate11State" << endl;
}


CKarate11State::CKarate11State(CBaseEntity* owner)
	: FSMState(owner)
{
}



CKarate11State::~CKarate11State()
{
}


void CKarate11State::Enter()
{
	mOwner->Play("karate-11");

}



void CKarate11State::Execute(float dTime)
{
}



void CKarate11State::Exit()
{
}







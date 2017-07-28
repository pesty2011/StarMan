#include "Karate07State.h"


CKarate07State::CKarate07State()
	: FSMState()
{
	cout << "Create new CKarate07State" << endl;
}


CKarate07State::CKarate07State(CBaseEntity* owner)
	: FSMState(owner)
{
}



CKarate07State::~CKarate07State()
{
}


void CKarate07State::Enter()
{
	mOwner->Play("karate-07");

}



void CKarate07State::Execute(float dTime)
{
}



void CKarate07State::Exit()
{
}







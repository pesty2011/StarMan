#include "Karate12State.h"


CKarate12State::CKarate12State()
	: FSMState()
{
	cout << "Create new CKarate12State" << endl;
}


CKarate12State::CKarate12State(CBaseEntity* owner)
	: FSMState(owner)
{
}



CKarate12State::~CKarate12State()
{
}


void CKarate12State::Enter()
{
	mOwner->Play("karate-12");

}



void CKarate12State::Execute(float dTime)
{
}



void CKarate12State::Exit()
{
}







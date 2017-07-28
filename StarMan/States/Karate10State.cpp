#include "Karate10State.h"


CKarate10State::CKarate10State()
	: FSMState()
{
	cout << "Create new CKarate10State" << endl;
}


CKarate10State::CKarate10State(CBaseEntity* owner)
	: FSMState(owner)
{
}



CKarate10State::~CKarate10State()
{
}


void CKarate10State::Enter()
{
	mOwner->Play("karate-10");

}



void CKarate10State::Execute(float dTime)
{
}



void CKarate10State::Exit()
{
}







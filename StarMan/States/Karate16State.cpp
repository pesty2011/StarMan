#include "Karate16State.h"


CKarate16State::CKarate16State()
	: FSMState()
{
	cout << "Create new CKarate16State" << endl;
}


CKarate16State::CKarate16State(CBaseEntity* owner)
	: FSMState(owner)
{
}



CKarate16State::~CKarate16State()
{
}


void CKarate16State::Enter()
{
	mOwner->Play("karate-16");

}



void CKarate16State::Execute(float dTime)
{
}



void CKarate16State::Exit()
{
}







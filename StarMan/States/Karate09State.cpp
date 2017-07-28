#include "Karate02State.h"


CKarate09State::CKarate09State()
	: FSMState()
{
	cout << "Create new CKarate09State" << endl;
}


CKarate09State::CKarate09State(CBaseEntity* owner)
	: FSMState(owner)
{
}



CKarate09State::~CKarate09State()
{
}


void CKarate09State::Enter()
{
	mOwner->Play("karate-09");

}



void CKarate09State::Execute(float dTime)
{
}



void CKarate09State::Exit()
{
}







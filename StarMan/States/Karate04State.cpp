#include "Karate04State.h"


CKarate04State::CKarate04State()
	: FSMState()
{
	cout << "Create new CKarate04State" << endl;
}


CKarate04State::CKarate04State(CBaseEntity* owner)
	: FSMState(owner)
{
}



CKarate04State::~CKarate04State()
{
}


void CKarate04State::Enter()
{
	mOwner->Play("karate-04");

}



void CKarate04State::Execute(float dTime)
{
}



void CKarate04State::Exit()
{
}







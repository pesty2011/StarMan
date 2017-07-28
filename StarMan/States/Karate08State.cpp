#include "Karate08State.h"


CKarate08State::CKarate08State()
	: FSMState()
{
	cout << "Create new CKarate02State" << endl;
}


CKarate08State::CKarate08State(CBaseEntity* owner)
	: FSMState(owner)
{
}



CKarate08State::~CKarate08State()
{
}


void CKarate08State::Enter()
{
	mOwner->Play("karate-08");

}



void CKarate08State::Execute(float dTime)
{
}



void CKarate08State::Exit()
{
}







#include "Karate03State.h"


CKarate03State::CKarate03State()
	: FSMState()
{
	cout << "Create new CKarate03State" << endl;
}


CKarate03State::CKarate03State(CBaseEntity* owner)
	: FSMState(owner)
{
}



CKarate03State::~CKarate03State()
{
}


void CKarate03State::Enter()
{
	mOwner->Play("karate-03");

}



void CKarate03State::Execute(float dTime)
{
}



void CKarate03State::Exit()
{
}







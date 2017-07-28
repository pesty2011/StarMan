#include "Karate13State.h"


CKarate13State::CKarate13State()
	: FSMState()
{
	cout << "Create new CKarate13State" << endl;
}


CKarate13State::CKarate13State(CBaseEntity* owner)
	: FSMState(owner)
{
}



CKarate13State::~CKarate13State()
{
}


void CKarate13State::Enter()
{
	mOwner->Play("karate-13");

}



void CKarate13State::Execute(float dTime)
{
}



void CKarate13State::Exit()
{
}







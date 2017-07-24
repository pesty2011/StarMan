#include "Karate02State.h"


CKarate02State::CKarate02State()
	: FSMState()
{
	cout << "Create new CKarate02State" << endl;
}


CKarate02State::CKarate02State(CBaseEntity* owner)
	: FSMState(owner)
{
}



CKarate02State::~CKarate02State()
{
}


void CKarate02State::Enter()
{
	mOwner->Play("karate-02");

}



void CKarate02State::Execute(float dTime)
{
}



void CKarate02State::Exit()
{
}







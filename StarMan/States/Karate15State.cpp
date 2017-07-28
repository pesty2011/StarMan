#include "Karate15State.h"


CKarate15State::CKarate15State()
	: FSMState()
{
	cout << "Create new CKarate15State" << endl;
}


CKarate15State::CKarate15State(CBaseEntity* owner)
	: FSMState(owner)
{
}



CKarate15State::~CKarate15State()
{
}


void CKarate15State::Enter()
{
	mOwner->Play("karate-15");

}



void CKarate15State::Execute(float dTime)
{
}



void CKarate15State::Exit()
{
}







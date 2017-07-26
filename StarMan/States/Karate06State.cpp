#include "Karate02State.h"


CKarate06State::CKarate06State()
	: FSMState()
{
	cout << "Create new CKarate02State" << endl;
}


CKarate06State::CKarate06State(CBaseEntity* owner)
	: FSMState(owner)
{
}



CKarate06State::~CKarate06State()
{
}


void CKarate06State::Enter()
{
	mOwner->Play("karate-06");

}



void CKarate06State::Execute(float dTime)
{
}



void CKarate06State::Exit()
{
}







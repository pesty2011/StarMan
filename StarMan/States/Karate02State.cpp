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
	cout << "CKarate02State: Enter" << endl;
}



void CKarate02State::Execute(float dTime)
{
	cout << "CKarate02State: Execute" << endl;

}



void CKarate02State::Exit()
{
	cout << "CKarate02State: Exit" << endl;

}







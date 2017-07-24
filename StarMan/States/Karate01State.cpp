#include "Karate01State.h"

#include "..\BaseEntity.h"
#include "..\MessageDispatch.h"

CKarate01State::CKarate01State()
{
}

CKarate01State::CKarate01State(CBaseEntity* owner) 
	: FSMState(owner)
{
	cout << "Create new CKarate01State" << endl;
}


CKarate01State::~CKarate01State()
{

}


void CKarate01State::Enter()
{
	mOwner->Play("karate-01");
}



void CKarate01State::Execute(float dTime) 
{
}



void CKarate01State::Exit()
{
}





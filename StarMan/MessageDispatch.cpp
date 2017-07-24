#include <iostream>

#include "Entities\EntityManager.h"
#include "BaseEntity.h"
#include "MessageDispatch.h"
#include "Helpers\GameTimer.h"


extern GameTimer	g_timer;			// global timer for the application

//------------------------------ Instance -------------------------------------

CMessageDispatch* CMessageDispatch::Instance()
{
	static CMessageDispatch instance;

	return &instance;
}



CMessageDispatch::CMessageDispatch()
{
}


CMessageDispatch::~CMessageDispatch()
{
}



void CMessageDispatch::Discharge(CBaseEntity* pReceiver,	const EntityMessage& msg)
{
	if (pReceiver->OnHandleMessage(msg) == false)
	{
		//telegram could not be handled
		std::cout << "Message not handled";
	}
}




//---------------------------- DispatchMessage ---------------------------
//
//  given a message, a receiver, a sender and any time delay , this function
//  routes the message to the correct agent (if no delay) or stores
//  in the message queue to be dispatched at the correct time
//------------------------------------------------------------------------
void CMessageDispatch::DispatchEntityMessage(float  delay,
	int    sender,
	int    receiver,
	int    msg,
	int		data,
	void*  ExtraInfo)
{
	//get pointers to the sender and receiver
	CBaseEntity* pSender = EntityMgr->FindEntity(sender);
	CBaseEntity* pReceiver = EntityMgr->FindEntity(receiver);

	//make sure the receiver is valid
	if (pReceiver == NULL)
	{
		cout << "\nWarning! No Receiver with ID of " << receiver << " found";

		return;
	}

	//create the telegram
	EntityMessage telegram(0, sender, receiver, msg, data, ExtraInfo);
	
	
	float CurrentTime = g_timer.CurrentTime();

	//if there is no delay, route telegram immediately                       
	if (delay <= 0.0f)
	{
		cout << "\nInstant telegram dispatched at time: " << CurrentTime
			<< " by " << pSender->GetName() << " for " << pReceiver->GetName()
			<< ". Msg is " << msg;

		//send the telegram to the recipient
		Discharge(pReceiver, telegram);
	}

	//else calculate the time when the telegram should be dispatched
	else
	{
		telegram.DispatchTime = CurrentTime + delay;

		//and put it in the queue
		PriorityQ.insert(telegram);

		cout << "\nDelayed message from: " << pSender->GetName() << " recorded at time "
			<< CurrentTime << " for " << pReceiver->GetName()
			<< ". Msg is " << msg << endl;
	}
}




//---------------------- DispatchDelayedMessages -------------------------
//
//  This function dispatches any telegrams with a timestamp that has
//  expired. Any dispatched telegrams are removed from the queue
//------------------------------------------------------------------------
void CMessageDispatch::DispatchDelayedMessages()
{
	//get current time
	float CurrentTime = g_timer.CurrentTime();

	//now peek at the queue to see if any telegrams need dispatching.
	//remove all telegrams from the front of the queue that have gone
	//past their sell by date
	while (!PriorityQ.empty() &&
		(PriorityQ.begin()->DispatchTime < CurrentTime) &&
		(PriorityQ.begin()->DispatchTime > 0))
	{
		//read the telegram from the front of the queue
		const EntityMessage& telegram = *PriorityQ.begin();

		//find the recipient
		CBaseEntity* pReceiver = EntityMgr->FindEntity(telegram.Receiver);

		cout << "\nQueued telegram ready for dispatch: Sent to "
			<< pReceiver->GetName() << ". Msg is " << telegram.Msg;

		//send the telegram to the recipient
		Discharge(pReceiver, telegram);

		//remove it from the queue
		PriorityQ.erase(PriorityQ.begin());
	}
}



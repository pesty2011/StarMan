#pragma once


#include <set>
#include "EntityMessage.h"


class CBaseEntity;

const float SEND_MESSAGE_IMMEDIATELY = 0.0f;
const int NO_ADDITIONAL_INFO = 0;


#define Dispatch CMessageDispatch::Instance()



class CMessageDispatch
{
private:
	// set is used as a container for the delayed messages
	// because of the benefit of automatic sorting and avoidance
	// of duplicates.  messages will be sorted by their dispatch
	// time.
	std::set<EntityMessage> PriorityQ;


	void Discharge(CBaseEntity* pReciever, const EntityMessage& msg);

	CMessageDispatch();
	~CMessageDispatch();


//	CMessageDispatch(const CMessageDispatch&);
//	CMessageDispatch& operator=(const CMessageDispatch&);


public:
	// this class is a singleton
	static	CMessageDispatch* Instance();

	//send a message to another agent. Receiving agent is referenced by ID.
	void DispatchEntityMessage(float  delay,
		int    sender,
		int    receiver,
		int    msg,
		int		data,
		void*  ExtraInfo);

	//send out any delayed messages. This method is called each time through   
	//the main game loop.
	void DispatchDelayedMessages();

};


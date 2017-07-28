#pragma once

#include <iostream>
#include <math.h>

#define MSG_MASK_TYPE(a)		(((a) & 0xFF000000) >> 24)
#define MSG_MASK_EVENT(a)		((a) & 0x00FFFFFF)

#define CREATE_MESSAGE(a,b)		((a)<<24 | (b))



enum
{
	MSGTYPE_EVENT,								// event occurrence
	MSGTYPE_TRANSITION,							// transition event triggered
	MSGTYPE_PLAYBACK,							// message from animation playback
	MSGTYPE_COLLISION							// collision occurred
};



enum
{
	MSGEVENT_UNKNOWN = 0,					// ERROR, unknown
	MSGEVENT_ANIMPLAYBACK_DONE,				// animation completed
	MSGEVENT_ANIMPLAYBACK_LOOP,				// looping just occurred
	MSGEVENT_ANIMPLAYBACK_BLEND,			// blend animation between 2 sequences
	MSGEVENT_ANIMPLAYBACK_PLAY,				// play a new animation

	MSGEVENT_COLLISION_COLLIDED,
};




/* ----------------------------------------------------------------------------
	Summary:
	When a collision occurs, deliver this in the extra info
	field of the message which can then be broken down.

---------------------------------------------------------------------------- */
typedef struct __CollisionEvent__
{
	int		From;
	int		To;
	string	BoneName;

} tCollisionEvent;




/* ----------------------------------------------------------------------------
	Summary:
	This defines an entity message.  An entity message is a data structure
	that records information in order to dispatch a message to a specified
	entity within the entity manager.

	Messages are used by the animation player, state machine and any other
	type of message that might need to be communicated to different 
	entities.



	Author:
	Rob Anderson
	Based upon Mat Buckland (Programming Game AI by Example)
---------------------------------------------------------------------------- */
struct EntityMessage
{
	int				Sender;				// who is sending message
	int				Receiver;			// who is receiving the message
	int				Msg;				// what is the message
	int				Data;
	float			DispatchTime;		// when was it dispatched
	void*			Info;				// pointer to any extra information required




	// constructor for a new message ...
	EntityMessage(double time, int sender, int receiver, int msg, int data, void* info = NULL) :
					Sender(sender),
					Receiver(receiver),
					Msg(msg),
					Data(data),
					Info(info)
	{}
};

const float	SmallestDelay = 0.25f;

inline bool operator==(const EntityMessage& m1, const EntityMessage& m2)
{
	return (fabs(m1.DispatchTime - m2.DispatchTime) < SmallestDelay) &&
		(m1.Sender == m2.Sender) &&
		(m1.Receiver == m2.Receiver) &&
		(m1.Msg == m2.Msg) &&
		(m1.Data == m2.Data);
}


inline bool operator<(const EntityMessage& m1, const EntityMessage& m2)
{
	if (m1 == m2)
	{
		return false;
	}
	else
	{
		return (m1.DispatchTime < m2.DispatchTime);
	}
}




inline std::ostream& operator<<(std::ostream& os, const EntityMessage& m)
{
	os << "Time: " << m.DispatchTime << "  Sender: " << m.Sender << "  Receiver: " << m.Receiver << "  Msg: " << m.Msg;

	return os;
}


// Helpful function for dereferencing the Info field to a required type
template <class T>
inline T DereferenceToType(void* p)
{
	return *(T*)(p);
}





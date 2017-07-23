#pragma once
using namespace std;

#include <string>

#include "AnimPlayer.h"


//class CAnimPlayer;
class FSMSystem;



enum EntityTypes
{
	EN_Unknown,
	EN_Bone,
	EN_Skeleton,
	EN_Actor,
	EN_Clip,
};



class CBaseEntity
{
private:
	// every entity has a unique ID value
	int						mID;

	// this is the next valid ID.  each time a new entity is instantiated,
	// this value will be updated.
	static int				mNextValidID;

	void SetID(int value)	{ mID = value; }

public:
	CBaseEntity(int id);
	CBaseEntity();
	~CBaseEntity();

	float					mPosition[3];	// current world position
	float					mForward[3];	// current forward orientation

	string					mName;			// name of animation base object

	CBaseEntity*			mParent;		// animation base object parent
	CAnimPlayer*			m_pAnim;		// the animation player which updates animation
	FSMSystem*				m_pFSM;			// animation state machine

	EntityTypes				mType;

	virtual CBaseEntity*	GetChild(int i);
	virtual void			InsertChild(CBaseEntity* child);
	virtual void			DeleteChild(CBaseEntity* child);
	virtual int				NumChildren();

	string					GetName();
	void					SetName(string name);


	void					Play(string animName);
	virtual void			Update(float dTime);
	virtual void			Display();
};


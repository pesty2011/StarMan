#pragma once
using namespace std;

#include <string>

#include "AnimPlayer.h"
#include "EntityMessage.h"

//class CAnimPlayer;
class FSMSystem;



enum EntityTypes
{
	EN_Unknown,
	EN_Actor,
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

	t3Point					mColour;		// colour of the entity
	t3Point					mPosition;		// current world position
	t3Point					mDir;			// facing direction 


	CBaseEntity*			m_pTarget;		// who is my facing target

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
	int						GetID() { return mID; }

	virtual void			Init();
	void					Play(string animName);
	virtual void			Update(float dTime);
	virtual void			Display();


	t3Point					GetPos() { return mPosition; }
	t3Point					GetColor() { return mColour; }
	t3Point					GetDir() { return mDir; }

	void					SetTarget(CBaseEntity* entity) { m_pTarget = entity; }
	CBaseEntity*			GetTarget() { return m_pTarget; }

	virtual bool			OnHandleMessage(const EntityMessage& msg);


	// Collider system stuff
public:
	bool					GetBonePos(string name, t3Point* pt);

};


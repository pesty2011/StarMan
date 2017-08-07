#pragma once
using namespace std;

#include <string>

#include "AnimPlayer.h"
#include "EntityMessage.h"
#include "glm\vec3.hpp"

class FSMSystem;

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

	glm::vec3				mColour;		// colour of the entity
	glm::vec3				mPosition;		// current world position
	glm::vec3				mDir;			// facing direction 

	string					mName;			// name of animation base object

	CBaseEntity*			mParent;		// animation base object parent
	CAnimPlayer*			m_pAnim;		// the animation player which updates animation
	FSMSystem*				m_pFSM;			// animation state machine

	string					GetName();
	void					SetName(string name);
	int						GetID() { return mID; }

	virtual void			Init();
	void					Play(string animName);
	virtual void			Update(float dTime);
	virtual void			Display();


	glm::vec3				GetPos() { return mPosition; }
	glm::vec3				GetColor() { return mColour; }
	glm::vec3				GetDir() { return mDir; }

	virtual bool			OnHandleMessage(const EntityMessage& msg);


	// Collider system stuff
public:
	bool					GetBonePos(string name, glm::vec3* pt);

};


#pragma once
using namespace std;


#include <string>



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
public:
	CBaseEntity();
	~CBaseEntity();

	string					mName;			// name of animation base object

	CBaseEntity*			mParent;		// animation base object parent
	EntityTypes				mType;

	virtual CBaseEntity*	GetChild(int i);
	virtual void			InsertChild(CBaseEntity* child);
	virtual void			DeleteChild(CBaseEntity* child);
	virtual int				NumChildren();


	string					GetName();
	void					SetName(string name);
};


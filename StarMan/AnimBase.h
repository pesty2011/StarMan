#pragma once
using namespace std;


#include <string>



enum eABTypes
{
	ABT_Unknown,
	ABT_Bone,
	ABT_Skeleton,
	

};

class CAnimBase
{
public:
	CAnimBase();
	~CAnimBase();

	string			mName;			// name of animation base object

	CAnimBase*		mParent;		// animation base object parent
	eABTypes		mType;

	virtual CAnimBase*		GetChild(int i);
	virtual void			InsertChild(CAnimBase* child);
	virtual void			DeleteChild(CAnimBase* child);
	virtual int				NumChildren();

	string					GetName();

};


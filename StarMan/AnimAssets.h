#pragma once
#include "Skeleton.h"
#include "Joint.h"
#include "Anim.h"
#include "AnimFrame.h"
#include "Channel.h"

#include <string>



class CAnimAssets
{
public:
	CAnimAssets();
	~CAnimAssets();

private:
	CJoint*	LoadJoint(std::istream& stream, CJoint* parent = NULL);
	void LoadHierarchy(std::istream& stream);
	void LoadMotion(std::istream& stream);

	CSkeleton*		m_pSkeleton;
	CAnim*			m_pAnim;

public:
	void Load(const std::string& filename);

};


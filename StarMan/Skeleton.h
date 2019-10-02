#pragma once
#include "Joint.h"

class CSkeleton
{
public:
	CSkeleton();
	~CSkeleton();


	CJoint*			m_pRoot;
	unsigned int	m_NumChannels;


};


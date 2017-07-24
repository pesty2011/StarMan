#pragma once
#include <vector>
#include "3Dutil.h"


class CFrame
{
public:
	CFrame();
	~CFrame();

	int								m_nNumAngles;
	std::vector<tEulerAngle*>		m_Angles;

	t3Point							m_Offset;
	float							m_fTime;
};


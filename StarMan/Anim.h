#pragma once
#include <vector>
#include "Frame.h"


class CAnim
{
public:
	CAnim();
	~CAnim();

	int								m_nNumBones;
	std::vector<CFrame>				m_Frames;
	int								m_Rate;

	bool							m_bLoop;
	bool							m_bLocomotion;
};


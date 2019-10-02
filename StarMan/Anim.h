#pragma once
#include <vector>
#include "AnimFrame.h"

class CAnim
{
public:
	CAnim();
	~CAnim();

	unsigned int				m_NumFrames;
	unsigned int				m_NumMotionChannels;
	
	std::vector<CAnimFrame*>	m_MotionData;
	unsigned*					m_JointChannelOffsets;			// number of channels from beginning of hierarchy for i-th joint
};


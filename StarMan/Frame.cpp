#include "Frame.h"



CFrame::CFrame()
{
	m_nNumAngles = 0;
	m_Offset.x = m_Offset.y = m_Offset.z = 0.0f;
	m_fTime = 0.0f;
}


CFrame::~CFrame()
{
	// clean up the memory of the angles in the frame data
	for (int i = 0; i < m_Angles.size(); i++)
	{
		tEulerAngle*	angle = m_Angles[i];
		delete angle;
	}
	m_Angles.clear();
}

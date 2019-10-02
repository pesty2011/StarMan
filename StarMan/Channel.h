#pragma once


typedef enum __ChannelEnum__
{
	X_ROTATION, Y_ROTATION, Z_ROTATION,
	X_POSITION, Y_POSITION, Z_POSITION

} ChannelDefs;


class CJoint;

class CChannel
{
public:
	CChannel();
	~CChannel();

	CJoint*					m_pJoint;
	ChannelDefs				m_Type;
	unsigned int			m_Index;
};


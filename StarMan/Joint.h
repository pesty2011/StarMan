#pragma once
#include <vector>
#include "glm\vec3.hpp"
#include "glm\mat4x4.hpp"
class CJoint
{
public:
	CJoint();
	~CJoint();

	char*					m_pName;				// name of the joint
	unsigned int			m_Index;

	CJoint*					m_pParent;				// this joint's parent joint
	std::vector<CJoint*>	m_Children;				// array of joint's children

	glm::vec3				m_Offset;				// offset data for joint

	unsigned int			m_NumChannels;			// number of channels this joint has
	short*					m_ChannelOrder;			// ordered list of channels
	unsigned int			m_ChannelStart;			// index of joint's channel data in motion array

	// ? what is site
	bool					m_bSite;
	glm::vec3				m_Site;


	glm::mat4				m_Matrix;				// local transformation matrix (pre-multiplied with parent)
};


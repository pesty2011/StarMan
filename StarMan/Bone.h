#pragma once
#include <string>
#include <vector>

#include "BaseEntity.h"



class CBone :
	public CBaseEntity
{
public:
	CBone();
	~CBone();

	std::string				m_Name;
	int						m_Index;
	CBone*					m_pParent;
	std::vector<CBone*>		m_Children;

	float					m_Offset[3];
	float					m_Angle[3];
};


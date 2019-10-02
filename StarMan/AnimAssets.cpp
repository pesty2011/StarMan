#include "AnimAssets.h"
#include <iostream>
#include <fstream>

#include "Util.h"

CAnimAssets::CAnimAssets()
{
}


CAnimAssets::~CAnimAssets()
{
}



void CAnimAssets::Load(const std::string& filename)
{
	std::fstream file;
	file.open(filename.c_str(), std::ios_base::in);

	if (file.is_open())
	{
		std::string line;

		while (file.good())
		{
			file >> line;

			if (trim(line) == "HIERARCHY")
			{
				m_pSkeleton = new CSkeleton();
				m_pAnim = new CAnim();
				LoadHierarchy(file);
			}
			break;
		}
		file.close();
	}
}



CJoint*	CAnimAssets::LoadJoint(std::istream& stream, CJoint* parent)
{
	CJoint* joint = new CJoint();
	joint->m_pParent = parent;



	// load the joint name
	std::string* name = new std::string;
	stream >> *name;
	joint->m_pName = name->c_str();



	return joint;
}




void CAnimAssets::LoadHierarchy(std::istream& stream)
{
	std::string tmp;

	while (stream.good())
	{
		stream >> tmp;

		if (trim(tmp) == "ROOT")
		{
			m_pSkeleton->m_pRoot = LoadJoint(stream);
		}
		else if (trim(tmp) == "MOTION")
		{
			LoadMotion(stream);
		}
	}
}




void CAnimAssets::LoadMotion(std::istream& stream)
{

}

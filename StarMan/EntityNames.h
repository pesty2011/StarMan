#pragma once
#include <string>


enum
{
	StarMan_1,
	StarMan_2
};


inline std::string GetNameOfEntity(int n)
{
	switch (n)
	{
	case StarMan_1:
		return "StarMan1";
		break;
	case StarMan_2:
		return "StarMan2";
		break;

	default:
		return "Unknown";
		break;
	}
}
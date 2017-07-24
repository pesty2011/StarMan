#pragma once
#include <string>


enum
{
	StarMan_1,
	StarMan_2,
	EntityTotal
};


static string EntityDesc[] =
{
	{ "StarMan1" },
	{ "StarMan2" }
};



inline std::string GetNameOfEntity(int n)
{
	if (n < EntityTotal)
	{
		return EntityDesc[n];
	}
	return "UnknownEntity";
}
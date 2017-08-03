#pragma once
#include "BaseEntity.h"

class Observer
{
public:
	virtual ~Observer();
	virtual void onNotify(const CBaseEntity& entity, int message) = 0;
};


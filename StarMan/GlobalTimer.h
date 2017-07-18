#pragma once
#include <stdio.h>
#include <Windows.h>

class GlobalTimer
{
	UINT64					mTimeCycles;
	double					mTimeScale;
	bool					mIsPaused;

public:
	GlobalTimer();
	~GlobalTimer();
};


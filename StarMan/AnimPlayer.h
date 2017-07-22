#pragma once

#include "BVH\BVH.h"

class CBaseEntity;


class CAnimPlayer
{
protected:
	float			mRate;
	bool			bLoop;
	bool			bIsPlaying;
	bool			bIsLoaded;

	int				mCurrFrame;
	int				mPrevFrame;

	float			mStartTime;
	float			mCurrTime;

	float			mInterval;
	float			mTotalTime;


	BVH*			m_pMotionData;		// the motion data
	CBaseEntity*	m_pOwner;			// who owns this animation player


public:
	CAnimPlayer();
	CAnimPlayer(CBaseEntity* owner);
	~CAnimPlayer();

	void SetLoop(bool loop) { bLoop = loop; }

	void Play();
	void Play(BVH* data);
	void Play(BVH* data, bool loop);
	void Play(string animName);

	void Stop();
	void Pause();

	void SetBVH(BVH* data);
	void SetBVH(string motionName);

	void Update(float dTime);
	void Display();
};


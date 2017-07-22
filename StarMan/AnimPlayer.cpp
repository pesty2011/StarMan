#include <iostream>

#include "AnimPlayer.h"
#include "AssetManager.h"


CAnimPlayer::CAnimPlayer(CBaseEntity* owner)
{
	mRate = 60.0f;
	bLoop = false;
	bIsPlaying = false;
	bIsLoaded = false;

	mCurrFrame = 0;
	mPrevFrame = 0;

	mStartTime = 0.0f;
	mCurrTime = 0.0f;

	mInterval = 0.0f;
	mTotalTime = 0.0f;

	m_pMotionData = NULL;

	m_pOwner = owner;

}

CAnimPlayer::CAnimPlayer()
{
	mRate = 60.0f;
	bLoop = false;
	bIsPlaying = false;
	bIsLoaded = false;

	mCurrFrame = 0;
	mPrevFrame = 0;

	mStartTime = 0.0f;
	mCurrTime = 0.0f;

	mInterval = 0.0f;
	mTotalTime = 0.0f;

	m_pMotionData = NULL;
	m_pOwner = NULL;
}


CAnimPlayer::~CAnimPlayer()
{
}




void CAnimPlayer::Play()
{
	cout << "===========================================" << endl;
	cout << "PLAY" << endl;
	cout << "NumFrames: " << m_pMotionData->GetNumFrame() << endl;
	cout << "Interval: " << mInterval << endl;
	cout << "TotalTime: " << mTotalTime << endl;
	cout << "===========================================" << endl;
}



void CAnimPlayer::Play(BVH* data)
{
	SetBVH(data);
}


void CAnimPlayer::Play(BVH* data, bool loop)
{
	SetBVH(data);
	SetLoop(loop);
}



void CAnimPlayer::Play(string animName)
{
	BVH* motion = AssetMgr->FindMotion(animName);
	if (motion != NULL)
	{
		SetBVH(motion);
	}
	else
	{
		cout << "[ERROR] animation not found: " << animName << endl;
	}
}




void CAnimPlayer::Stop()
{
	cout << "===========================================" << endl;
	cout << "STOP" << endl;
	cout << "===========================================" << endl;
}



void CAnimPlayer::Pause()
{
	cout << "===========================================" << endl;
	cout << "PAUSE" << endl;
	cout << "===========================================" << endl;
}



void CAnimPlayer::SetBVH(BVH* data)
{
	m_pMotionData = data;
	if (data != NULL)
	{
		bLoop = false;

		mInterval = (float)data->GetInterval();
		mTotalTime = (float)data->GetNumFrame() * mInterval;

		mCurrFrame = 0;
		mPrevFrame = 0;
	}
}


void CAnimPlayer::SetBVH(string motionName)
{
	m_pMotionData = AssetMgr->FindMotion(motionName);
	if (m_pMotionData != NULL)
	{
		bLoop = false;

		mInterval = (float)m_pMotionData->GetInterval();
		mTotalTime = (float)m_pMotionData->GetNumFrame() * mInterval;

		mCurrFrame = 0;
		mPrevFrame = 0;
	}
}



void CAnimPlayer::Update(float dTime)
{
	if (bIsPlaying)
	{
		mCurrTime += dTime;

		float animStep = mCurrTime;
		int frameinc = 0;
		while (animStep > 0)
		{
			animStep -= mInterval;
			if (animStep > 0.0f)
			{
				frameinc++;
			}
		}

		mCurrFrame += frameinc;
		mCurrFrame = mCurrFrame % m_pMotionData->GetNumFrame();
		mCurrFrame = 0;
	}
}





void CAnimPlayer::Display()
{
	if (bIsPlaying)
	{
		m_pMotionData->RenderFigure(0, 0.1f);
	}
}



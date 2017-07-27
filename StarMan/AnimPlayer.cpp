#include <iostream>

#include "BaseEntity.h"
#include "AnimPlayer.h"
#include "AssetManager.h"
#include "EntityMessage.h"
#include "MessageDispatch.h"


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
	cout << "PLAY: " << m_pMotionData->GetMotionName() << endl;
	cout << "NumFrames: " << m_pMotionData->GetNumFrame() << endl;
	cout << "Interval: " << mInterval << endl;
	cout << "TotalTime: " << mTotalTime << endl;
	cout << "===========================================" << endl;

	mFrameTime = 0.0f;
	mCurrFrame = 0;
	mPrevFrame = 0;

	bIsPlaying = true;
}



void CAnimPlayer::Play(BVH* data)
{
	SetBVH(data);
}


void CAnimPlayer::Play(BVH* data, bool loop)
{
	SetBVH(data);
	SetLoop(loop);

	Play();
}



void CAnimPlayer::Play(string animName)
{
	BVH* motion = AssetMgr->FindMotion(animName);
	if (motion != NULL)
	{
		SetBVH(motion);
		Play();
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
	bIsPlaying = false;
}



void CAnimPlayer::Pause()
{
	cout << "===========================================" << endl;
	cout << "PAUSE" << endl;
	cout << "===========================================" << endl;
	bIsPlaying = false;
}



void CAnimPlayer::SetBVH(BVH* data)
{
	m_pMotionData = data;
	if (data != NULL)
	{
		bLoop = false;

		mInterval = (float)data->GetInterval();
		mNumFrames = data->GetNumFrame();
		mTotalTime = (float)mNumFrames * mInterval;
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
#if false
		mFrameTime += dTime;

		float animStep = mFrameTime;
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
		mFrameTime = 0;
#else
		mFrameTime += dTime;
		float time = mFrameTime / mTotalTime;
		float frame = (float)m_pMotionData->GetNumFrame() * time;
		mPrevFrame = mCurrFrame;
		mCurrFrame = (int)frame;


		if (bLoop)
		{
			mCurrFrame = mCurrFrame % m_pMotionData->GetNumFrame();
		}
		else if (mFrameTime >= mTotalTime)
		{
			mCurrFrame = m_pMotionData->GetNumFrame() - 1;
			
			// send a message that we are done doing this move ...
			Dispatch->DispatchEntityMessage(SEND_MESSAGE_IMMEDIATELY, m_pOwner->GetID(), m_pOwner->GetID(), CREATE_MESSAGE(MSGTYPE_PLAYBACK, MSGEVENT_ANIMPLAYBACK_DONE) , 0, NULL);

		}


		// calculate the percentage moves here if we need to blend going 
		// into another move or not ...


#endif
	}
}





void CAnimPlayer::Display()
{
	if (m_pMotionData)
	{
		t3Point pos = m_pOwner->GetPos();
		t3Point colour = m_pOwner->GetColor();
		glColor3f(colour.v[0], colour.v[1], colour.v[2]);

		m_pMotionData->SetDir(m_pOwner->GetDir());
		m_pMotionData->RenderFigure(pos, mCurrFrame, 0.05f);
	}
}



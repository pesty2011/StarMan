#include <iostream>

#include "BaseEntity.h"
#include "EntityNames.h"
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
	mDeltaTime = 0.0f;

	mCurrFrame = 0;
	mPrevFrame = 0;
	mNextFrame = 0;

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



bool CAnimPlayer::GetBonePos(std::string name, t3Point* pt)
{
	if (m_pMotionData)
	{
		return m_pMotionData->GetBonePos(name, pt);
	}
	return false;
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

		t3Point colour = m_pOwner->GetColor();
		data->SetBonesBaseColour(colour.x, colour.y, colour.z);
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

		t3Point colour = m_pOwner->GetColor();
		m_pMotionData->SetBonesBaseColour(colour.x, colour.y, colour.z);
	}
}


/* ----------------------------------------------------------------------------
	Summary:
	The Update method will apply the delta time to the animation timeline
	and calculate out the different frames.

	The method stores off the previous frame and updates the 

	Parameters:
	[in] dTime : delta time since last frame

---------------------------------------------------------------------------- */
void CAnimPlayer::Update(float dTime)
{
	if (bIsPlaying)
	{
		mFrameTime += dTime;
		float time = mFrameTime / mTotalTime;
		float frame = (float)m_pMotionData->GetNumFrame() * time;
		float interval = (float)m_pMotionData->GetInterval();
		
		
		mPrevFrame = mCurrFrame;
		mCurrFrame = (int)frame;
		mNextFrame = mCurrFrame + 1;


		if (bLoop)
		{
			mCurrFrame = mCurrFrame % m_pMotionData->GetNumFrame();

			// what is the next frame to play
			mNextFrame = mCurrFrame + 1;
			mNextFrame = mNextFrame % m_pMotionData->GetNumFrame();
		}
		else if (mFrameTime >= mTotalTime)
		{
			mCurrFrame = m_pMotionData->GetNumFrame() - 1;

			// what is the next frame to play
			mNextFrame = mCurrFrame + 1;
			mNextFrame = mNextFrame % m_pMotionData->GetNumFrame();

			// send a message that we are done doing this move ...
			Dispatch->DispatchEntityMessage(SEND_MESSAGE_IMMEDIATELY, m_pOwner->GetID(), m_pOwner->GetID(), CREATE_MESSAGE(MSGTYPE_PLAYBACK, MSGEVENT_ANIMPLAYBACK_DONE) , 0, NULL);
		}


		// calculate the percentage moves here if we need to blend going 
		// into another move or not ...
		float startTime = mCurrFrame * interval;
		float endTime = mNextFrame * interval;
		mDeltaTime = (mFrameTime - startTime) / interval;

#if _DEBUG
		if (m_pOwner->GetID() == StarMan_1)
		{
//			cout << "Delta: " << mDeltaTime << " From: " << mCurrFrame << " : " << mNextFrame << " Interval: " << interval << endl;
		}
#endif
	}


	// update the animation timers, just for visually changing the bone colours.
	if (m_pMotionData)
	{
		m_pMotionData->UpdateTimers(dTime);
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
		m_pMotionData->SetPos(m_pOwner->GetPos());
		m_pMotionData->RenderFigure(mCurrFrame, mNextFrame, 0.05f, mDeltaTime);

		m_pMotionData->RenderBones();



#if false
		glColor3f(1.0f, 0.0f, 0.0f);
		glPushMatrix();
		glLoadIdentity();
		t3Point hip = m_pMotionData->GetHipPosition();
		glTranslatef(hip.x, hip.y, hip.z);

		static GLUquadric*  pQuad = NULL;
		if (pQuad == NULL)
		{
			// create a new quadric primative
			pQuad = gluNewQuadric();
		}

		gluQuadricDrawStyle(pQuad, GLU_LINE);
		gluQuadricNormals(pQuad, GLU_FLAT);


		gluSphere(pQuad, 0.1f, 10.0f, 3.0f);

		glPopMatrix();
#endif
	}
}



void CAnimPlayer::SetBoneColour(string boneName, float time, float r, float g, float b)
{
	if (m_pMotionData)
	{
		m_pMotionData->SetBoneColour(boneName, time, r, g, b);
	}
}

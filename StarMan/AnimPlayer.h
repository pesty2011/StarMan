#pragma once
#include <GL\glew.h>
#include <GL\freeglut.h>

#include "BVH\BVH.h"

class CBaseEntity;


class CAnimPlayer
{
protected:
	float			mRate;				// playback rate
	bool			bLoop;				// do we loop the playback
	bool			bIsPlaying;			// is an animation playing
	bool			bIsLoaded;			// is an animation loaded

	int				mCurrFrame;			// current frame we are displaying
	int				mPrevFrame;			// previous frame we displayed
	int				mNextFrame;			// what is the next frame
	int				mNumFrames;


	float			mStartTime;			// starting time of the animation
	float			mCurrTime;			// current time
	float			mFrameTime;			// current animation time
	float			mDeltaTime;


	float			mInterval;			// intervals between keys
	float			mTotalTime;			// total time of animation


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

	bool GetBonePos(std::string name, glm::vec3* pt);
	void SetBoneColour(string boneName, float time, float r, float g, float b);

};


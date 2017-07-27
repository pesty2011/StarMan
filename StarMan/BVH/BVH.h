/**
	BVH Player
	Copyright (c) 2004-2017, Masaki OSHITA (www.oshita-lab.org)
	Released under the MIT license http://opensource.org/licenses/mit-license.php

	Modified by Rob Anderson.

	Added in cache system in order to cache modelview point.
	Added bone colour


*/


#ifndef  _BVH_H_
#define  _BVH_H_


#include <vector>
#include <map>
#include <string>


#include "..\3Dutil.h"

using namespace  std;


class  BVH
{
public:
	// predefine for 
	struct Joint;
	struct Channel;


	enum  ChannelEnum
	{
		X_ROTATION, Y_ROTATION, Z_ROTATION,
		X_POSITION, Y_POSITION, Z_POSITION
	};



	struct Channel
	{
		Joint*               joint;
		ChannelEnum          type;
		int                  index;
	};



	struct Joint
	{
		string				name;					// name of the joint
		int					index;					// index ?
		Joint*				parent;					// parent joint
		vector<Joint*>		children;				// number of children in joint
		double				offset[3];				// offset of the joint
		bool				has_site;				// ?
		double				site[3];				// ?
		vector<Channel*>	channels;				// channels in the joint

		float				colour[3];				// colour of the joint
		float				baseColour[3];			// default colour
		float				timer;					// timer for bone colour
	};


private:
	typedef std::map<string, t3Point> BoneMap;
	typedef std::map<string, Joint*> JointIndexMap;
	typedef std::vector<Joint*>  JointVector;
	typedef std::vector<Channel*> ChannelVector;



private:
	bool					isLoaded;

	string					file_name;
	string					motion_name;

	int						m_NumChannels;			// number of channels in capture data
	vector<Channel*>		m_Channels;				// array of all of the channel pointers
	vector<Joint*>			m_Joints;					// vector of joint pointers
	JointIndexMap			m_JointIndexMap;		// map lookup of all of the joints

	bool					m_bRawData;

	// this represents the stripped version of the BVH 
	// motion data.
	double*					m_pAnimData;
	int						m_NumAnimFrames;
	double					m_AnimInterval;


	int						m_NumFrames;
	double					m_Interval;
	double*					m_pMotion;



	t3Point					m_Pos;
	t3Point					m_Dir;
	t3Point					m_HipPosition;
	BoneMap					m_BoneMap;

// Constructor, Deconstructor stuff
public:
	BVH();
	BVH(const char* pBVHFilename);
	~BVH();

	void			Clear();
	void			Load( const char * bvh_file_name );

public:
	bool			IsLoadSuccess() const { return isLoaded; }
	void			ToggleRawData() { m_bRawData ^= true; }
	bool			IsRawData() { return m_bRawData; }


	const string&   GetFileName() const { return file_name; }
	const string&   GetMotionName() const { return motion_name; }

	
	const int       GetNumJoint() const { return  m_Joints.size(); }
	const Joint*    GetJoint( int no ) const { return  m_Joints[no]; }
	const int       GetNumChannel() const { return  m_Channels.size(); }
	const Channel*  GetChannel( int no ) const { return  m_Channels[no]; }


	const Joint*    GetJoint( const string& jointName ) const  {
		JointIndexMap::const_iterator  i = m_JointIndexMap.find(jointName);
		return  ( i != m_JointIndexMap.end() ) ? (*i).second : NULL; 
	}


	const Joint*   GetJoint(const char* jointName ) const  {
		JointIndexMap::const_iterator  i = m_JointIndexMap.find(jointName);
		return  ( i != m_JointIndexMap.end() ) ? (*i).second : NULL; 
	}



	// custom colour functions
	void			SetBonesBaseColour(float r, float g, float b);
	void			SetBoneColour(string boneName, float time, float r, float g, float b);


	t3Point			Lerp(float time, t3Point a, t3Point b);
	void			StripBVHFile(float amount);
	
	int	GetNumFrame() const 
	{ 
		if (m_bRawData == true)
			return  m_NumFrames;
		else
			return m_NumAnimFrames;
	}


	double GetInterval() const 
	{ 
		if (m_bRawData == true)
			return  m_Interval;
		else
			return m_AnimInterval;
	}


	// Rendering methods for the BVH system
public:
	void			RenderFigure(int frameNum, float scale = 1.0f);
	void			RenderFigure( const Joint* root, const double* data, float scale = 1.0f );
	void			RenderBones();
	void			RenderBone(const Joint* joint, float x0, float y0, float z0, float x1, float y1, float z1 );

	void			SetDir(t3Point facing) { m_Dir = facing; }
	void			SetPos(t3Point pos) { m_Pos = pos; }
};


#endif // _BVH_H_

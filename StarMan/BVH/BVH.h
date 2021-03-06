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
#include "..\glm\vec3.hpp"


using namespace  std;

#define  BUFFER_LENGTH  1024*32					// 32K line buffer


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
	typedef std::map<string, glm::vec3> BoneMap;
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
	bool					m_bUseTranslation;

	// this represents the stripped version of the BVH 
	// motion data.
	double*					m_pAnimData;
	int						m_NumAnimFrames;
	double					m_AnimInterval;


	int						m_NumFrames;
	double					m_Interval;
	double*					m_pMotion;



	glm::vec3				m_Pos;
	glm::vec3				m_Dir;
	glm::vec3				m_HipPosition;
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

	void			ToggleTrans() { m_bUseTranslation ^= true; }
	bool			IsTrans() { return m_bUseTranslation; }


	const string&   GetFileName() const { return file_name; }
	const string&   GetMotionName() const { return motion_name; }

	
	const int       GetNumJoint() const { return  static_cast<const int>(m_Joints.size()); }
	const Joint*    GetJoint( int no ) const { return  m_Joints[no]; }
	const int       GetNumChannel() const { return  static_cast<const int>(m_Channels.size()); }
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


	glm::vec3		Lerp(float time, glm::vec3 a, glm::vec3 b);
	float			Lerp(float time, float a, float b);
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
	void			RenderFigure(int frameNum, int nextFrame, float scale, float time);
	void			RenderFigure( const Joint* root, const double* data, float scale = 1.0f );
	void			RenderFigureInterp(const Joint* joint, const double* pCurr, const double* pNext, float scale, float time);
	void			RenderBones();
	void			RenderBone(const Joint* joint, float x0, float y0, float z0, float x1, float y1, float z1 );

	inline void		SetDir(glm::vec3 facing) { m_Dir = facing; }
	inline void		SetPos(glm::vec3 pos) { m_Pos = pos; }

	bool			GetBonePos(string name, glm::vec3* pt);

	void			UpdateTimers(float dTime);

private:
		char				line[BUFFER_LENGTH];
};


#endif // _BVH_H_

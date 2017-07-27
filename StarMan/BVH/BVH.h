/**
***  BVH Player
***  Copyright (c) 2004-2017, Masaki OSHITA (www.oshita-lab.org)
***  Released under the MIT license http://opensource.org/licenses/mit-license.php
**/


#ifndef  _BVH_H_
#define  _BVH_H_


#include <vector>
#include <map>
#include <string>

#include "..\3Dutil.h"

using namespace  std;


static const char Xrotation[] = "Xrotation";
static const char Yrotation[] = "Yrotation";
static const char Zrotation[] = "Zrotation";

static const char Xposition[] = "Xposition";
static const char Yposition[] = "Yposition";
static const char Zposition[] = "Zposition";




class  BVH
{
private:
	typedef std::map<string, t3Point> BoneMap;

public:
	enum  ChannelEnum
	{
		X_ROTATION, Y_ROTATION, Z_ROTATION,
		X_POSITION, Y_POSITION, Z_POSITION
	};
	
	struct  Joint;							// define this for Channel
	
	struct Channel
	{
		Joint*               joint;
		ChannelEnum          type;
		int                  index;
	};



	struct Joint
	{
		string				name;
		int					index;
		Joint*				parent;
		vector<Joint*>		children;
		double				offset[3];
		bool				has_site;
		double				site[3];
		vector<Channel*>	channels;
	};


private:
	bool					isLoaded;

	string					file_name;
	string					motion_name;

	int						numChannels;			// number of channels in capture data
	vector<Channel*>		channels;				// array of all of the channel pointers
	vector<Joint*>			joints;					// vector of joint pointers
	map<string, Joint*>		joint_index;			// map lookup of all of the joints

	int						m_NumFrames;
	double					m_Interval;
	double*					motion;

	t3Point					m_Pos;
	t3Point					m_Dir;
	t3Point					m_HipPosition;
	BoneMap					m_BoneMap;

// Constructor, Deconstructor stuff
public:
	BVH();
	BVH( const char * bvh_file_name );
	~BVH();

	void			Clear();
	void			Load( const char * bvh_file_name );

public:
	bool			IsLoadSuccess() const { return isLoaded; }

	const string&   GetFileName() const { return file_name; }
	const string&   GetMotionName() const { return motion_name; }

	
	const int       GetNumJoint() const { return  joints.size(); }
	const Joint*    GetJoint( int no ) const { return  joints[no]; }
	const int       GetNumChannel() const { return  channels.size(); }
	const Channel*  GetChannel( int no ) const { return  channels[no]; }


	const Joint*    GetJoint( const string& jointName ) const  {
		map<string, Joint*>::const_iterator  i = joint_index.find(jointName);
		return  ( i != joint_index.end() ) ? (*i).second : NULL; 
	}


	const Joint*   GetJoint(const char* jointName ) const  {
		map<string, Joint*>::const_iterator  i = joint_index.find(jointName);
		return  ( i != joint_index.end() ) ? (*i).second : NULL; 
	}

	
	int				GetNumFrame() const { return  m_NumFrames; }
	double			GetInterval() const { return  m_Interval; }
	double			GetMotion( int f, int c ) const { return  motion[ f*numChannels + c ]; }
	void			SetMotion( int f, int c, double v ) { motion[ f*numChannels + c ] = v; }

	// Rendering methods for the BVH system
public:
	void			RenderFigure(int frame_no, float scale = 1.0f);
	void			RenderFigure(t3Point pos, int frameNum, float scale = 1.0f);
	void			RenderBindFigure(t3Point pos, int frameNum, float scale);
	void			RenderFigure( const Joint* root, const double* data, float scale = 1.0f );
	void			RenderBindPose(const Joint* pJoint, const double* data, float scale = 1.0f);
	void			RenderBones();
	void			RenderBone( float x0, float y0, float z0, float x1, float y1, float z1 );

	void			SetDir(t3Point facing) { m_Dir = facing; }
	void			SetPos(t3Point pos) { m_Pos = pos; }
	t3Point			GetHipPosition() { return m_HipPosition; }

};



#endif // _BVH_H_

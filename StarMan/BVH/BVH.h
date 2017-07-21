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

using namespace  std;


static const char Xrotation[] = "Xrotation";
static const char Yrotation[] = "Yrotation";
static const char Zrotation[] = "Zrotation";

static const char Xposition[] = "Xposition";
static const char Yposition[] = "Yposition";
static const char Zposition[] = "Zposition";



class  BVH
{
public:
	enum  ChannelEnum
	{
		X_ROTATION, Y_ROTATION, Z_ROTATION,
		X_POSITION, Y_POSITION, Z_POSITION
	};
	
	struct  Joint;							// define this for Channel
	
	struct Channel
	{
		Joint *              joint;
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

	int						numFrames;
	double					interval;
	double*					motion;

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

	
	int				GetNumFrame() const { return  numFrames; }
	double			GetInterval() const { return  interval; }
	double			GetMotion( int f, int c ) const { return  motion[ f*numChannels + c ]; }
	void			SetMotion( int f, int c, double v ) { motion[ f*numChannels + c ] = v; }

	// Rendering methods for the BVH system
public:
	void			RenderFigure( int frame_no, float scale = 1.0f );
	static void		RenderFigure( const Joint* root, const double* data, float scale = 1.0f );
	static void		RenderBone( float x0, float y0, float z0, float x1, float y1, float z1 );
};



#endif // _BVH_H_

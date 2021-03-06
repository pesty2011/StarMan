/**
***  BVH Player
***  Copyright (c) 2004-2017, Masaki OSHITA (www.oshita-lab.org)
***  Released under the MIT license http://opensource.org/licenses/mit-license.php
**/

#include <fstream>
#include <string.h>
#include <math.h>
#include <iostream>
#include <GL/glut.h>
#include "..\glm\vec3.hpp"

#include "BVH.h"


BVH::BVH()
{
	m_pMotion = NULL;
	Clear();

	m_Pos = glm::vec3(0.0f, 0.0f, 0.0f);
	m_Dir = glm::vec3(0.0f, 0.0f, 0.0f);


	m_bRawData = true;
	m_bUseTranslation = true;
}


BVH::BVH(const char * bvh_file_name)
{
	m_pMotion = NULL;
	Clear();
	Load(bvh_file_name);

	m_Pos = glm::vec3(0.0f, 0.0f, 0.0f);
	m_Dir = glm::vec3(0.0f, 0.0f, 0.0f);

	m_bRawData = true;
	m_bUseTranslation = true;
}


BVH::~BVH()
{
	Clear();
}



void  BVH::Clear()
{
	unsigned int channelSize = static_cast<int>(m_Channels.size());
	unsigned int jointSize = static_cast<int>(m_Joints.size());

	unsigned int  i;

	for (i = 0; i < channelSize; i++)
		delete m_Channels[i];

	for (i = 0; i < jointSize; i++)
		delete m_Joints[i];

	if (m_pMotion != NULL)
		delete m_pMotion;

	if (m_pAnimData != NULL)
		delete m_pAnimData;

	m_BoneMap.clear();

	isLoaded = false;
	
	file_name = "";
	motion_name = "";
	
	m_NumChannels = 0;
	m_Channels.clear();
	m_Joints.clear();
	m_JointIndexMap.clear();
	
	m_NumFrames = 0;
	m_Interval = 0.0;
	m_pMotion = NULL;
}




/* ----------------------------------------------------------------------------
	Summary:
	Load a BVH file into the class for rendering.  This function has been
	modified from the original method giving it the ability to strip out
	much of the dense data.

	This is done in the motion section of the BVH file format and will 
	strip out most of the motion, adjust the interval and modify the
	number of frames that are actually in the animation data.

	Parameters:
	[in] bvh_file_name : pointer to the bvh file to load

---------------------------------------------------------------------------- */
void  BVH::Load(const char * bvh_file_name )
{

	ifstream			file;
	char*				token;
	char				separator[] = " :,\t";
	vector<Joint*>		joint_stack;
	Joint*				joint = NULL;
	Joint*				new_joint = NULL;
	bool				is_site = false;
	double				x, y ,z;
	int					i, j;


	Clear();

	file_name = bvh_file_name;											// save the filename

	// do some manipulation to strip out a lot of the filename 
	// in order to create a unique motion file name based on the
	// filename itself.  
	// TODO: remove this and come up with a better scheme.
	const char* mn_first = bvh_file_name;
	const char* mn_last = bvh_file_name + strlen( bvh_file_name );

	if (strrchr(bvh_file_name, '\\') != NULL)
	{
		mn_first = strrchr(bvh_file_name, '\\') + 1;
	}
	else if (strrchr(bvh_file_name, '/') != NULL)
	{
		mn_first = strrchr(bvh_file_name, '/') + 1;
	}

	if (strrchr(bvh_file_name, '.') != NULL)
	{
		mn_last = strrchr(bvh_file_name, '.');
	}


	if (mn_last < mn_first)
	{
		mn_last = bvh_file_name + strlen(bvh_file_name);
	}
	motion_name.assign( mn_first, mn_last );


	char path[256];
	GetCurrentDirectoryA(sizeof(path), path);
	std::string workingDirectory(path);
	workingDirectory = workingDirectory + std::string("\\..\\mocap\\") + std::string(bvh_file_name);


	//### open the file to parse it ...
	file.open( workingDirectory.c_str(), ios::in );
	if (file.is_open() == 0)
	{
		// failed to load the file
		return; 
	}



	// loop through the file until we reach the end of it.
	while ( !file.eof() )
	{
		if ( file.eof() )  
			goto bvh_error;

		// read a line from the file
		file.getline( line, BUFFER_LENGTH );

		// separate out the tokens using the separators as conditions
		token = strtok( line, separator );

		// check if token is valid
		if ( token == NULL )  
			continue;

		// is the token a collection of values
		if ( strcmp( token, "{" ) == 0 )
		{
			joint_stack.push_back( joint );
			joint = new_joint;
			continue;
		}

		// end of the joint
		if ( strcmp( token, "}" ) == 0 )
		{
			
			joint = joint_stack.back();
			joint_stack.pop_back();
			is_site = false;
			continue;
		}

		// roots or joint?
		if ( ( strcmp( token, "ROOT" ) == 0 ) ||
		     ( strcmp( token, "JOINT" ) == 0 ) )
		{
			// create a new joint ...
			new_joint = new Joint();
			
			new_joint->index = static_cast<int>(m_Joints.size());
			new_joint->parent = joint;
			new_joint->has_site = false;

			new_joint->offset[0] = 0.0;  
			new_joint->offset[1] = 0.0;  
			new_joint->offset[2] = 0.0;

			new_joint->site[0] = 0.0;  
			new_joint->site[1] = 0.0;  
			new_joint->site[2] = 0.0;

			new_joint->colour[0] = 0.0f;
			new_joint->colour[1] = 0.0f;
			new_joint->colour[2] = 0.0f;

			new_joint->baseColour[0] = 0.8f;
			new_joint->baseColour[1] = 0.8f;
			new_joint->baseColour[2] = 0.8f;

			m_Joints.push_back(new_joint);

			// check if there is a parent joint
			if (joint)
			{
				joint->children.push_back(new_joint);
			}

			token = strtok( NULL, "" );
			while ( *token == ' ' )  token++;
			new_joint->name = token;

			m_JointIndexMap[ new_joint->name ] = new_joint;

			// add in cache working frame ...
			m_BoneMap[new_joint->name] = glm::vec3(0.0f, 0.0f, 0.0f);
			continue;
		}

		if ( ( strcmp( token, "End" ) == 0 ) )
		{
			new_joint = joint;
			is_site = true;
			continue;
		}

		if ( strcmp( token, "OFFSET" ) == 0 )
		{
			token = strtok( NULL, separator );
			x = token ? atof( token ) : 0.0;
			token = strtok( NULL, separator );
			y = token ? atof( token ) : 0.0;
			token = strtok( NULL, separator );
			z = token ? atof( token ) : 0.0;
			
			if ( is_site )
			{
				joint->has_site = true;
				joint->site[0] = x;
				joint->site[1] = y;
				joint->site[2] = z;
			}
			else
			{
				joint->offset[0] = x;
				joint->offset[1] = y;
				joint->offset[2] = z;
			}
			continue;
		}



		if ( strcmp( token, "CHANNELS" ) == 0 )
		{
			// resize the channels vector inside the joint to accomodate new channels
			token = strtok( NULL, separator );						// get the number of channels
			joint->channels.resize( token ? atoi( token ) : 0 );

			// go through all of the channels and parse channel data
			for ( i = 0; i < (int)joint->channels.size(); i++ )
			{
				// create a new channel
				Channel* channel = new Channel();

				channel->joint = joint;
				channel->index = static_cast<int>(m_Channels.size());
				m_Channels.push_back(channel);

				joint->channels[i] = channel;

				// parse the channel data types
				token = strtok( NULL, separator );

				if ( strcmp( token, "Xrotation") == 0 )
					channel->type = X_ROTATION;
				else if ( strcmp( token, "Yrotation" ) == 0 )
					channel->type = Y_ROTATION;
				else if ( strcmp( token, "Zrotation" ) == 0 )
					channel->type = Z_ROTATION;
				else if ( strcmp( token, "Xposition" ) == 0 )
					channel->type = X_POSITION;
				else if ( strcmp( token, "Yposition" ) == 0 )
					channel->type = Y_POSITION;
				else if ( strcmp( token, "Zposition" ) == 0 )
					channel->type = Z_POSITION;
			}
		}

		// We are done once we find MOTION keyword, so break out of 
		// the loop so we can parse the motion section of the BVH 
		// file.
		if ( strcmp( token, "MOTION" ) == 0 )
			break;
	}


	file.getline( line, BUFFER_LENGTH );
	token = strtok( line, separator );
	if ( strcmp( token, "Frames" ) != 0 )  
		goto bvh_error;

	// Number of Frames
	token = strtok( NULL, separator );
	if ( token == NULL )  
		goto bvh_error;

	m_NumFrames = atoi( token );


	file.getline( line, BUFFER_LENGTH );
	token = strtok( line, ":" );

	// Interval between each frame ...
	if ( strcmp( token, "Frame Time" ) != 0 )  
		goto bvh_error;

	token = strtok( NULL, separator );
	if ( token == NULL )  
		goto bvh_error;

	m_Interval = atof( token );

	// save off the number of channels ...
	m_NumChannels = static_cast<int>(m_Channels.size());

	// create a motion array to accomodate the number 
	// of frames and the number of channels in the BVH section
	m_pMotion = new double[ m_NumFrames * m_NumChannels ];


	// loop through all of the frames
	for (i = 0; i < m_NumFrames; i++)
	{
		file.getline( line, BUFFER_LENGTH );
		token = strtok( line, separator );
		for (j = 0; j < m_NumChannels; j++ )
		{
			if ( token == NULL )
				goto bvh_error;

			m_pMotion[i * m_NumChannels + j] = atof( token );
			token = strtok( NULL, separator );
		}
	}

	// we are done
	file.close();

	// flag that we successful
	isLoaded = true;

	return;

bvh_error:
	file.close();
}



/* ============================================================================
	Summary:
		Given the frame number, render the figure onto the screen

	Paramaters:
	[in] frame_no	:	frame number to display
	[in] scale		:	scale to apply

============================================================================ */
void  BVH::RenderFigure(int frameNum, float scale)
{
	if (m_bRawData == true)
	{
		if (m_Joints.size() > 0)
			RenderFigure( m_Joints[0], m_pMotion + frameNum * m_NumChannels, scale );
	}
	else
	{
		if (m_Joints.size() > 0)
			RenderFigure(m_Joints[0], m_pAnimData + frameNum * m_NumChannels, scale);
	}
}



/* ============================================================================
	Summary:
	Given the frame number, render the figure onto the screen

	Paramaters:
	[in] frame_no	:	frame number to display
	[in] scale		:	scale to apply
	[in] time		:	time since start
============================================================================ */
void  BVH::RenderFigure(int frameNum, int nextFrame, float scale, float time)
{
	if (m_bRawData == true)
	{
		if (m_Joints.size())
		{
			RenderFigureInterp(m_Joints[0],	m_pMotion + frameNum * m_NumChannels,
								m_pMotion + nextFrame * m_NumChannels,
								scale, time);
		}
	}
	else
	{
		if (m_Joints.size())
		{
			RenderFigureInterp(m_Joints[0],
				m_pAnimData + frameNum * m_NumChannels,
				m_pAnimData + nextFrame * m_NumChannels,
				scale, time);
		}
	}
}



/* ============================================================================
	Summary:
	Given the frame number, render the figure onto the screen

	Paramaters:
	[in] joint*		:	pointer to a specific joint to render
	[in] motion*	:	motion data to render
	[in] scale		:	scale to apply


============================================================================ */
void  BVH::RenderFigure(const Joint* joint, const double * data, float scale)
{
	GLfloat modelMatrix[16];

	glPushMatrix();

	// ROOT ?
	if ( joint->parent == NULL )
	{
		// This translates the root ...
		glTranslatef(m_Pos.x, m_Pos.y, m_Pos.z);
		glRotatef(m_Dir.y, 0.0f, 1.0f, 0.0f);

		if (m_bUseTranslation == true)
			glTranslatef((float)data[0] * scale, (float)data[1] * scale, (float)data[2] * scale);
		else
			glTranslatef(0.0f, (float)data[1] * scale, 0.0f);

		glGetFloatv(GL_MODELVIEW_MATRIX, modelMatrix);
		m_HipPosition = glm::vec3(modelMatrix[12], modelMatrix[13], modelMatrix[14]);
		m_BoneMap[joint->name] = m_HipPosition;
	}
	else
	{
		glTranslatef((float)joint->offset[0] * scale, (float)joint->offset[1] * scale, (float)joint->offset[2] * scale );
	}


	size_t  i;
	for ( i = 0; i < joint->channels.size(); i++ )
	{
		Channel* channel = joint->channels[i];

		if ( channel->type == X_ROTATION )
			glRotatef((float)data[ channel->index ], 1.0f, 0.0f, 0.0f );
		else if ( channel->type == Y_ROTATION )
			glRotatef((float)data[ channel->index ], 0.0f, 1.0f, 0.0f );
		else if ( channel->type == Z_ROTATION )
			glRotatef((float)data[ channel->index ], 0.0f, 0.0f, 1.0f );
	}


	// retrieve the modelMatrix and get translation of the joint position
	glGetFloatv(GL_MODELVIEW_MATRIX, modelMatrix);
	glm::vec3 pt = glm::vec3(modelMatrix[12], modelMatrix[13], modelMatrix[14]);
	m_BoneMap[joint->name] = pt;



	// if there are no children, just render the bone to the joint site
	if ( joint->children.size() == 0 )
	{
		RenderBone(joint, 0.0f, 0.0f, 0.0f,
			(float)joint->site[0] * scale, 
			(float)joint->site[1] * scale, 
			(float)joint->site[2] * scale );
	}


	// if there is only one child, render the bone to the offset of the next child
	if ( joint->children.size() == 1 )
	{
		Joint* child = joint->children[0];
		RenderBone(joint, 0.0f, 0.0f, 0.0f,
			(float)child->offset[0] * scale, 
			(float)child->offset[1] * scale, 
			(float)child->offset[2] * scale );
	}


	if ( joint->children.size() > 1 )
	{
		float  center[3] = { 0.0f, 0.0f, 0.0f };

		for ( i = 0; i < joint->children.size(); i++ )
		{
			Joint* child = joint->children[i];

			center[0] += (float)child->offset[0];
			center[1] += (float)child->offset[1];
			center[2] += (float)child->offset[2];
		}

		center[0] /= joint->children.size() + 1;
		center[1] /= joint->children.size() + 1;
		center[2] /= joint->children.size() + 1;

		RenderBone(joint, 0.0f, 0.0f, 0.0f,
			center[0] * scale, 
			center[1] * scale, 
			center[2] * scale );



		for ( i = 0; i < joint->children.size(); i++ )
		{
			Joint* child = joint->children[i];

			RenderBone(joint, center[0] * scale,
					center[1] * scale, 
					center[2] * scale,
					(float)child->offset[0] * scale, 
					(float)child->offset[1] * scale, 
					(float)child->offset[2] * scale );
		}
	}


	for ( i = 0; i < joint->children.size(); i++ )
	{
		RenderFigure( joint->children[i], data, scale );
	}
	glPopMatrix();
}




/* ============================================================================
	Summary:
	Render the bone using a cyclinder.

	Parameters
	[in] x0 :	starting x-coodinate
	[in] y0 :	starting y-coodinate
	[in] z0 :	starting z-coodinate
	[in] x1 :	ending x-coodinate
	[in] y1 :	ending y-coodinate
	[in] z1 :	ending z-coodinate


============================================================================ */
void BVH::RenderBone(const Joint* joint, float x0, float y0, float z0, float x1, float y1, float z1 )
{
	static GLUquadricObj*  quad_obj = NULL;


	// calculate the directional vector of the bone
	GLdouble  dir_x = x1 - x0;
	GLdouble  dir_y = y1 - y0;
	GLdouble  dir_z = z1 - z0;
	GLdouble  bone_length = sqrt( dir_x*dir_x + dir_y*dir_y + dir_z*dir_z );

	glLineWidth(2.0);
	
	if (joint->timer > 0)
		glColor3f(joint->colour[0], joint->colour[1], joint->colour[2]);
	else
		glColor3f(joint->baseColour[0], joint->baseColour[1], joint->baseColour[2]);

#if false
	glBegin(GL_LINES);
	glVertex3f(x0, y0, z0);
	glVertex3f(x1, y1, z1);
	glEnd();
#endif

	// draw a capsule ...
#if true
	if (quad_obj == NULL)
	{
		// create a new quadric primative
		quad_obj = gluNewQuadric();
	}


	gluQuadricDrawStyle( quad_obj, GLU_FILL );
	gluQuadricNormals( quad_obj, GLU_SMOOTH );

	glPushMatrix();

	// translate the position
	glTranslated( x0, y0, z0 );

	// normalize the directional vector
	double  length;
	
	length = bone_length;
	if ( length < 0.0001 ) 
	{ 
		dir_x = 0.0; 
		dir_y = 0.0; 
		dir_z = 1.0;  
		length = 1.0;
	}

	dir_x /= length;  
	dir_y /= length;  
	dir_z /= length;

	// create a standard up vector
	GLdouble  up_x, up_y, up_z;
	up_x = 0.0;
	up_y = 1.0;
	up_z = 0.0;

	// use cross-product to create a side vector
	double  side_x, side_y, side_z;
	side_x = up_y * dir_z - up_z * dir_y;
	side_y = up_z * dir_x - up_x * dir_z;
	side_z = up_x * dir_y - up_y * dir_x;

	// normalize the side vector
	length = sqrt( side_x*side_x + side_y*side_y + side_z*side_z );
	
	if ( length < 0.0001 ) 
	{
		side_x = 1.0; 
		side_y = 0.0; 
		side_z = 0.0;  
		length = 1.0;
	}
	
	side_x /= length;  
	side_y /= length;  
	side_z /= length;


	// use cross product to create up up vector
	up_x = dir_y * side_z - dir_z * side_y;
	up_y = dir_z * side_x - dir_x * side_z;
	up_z = dir_x * side_y - dir_y * side_x;

	// create orientation 4x4 matrix
	GLdouble  m[16] = { side_x, side_y, side_z, 0.0,
	                    up_x,   up_y,   up_z,   0.0,
	                    dir_x,  dir_y,  dir_z,  0.0,
	                    0.0,    0.0,    0.0,    1.0 };
	glMultMatrixd( m );

	// create a cylinder to draw
	GLdouble	radius= 0.05;		// radius of cylinder
	GLint		slices = 8;			// number of slices, more means smoother
	GLint		stack = 2;			// number of vertical slices for the cyclinder (doesn't matter really)

	// Draw the cyclinder
	gluCylinder( quad_obj, radius, radius, bone_length, slices, stack ); 




	glPopMatrix();
#endif
}


void BVH::RenderBones()
{
	BoneMap::iterator it;
	for (it = m_BoneMap.begin(); it != m_BoneMap.end(); it++)
	{
		glm::vec3 pt = it->second;
		
		glColor3f(1.0f, 0.0f, 0.0f);
		glPushMatrix();
		glLoadIdentity();

		glTranslatef(pt.x, pt.y, pt.z);

		static GLUquadric*  pQuad = NULL;
		if (pQuad == NULL)
		{
			// create a new quadric primative
			pQuad = gluNewQuadric();
		}

		gluQuadricDrawStyle(pQuad, GLU_LINE);
		gluQuadricNormals(pQuad, GLU_FLAT);

		gluSphere(pQuad, 0.1, 10, 3);
		glPopMatrix();
	}
}




/* ----------------------------------------------------------------------------
	Summary:
	Get the world bone position and return back to caller.  Given the
	name of the bone, search the bonemap and set the world position
	in the passed pointer.

	Parameter:
	[in] pt : pointer to point to hold the position
	[in] name : the name of the bone to retrieve

	Output:
	true	:	found bone and returned position
	false	:	bone doesn't exist.

---------------------------------------------------------------------------- */
bool BVH::GetBonePos(string name, glm::vec3* pt)
{
	BoneMap::const_iterator it = m_BoneMap.find(name);

	if (it != m_BoneMap.end())
	{
		pt->x = it->second.x;
		pt->y = it->second.y;
		pt->z = it->second.z;
		
		return true;
	}

	cout << "WARNING: unable to find bone - " << name << endl;
	return false;
}




/* ----------------------------------------------------------------------------
	Summary:
	Setup the base colour of the skeleton


---------------------------------------------------------------------------- */
void BVH::SetBonesBaseColour(float r, float g, float b)
{
	JointIndexMap::const_iterator it;
	for (it = m_JointIndexMap.begin(); it != m_JointIndexMap.end(); it++)
	{
		Joint* pJoint = it->second;

		pJoint->baseColour[0] = r;
		pJoint->baseColour[1] = g;
		pJoint->baseColour[2] = b;
	}
}



/* ----------------------------------------------------------------------------
	Summary:
	setup a specific colour for a specific bone, and a timer to keep that
	bone that colour.

---------------------------------------------------------------------------- */
void BVH::SetBoneColour(string jointName, float time, float r, float g, float b)
{
	JointIndexMap::const_iterator it = m_JointIndexMap.find(jointName);
	if (it != m_JointIndexMap.end())
	{
		it->second->colour[0] = r;
		it->second->colour[1] = g;
		it->second->colour[2] = b;

		it->second->timer = time;
	}
}


/* ----------------------------------------------------------------------------
	Summary:
	A quick and dirty function that strips out a lot of the extra frames
	from the BVH file and creates a new motion stream to play back.

---------------------------------------------------------------------------- */
void BVH::StripBVHFile(float amount)
{
	const float amt = 10;
	float totalTime = static_cast<float>(m_Interval * m_NumFrames);			// total time in seconds


	if (m_NumFrames > 200)
	{
		int numFrames = static_cast<int>(m_NumFrames / amt);
		int bufferSize = numFrames * m_NumChannels * 2;

		m_pAnimData = new double[bufferSize];
		m_NumAnimFrames = numFrames;
		m_AnimInterval = m_Interval * amt;

		// loop through all of the frames
		int frameIndex = 0;

		for (int i = 0; i < m_NumFrames; i += 100)
		{
			for (int j = 0; j < m_NumChannels; j++)
			{
				int index = frameIndex * m_NumChannels + j;

				// grab from raw buffer
				double data = m_pMotion[i * m_NumChannels + j];

				// save in new buffer
				m_pAnimData[index] = data;
			}
			frameIndex++;
		}
	}
}



/* ============================================================================
	Summary:
	Linear interpolate between a and b

	Paramaters:
	[in] time		:	value between 0 - 1
	[in] a			:	starting t3Point
	[in] b			:	ending t3Point

	Return:
	result of interpolation

============================================================================ */
glm::vec3 BVH::Lerp(float time, glm::vec3 a, glm::vec3 b)
{
	glm::vec3 p0 = a * (1.0f - time);
	glm::vec3 p1 = b * time;

	p0.x += p1.x;
	p0.y += p1.y;
	p0.z += p1.z;

	return p0;
}



/* ============================================================================
	Summary:
	Linear interpolate between a and b
	
	Paramaters:
	[in] time		:	value between 0 - 1
	[in] a			:	starting float
	[in] b			:	ending float

	Return:
	result of interpolation

============================================================================ */
float BVH::Lerp(float time, float a, float b)
{
	float p0 = a * (1.0f - time);
	float p1 = b * time;

	p0 += p1;

	return p0;
}




/* ============================================================================
	Summary:
	Given the frame number, render the figure onto the screen

	Paramaters:
	[in] joint*		:	pointer to a specific joint to render
	[in] data*		:	pointer to starting key in animation
	[in] next*		:	pointer to next key in animation
	[in] scale		:	scale to apply
	[in] time		:	unit value between 0.0 and 1.0 to calculate key


============================================================================ */
void  BVH::RenderFigureInterp(const Joint* joint, const double* data, const double* next, float scale, float time)
{
	GLfloat modelMatrix[16];

	glPushMatrix();

	// ROOT ?
	if (joint->parent == NULL)
	{
		// This translates the root ...
		glTranslatef(m_Pos.x, m_Pos.y, m_Pos.z);
		glRotatef(m_Dir.y, 0.0f, 1.0f, 0.0f);


		if (m_bUseTranslation == true)
		{
			glm::vec3 startKey = glm::vec3((float)data[0] * scale,
				(float)data[1] * scale, 
				(float)data[2] * scale);

			glm::vec3 endKey = glm::vec3((float)next[0] * scale,
				(float)next[1] * scale, 
				(float)next[2] * scale);

			glm::vec3 interp = Lerp(time, startKey, endKey);

			glTranslatef(interp.x, interp.y, interp.z);
		}
		else
		{
			glm::vec3 startKey = glm::vec3((float)data[0] * scale,
				(float)data[1] * scale,
				(float)data[2] * scale);

			glm::vec3 endKey = glm::vec3((float)next[0] * scale,
				(float)next[1] * scale,
				(float)next[2] * scale);

			glm::vec3 interp = Lerp(time, startKey, endKey);
			glTranslatef(0.0f, interp.y, 0.0f);
		}

		// cache off the world coordinate of this bone ...
		glGetFloatv(GL_MODELVIEW_MATRIX, modelMatrix);
		m_HipPosition = glm::vec3(modelMatrix[12], modelMatrix[13], modelMatrix[14]);
		m_BoneMap[joint->name] = m_HipPosition;
	}
	else
	{
		glTranslatef((float)joint->offset[0] * scale, 
			(float)joint->offset[1] * scale, 
			(float)joint->offset[2] * scale);
	}


	size_t  i;
	for (i = 0; i < joint->channels.size(); i++)
	{
		Channel* channel = joint->channels[i];

		if (channel->type == X_ROTATION)
		{
			float start = (float)data[channel->index];
			float end = (float)data[channel->index];
			float interp = Lerp(time, start, end);

			glRotatef(interp, 1.0f, 0.0f, 0.0f);

		}
		else if (channel->type == Y_ROTATION)
		{
			float start = (float)data[channel->index];
			float end = (float)data[channel->index];
			float interp = Lerp(time, start, end);

			glRotatef(interp, 0.0f, 1.0f, 0.0f);
		}
		else if (channel->type == Z_ROTATION)
		{
			float start = (float)data[channel->index];
			float end = (float)data[channel->index];
			float interp = Lerp(time, start, end);

			glRotatef(interp, 0.0f, 0.0f, 1.0f);
		}
	}


	// retrieve the modelMatrix and get translation of the joint position
	glGetFloatv(GL_MODELVIEW_MATRIX, modelMatrix);
	glm::vec3 pt = glm::vec3(modelMatrix[12], modelMatrix[13], modelMatrix[14]);
	m_BoneMap[joint->name] = pt;



	// if there are no children, just render the bone to the joint site
	if (joint->children.size() == 0)
	{
		RenderBone(joint, 0.0f, 0.0f, 0.0f,
			(float)joint->site[0] * scale,
			(float)joint->site[1] * scale,
			(float)joint->site[2] * scale);
	}


	// if there is only one child, render the bone to the offset of the next child
	if (joint->children.size() == 1)
	{
		Joint* child = joint->children[0];
		RenderBone(joint, 0.0f, 0.0f, 0.0f,
			(float)child->offset[0] * scale,
			(float)child->offset[1] * scale,
			(float)child->offset[2] * scale);
	}


	if (joint->children.size() > 1)
	{
		float  center[3] = { 0.0f, 0.0f, 0.0f };

		for (i = 0; i < joint->children.size(); i++)
		{
			Joint* child = joint->children[i];

			center[0] += (float)child->offset[0];
			center[1] += (float)child->offset[1];
			center[2] += (float)child->offset[2];
		}

		center[0] /= joint->children.size() + 1;
		center[1] /= joint->children.size() + 1;
		center[2] /= joint->children.size() + 1;

		RenderBone(joint, 0.0f, 0.0f, 0.0f,
			center[0] * scale,
			center[1] * scale,
			center[2] * scale);



		for (i = 0; i < joint->children.size(); i++)
		{
			Joint* child = joint->children[i];

			RenderBone(joint, center[0] * scale,
				center[1] * scale,
				center[2] * scale,
				(float)child->offset[0] * scale,
				(float)child->offset[1] * scale,
				(float)child->offset[2] * scale);
		}
	}


	for (i = 0; i < joint->children.size(); i++)
	{
		RenderFigure(joint->children[i], data, scale);
	}
	glPopMatrix();
}



void BVH::UpdateTimers(float dTime)
{
	JointIndexMap::const_iterator it;
	for (it = m_JointIndexMap.begin(); it != m_JointIndexMap.end(); it++)
	{
		Joint* pJoint = it->second;
		if (pJoint && pJoint->timer > 0)
			pJoint->timer -= dTime;
	}
}

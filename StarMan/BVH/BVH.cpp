/**
***  BVH Player
***  Copyright (c) 2004-2017, Masaki OSHITA (www.oshita-lab.org)
***  Released under the MIT license http://opensource.org/licenses/mit-license.php
**/

#include <fstream>
#include <string.h>
#include <math.h>
#include <GL/glut.h>
#include "BVH.h"


BVH::BVH()
{
	motion = NULL;
	Clear();
}


BVH::BVH( const char * bvh_file_name )
{
	motion = NULL;
	Clear();
	Load( bvh_file_name );
	
	world.x = world.y = world.z = 0.0f;
	world.x = 5.0f;
}


BVH::~BVH()
{
	Clear();
}



void  BVH::Clear()
{
	int channelSize = channels.size();
	int jointSize = joints.size();

	int  i;
	for ( i=0; i < channelSize; i++ )
		delete  channels[ i ];


	for ( i=0; i < jointSize; i++ )
		delete  joints[ i ];


	if ( motion != NULL )
		delete  motion;


	isLoaded = false;
	
	file_name = "";
	motion_name = "";
	
	numChannels = 0;
	channels.clear();
	joints.clear();
	joint_index.clear();
	
	numFrames = 0;
	interval = 0.0;
	motion = NULL;
}




/*
	Summary:
	Load a BVH file into the class for rendering.


*/
void  BVH::Load( const char * bvh_file_name )
{
	#define  BUFFER_LENGTH  1024*32					// 32K line buffer

	ifstream			file;
	char				line[ BUFFER_LENGTH ];
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

	// open the file to parse it ...
	file.open( bvh_file_name, ios::in );
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
			new_joint->index = joints.size();
			new_joint->parent = joint;
			new_joint->has_site = false;
			new_joint->offset[0] = 0.0;  new_joint->offset[1] = 0.0;  new_joint->offset[2] = 0.0;
			new_joint->site[0] = 0.0;  new_joint->site[1] = 0.0;  new_joint->site[2] = 0.0;
			joints.push_back( new_joint );

			// check if there is a parent joint
			if (joint)
			{
				joint->children.push_back(new_joint);
			}

			token = strtok( NULL, "" );
			while ( *token == ' ' )  token++;
			new_joint->name = token;

			joint_index[ new_joint->name ] = new_joint;
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
			// 末端位置に座標値を設定
			{
				joint->offset[0] = x;
				joint->offset[1] = y;
				joint->offset[2] = z;
			}
			continue;
		}



		// 関節のチャンネル情報
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
				channel->index = channels.size();
				channels.push_back(channel);

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

		// Appears we are done once we find MOTION keyword ...
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

	numFrames = atoi( token );


	file.getline( line, BUFFER_LENGTH );
	token = strtok( line, ":" );
	if ( strcmp( token, "Frame Time" ) != 0 )  
		goto bvh_error;
	token = strtok( NULL, separator );
	if ( token == NULL )  
		goto bvh_error;
	interval = atof( token );

	// save off the number of channels ...
	numChannels = channels.size();

	// create a motion array to accomodate the number 
	// of frames and the number of channels.
	motion = new double[ numFrames * numChannels ];

	// loop through all of the frames
	for (i = 0; i < numFrames; i++)
	{
		file.getline( line, BUFFER_LENGTH );
		token = strtok( line, separator );
		for (j = 0; j < numChannels; j++ )
		{
			if ( token == NULL )
				goto bvh_error;

			motion[i * numChannels + j] = atof( token );
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



void BVH::RenderFigure(t3Point pos, int frameNum, float scale)
{
	world = pos;
	RenderFigure(frameNum, scale);
}


void BVH::RenderBindFigure(t3Point pos, int frameNum, float scale)
{
	RenderBindPose(joints[0], motion + frameNum * numChannels, scale);

}


/* ============================================================================
	Summary:
		Given the frame number, render the figure onto the screen

	Paramaters:
	[in] frame_no	:	frame number to display
	[in] scale		:	scale to apply

============================================================================ */
void  BVH::RenderFigure( int frame_no, float scale )
{
	RenderFigure( joints[0], motion + frame_no * numChannels, scale );
}



void BVH::RenderBindPose(const Joint* pJoint, const double* data, float scale)
{
	glPushMatrix();


	// check if this is the root joint of the skeleton
	if (pJoint->parent == NULL)
	{
		// This translates the root ...
		glTranslatef(world.x, world.y, world.z);
		glTranslatef((float)data[0] * scale, (float)data[1] * scale, (float)data[2] * scale);

	}
	else
	{
		glTranslatef((float)pJoint->offset[0] * scale, (float)pJoint->offset[1] * scale, (float)pJoint->offset[2] * scale);
	}


	size_t  i;

	for (i = 0; i < pJoint->channels.size(); i++)
	{
		Channel* channel = pJoint->channels[i];

		if (channel->type == X_ROTATION)
			glRotatef(0.0f, 1.0f, 0.0f, 0.0f);
		else if (channel->type == Y_ROTATION)
			glRotatef(0.0f, 0.0f, 1.0f, 0.0f);
		else if (channel->type == Z_ROTATION)
			glRotatef(0.0f, 0.0f, 0.0f, 1.0f);
	}



	if (pJoint->children.size() == 0)
	{
		RenderBone(0.0f, 0.0f, 0.0f, 
			(float)pJoint->site[0] * scale, 
			(float)pJoint->site[1] * scale,
			(float)pJoint->site[2] * scale);
	}



	if (pJoint->children.size() == 1)
	{
		Joint* child = pJoint->children[0];
		RenderBone(0.0f, 0.0f, 0.0f, 
			(float)child->offset[0] * scale, 
			(float)child->offset[1] * scale, 
			(float)child->offset[2] * scale);
	}



	if (pJoint->children.size() > 1)
	{
		float  center[3] = { 0.0f, 0.0f, 0.0f };

		for (i = 0; i < pJoint->children.size(); i++)
		{
			Joint* child = pJoint->children[i];

			center[0] += (float)child->offset[0];
			center[1] += (float)child->offset[1];
			center[2] += (float)child->offset[2];
		}

		center[0] /= pJoint->children.size() + 1;
		center[1] /= pJoint->children.size() + 1;
		center[2] /= pJoint->children.size() + 1;

		RenderBone(0.0f, 0.0f, 0.0f, 
			center[0] * scale, 
			center[1] * scale, 
			center[2] * scale);



		for (i = 0; i < pJoint->children.size(); i++)
		{
			Joint* child = pJoint->children[i];
			RenderBone(center[0] * scale,
						center[1] * scale,
						center[2] * scale,
						(float)child->offset[0] * scale,
						(float)child->offset[1] * scale,
						(float)child->offset[2] * scale);
		}
	}

	for (i = 0; i < pJoint->children.size(); i++)
	{
		RenderFigure(pJoint->children[i], data, scale);
	}
	glPopMatrix();
}


/* ============================================================================
	Summary:
	Given the frame number, render the figure onto the screen

	Paramaters:
	[in] joint*		:	pointer to a specific joint to render
	[in] motion*	:	motion data to render
	[in] scale		:	scale to apply


============================================================================ */
void  BVH::RenderFigure( const Joint * joint, const double * data, float scale )
{
	glPushMatrix();


	// check if this is the root joint of the skeleton
	if ( joint->parent == NULL )
	{

		// This translates the root ...
		glTranslatef(world.x, world.y, world.z);
		glTranslatef( (float)data[ 0 ] * scale, (float)data[ 1 ] * scale, (float)data[ 2 ] * scale );

	}
	else
	{
		glTranslatef((float)joint->offset[ 0 ] * scale, (float)joint->offset[ 1 ] * scale, (float)joint->offset[ 2 ] * scale );
	}


	size_t  i;
	for ( i = 0; i < joint->channels.size(); i++ )
	{
		Channel* channel = joint->channels[ i ];

		if ( channel->type == X_ROTATION )
			glRotatef((float)data[ channel->index ], 1.0f, 0.0f, 0.0f );
		else if ( channel->type == Y_ROTATION )
			glRotatef((float)data[ channel->index ], 0.0f, 1.0f, 0.0f );
		else if ( channel->type == Z_ROTATION )
			glRotatef((float)data[ channel->index ], 0.0f, 0.0f, 1.0f );
	}



	if ( joint->children.size() == 0 )
	{
		RenderBone( 0.0f, 0.0f, 0.0f, (float)joint->site[ 0 ] * scale, (float)joint->site[ 1 ] * scale, (float)joint->site[ 2 ] * scale );
	}



	if ( joint->children.size() == 1 )
	{
		Joint* child = joint->children[0];
		RenderBone( 0.0f, 0.0f, 0.0f, (float)child->offset[ 0 ] * scale, (float)child->offset[ 1 ] * scale, (float)child->offset[ 2 ] * scale );
	}



	if ( joint->children.size() > 1 )
	{
		float  center[ 3 ] = { 0.0f, 0.0f, 0.0f };

		for ( i = 0; i < joint->children.size(); i++ )
		{
			Joint* child = joint->children[ i ];

			center[ 0 ] += (float)child->offset[ 0 ];
			center[ 1 ] += (float)child->offset[ 1 ];
			center[ 2 ] += (float)child->offset[ 2 ];
		}

		center[ 0 ] /= joint->children.size() + 1;
		center[ 1 ] /= joint->children.size() + 1;
		center[ 2 ] /= joint->children.size() + 1;

		RenderBone(	0.0f, 0.0f, 0.0f, center[ 0 ] * scale, center[ 1 ] * scale, center[ 2 ] * scale );



		for ( i = 0; i < joint->children.size(); i++ )
		{
			Joint* child = joint->children[ i ];
			RenderBone(	center[ 0 ] * scale, 
				center[ 1 ] * scale, 
				center[ 2 ] * scale,
				(float)child->offset[ 0 ] * scale, 
				(float)child->offset[ 1 ] * scale, 
				(float)child->offset[ 2 ] * scale );
		}
	}

	for ( i = 0; i < joint->children.size(); i++ )
	{
		RenderFigure( joint->children[ i ], data, scale );
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
void BVH::RenderBone( float x0, float y0, float z0, float x1, float y1, float z1 )
{
	static GLUquadricObj*  quad_obj = NULL;


	// calculate the directional vector of the bone
	GLdouble  dir_x = x1 - x0;
	GLdouble  dir_y = y1 - y0;
	GLdouble  dir_z = z1 - z0;
	GLdouble  bone_length = sqrt( dir_x*dir_x + dir_y*dir_y + dir_z*dir_z );

	glLineWidth(2.0);
//	glColor3f(1.0f, 0, 0);
	glBegin(GL_LINES);
	glVertex3f(x0, y0, z0);
	glVertex3f(x1, y1, z1);
	glEnd();


	
#if 0
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

	// create openGL 4x4 rotation matrix
	GLdouble  m[16] = { side_x, side_y, side_z, 0.0,
	                    up_x,   up_y,   up_z,   0.0,
	                    dir_x,  dir_y,  dir_z,  0.0,
	                    0.0,    0.0,    0.0,    1.0 };
	glMultMatrixd( m );

	// create a cylinder to draw
	GLdouble	radius= 0.02;		// radius of cylinder
	GLint		slices = 8;			// number of slices, more means smoother
	GLint		stack = 2;			// number of vertical slices for the cyclinder (doesn't matter really)

	// Draw the cyclinder
	gluCylinder( quad_obj, radius, radius, bone_length, slices, stack ); 




	glPopMatrix();
#endif
}



#pragma once
#include <string>
#include <iostream>
#include <assert.h>
#include <vector>

#include "3Dutil.h"
#include "glm\vec3.hpp"

// short-cut to get the Collider System class
#define ColliderSys	CColliderSystem::Instance()

typedef struct __Line__
{
	t3Point		p0;
	t3Point		p1;
} Line;

typedef struct __ResultDistance__
{
	float*	pA;
	float*	pB;
	float	d;
}tClosestLineResult;



typedef struct __BonePosDef__
{
	glm::vec3		a;
	glm::vec3		b;

	__BonePosDef__(glm::vec3 i0, glm::vec3 i1)
	{
		a = i0;
		b = i1;
	}
} tBonePosDef;


typedef struct __Result__
{
	glm::vec3	p0;
	glm::vec3	p1;
	float		length;
} tClosestLine;

/* ----------------------------------------------------------------------------
	Summary:
	A simple collider class that check colliders against other colliders,
	which in this case is simply just line segments with a radius.

	The update loop which will check entities against one another
	for collisions.  The init function prepares which bones can collide with
	what other bones.

	This function works with the cached BVH bone coordinates that can be used
	to calculate the bone segments and check for the collisions.


	Notes:
	I didn't have time to write a clean collision system, so this code is 
	ugly and is drawn from the following websites to get something working.


	Reference:
	http://geomalgorithms.com/a07-_distance.html
	https://stackoverflow.com/questions/2824478/shortest-distance-between-two-line-segments
	https://www.gamedev.net/forums/topic/288963-swept-capsule-capsule-intersection-3d/

---------------------------------------------------------------------------- */
class CColliderSystem
{
public:
	typedef struct __Bone_Segment__
	{
		std::string		startBone;
		std::string		endBone;
	} BONE_SEGMENT;

private:
	typedef std::vector<BONE_SEGMENT*> ColliderList;

public:
	float		Determinate3(float* a, float* v1, float* v2);
	float*		cross3(float* v1, float* v2, float* output);
	float		dot3(float* v1, float* v2);
	float		norma3(float* v1);
	float*		multiplica3(float* v1, float v, float* v2);
	float*		soma3(float* v1, float* v2, int sinal, float* v);
	
	float		Determinate3(const glm::vec3& a, const glm::vec3& v1, const glm::vec3& v2);

	void closestDistanceBetweenLines(float* a0, float* a1, 
										float* b0, float* b1, 
										int clampAll, 
										int clampA0, 
										int clampA1, 
										int clampB0, 
										int clampB1, 
										tClosestLineResult* rd);

	void closestDistanceBetweenLines(const glm::vec3& a0, const glm::vec3& a1,
										const glm::vec3& b0, const glm::vec3& b1,
										int clampAll,
										int clampA0,
										int clampA1,
										int clampB0,
										int clampB1,
		tClosestLine* rd);

public:
	CColliderSystem();
	~CColliderSystem();

	ColliderList	m_From;
	ColliderList	m_To;

	void			Init();
	void			Update();
	void			Test();


	static CColliderSystem* Instance();
};


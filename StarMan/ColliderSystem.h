#pragma once
#include <string>
#include <iostream>
#include <assert.h>
#include <vector>

#include "3Dutil.h"
#include "glm\vec3.hpp"

// short-cut to get the Collider System class
#define ColliderSys	CColliderSystem::Instance()

typedef struct __BonePosDef__
{
	glm::vec3		a;					// starting position
	glm::vec3		b;					// ending position

	__BonePosDef__()
	{
		a = glm::vec3(0.0f, 0.0f, 0.0f);
		b = glm::vec3(0.0f, 0.0f, 0.0f);
	}

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

	__Result__()
	{
		length = 0.0f;
		p0 = glm::vec3(0.0f);
		p1 = glm::vec3(0.0f);
	}

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
	float		Determinate3(const glm::vec3& a, const glm::vec3& v1, const glm::vec3& v2);

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


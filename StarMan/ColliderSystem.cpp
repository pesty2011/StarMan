#include <string>
#include <iostream>
#include <assert.h>
#include <math.h>

#include "glm\glm.hpp"
#include "Entities\EntityManager.h"
#include "EntityNames.h"
#include "EntityMessage.h"
#include "MessageDispatch.h"
#include "BaseEntity.h"
#include "ColliderSystem.h"



/* ----------------------------------------------------------------------------
	Summary:
	returns an instance of the collider system

---------------------------------------------------------------------------- */
CColliderSystem* CColliderSystem::Instance()
{
	static CColliderSystem instance;
	return &instance;
}




CColliderSystem::CColliderSystem()
{
#if _DEBUG
	Test();
#endif


	Init();
}




CColliderSystem::~CColliderSystem()
{
	for (size_t i = 0; i < m_From.size(); i++)
	{
		delete m_From[i];
	}

	for (size_t i = 0; i < m_To.size(); i++)
	{
		delete m_To[i];
	}
	m_From.clear();
	m_To.clear();
}



/* ----------------------------------------------------------------------------
	Summary:
	Create all of the colliders for the arms, legs, and chest for the skeleton
	definition.

---------------------------------------------------------------------------- */
void CColliderSystem::Init()
{
	BONE_SEGMENT*	seg;

	// here are all from colliders
	seg = new BONE_SEGMENT;
	seg->startBone = "RightUpArm";
	seg->endBone = "RightLowArm";
	m_From.push_back(seg);

	seg = new BONE_SEGMENT;
	seg->startBone = "RightLowArm";
	seg->endBone = "RightHand";
	m_From.push_back(seg);

	seg = new BONE_SEGMENT;
	seg->startBone = "LeftUpArm";
	seg->endBone = "LeftLowArm";
	m_From.push_back(seg);

	seg = new BONE_SEGMENT;
	seg->startBone = "LeftLowArm";
	seg->endBone = "LeftHand";
	m_From.push_back(seg);

	seg = new BONE_SEGMENT;
	seg->startBone = "RightUpLeg";
	seg->endBone = "RightLowLeg";
	m_From.push_back(seg);

	seg = new BONE_SEGMENT;
	seg->startBone = "RightLowLeg";
	seg->endBone = "RightFoot";
	m_From.push_back(seg);

	seg = new BONE_SEGMENT;
	seg->startBone = "LeftUpLeg";
	seg->endBone = "LeftLowLeg";
	m_From.push_back(seg);

	seg = new BONE_SEGMENT;
	seg->startBone = "LeftLowLeg";
	seg->endBone = "LeftFoot";
	m_From.push_back(seg);


	// here are all of the To colliders
	seg = new BONE_SEGMENT;
	seg->startBone = "Hips";
	seg->endBone = "Chest";
	m_To.push_back(seg);

	seg = new BONE_SEGMENT;
	seg->startBone = "RightUpLeg";
	seg->endBone = "RightLowLeg";
	m_To.push_back(seg);

	seg = new BONE_SEGMENT;
	seg->startBone = "RightLowLeg";
	seg->endBone = "RightFoot";
	m_To.push_back(seg);

	seg = new BONE_SEGMENT;
	seg->startBone = "LeftUpLeg";
	seg->endBone = "LeftLowLeg";
	m_To.push_back(seg);

	seg = new BONE_SEGMENT;
	seg->startBone = "LeftLowLeg";
	seg->endBone = "LeftFoot";
	m_To.push_back(seg);

	seg = new BONE_SEGMENT;
	seg->startBone = "Chest";
	seg->endBone = "Neck";
	m_To.push_back(seg);

	seg = new BONE_SEGMENT;
	seg->startBone = "Neck";
	seg->endBone = "Head";
	m_To.push_back(seg);
}



/* ----------------------------------------------------------------------------
	Summary:
	This is where collisions take place between entities.  If a collision
	occurs, send a message to the entity.

---------------------------------------------------------------------------- */
void CColliderSystem::Update()
{
	CBaseEntity* from;
	CBaseEntity* to;

	// use the entity manager to get the entities and check them
	// against all possible other entities
	for (int i = 0; i < EntityTotal; i++)
	{
		from = EntityMgr->FindEntity(i);

		for (int j = 0; j < EntityTotal; j++)
		{
			to = EntityMgr->FindEntity(j);

			if (from->GetID() != to->GetID())
			{
				Line l1;
				Line l2;

				// loop through the from list and check against the to list
				// using the bone names, get the world position and create a line 
				// segment and check it against it
				for (size_t fromIndex = 0; fromIndex < m_From.size(); fromIndex++)
				{
					bool status;

					BONE_SEGMENT* fromBones = m_From[fromIndex];

					status = from->GetBonePos(fromBones->startBone, &l1.p0);
					if (status == false)
						continue;

					status = from->GetBonePos(fromBones->endBone, &l1.p1);
					if (status == false)
						continue;



					for (size_t toIndex = 0; toIndex < m_To.size(); toIndex++)
					{
						t3Point toStartBone;
						t3Point toEndBone;

						BONE_SEGMENT* toBones = m_To[toIndex];

						status = to->GetBonePos(toBones->startBone, &l2.p0);
						if (status == false)
							continue;

						status = to->GetBonePos(toBones->endBone, &l2.p1);
						if (status == false)
							continue;


						// success getting bones and line segments
						float dist = 0.0f;

						tClosestLineResult result;
						closestDistanceBetweenLines(&l1.p0.v[0], &l1.p1.v[0], &l2.p0.v[0], &l2.p1.v[0], 1, 0, 0, 0, 0, &result);


#if _DEBUG
						t3Point a, b;
						a = t3Point(result.pA[0], result.pA[1], result.pA[2]);
						b = t3Point(result.pB[0], result.pB[1], result.pB[2]);
#endif


						if (result.d < 0.5f)
						{
#if _DEBUG && 0
							// use the message dispatcher to signal a collision to the enity 
							// that a collision has occurred.
							cout << "Collision Detected" << endl;
							cout << "Attacker: " << fromBones->startBone << endl;
							cout << "Victum: " << toBones->startBone << endl;
							cout << endl;
#endif

							tCollisionEvent info;
							info.BoneName = toBones->startBone;
							info.From = from->GetID();
							info.To = to->GetID();
							Dispatch->DispatchEntityMessage(SEND_MESSAGE_IMMEDIATELY, from->GetID(), to->GetID(), CREATE_MESSAGE(MSGTYPE_COLLISION, MSGEVENT_COLLISION_COLLIDED), 0, (void*)&info);
						}
					}
				}
			}
		}
	}
}



void CColliderSystem::Test()
{
	Line l1;
	Line l2;

	l1.p0 = t3Point(-10.0f, 0, 0);
	l1.p1 = t3Point(10.0f, 0, 0);

	l2.p0 = t3Point(0.0f, 10.0f, 0);
	l2.p1 = t3Point(0.0f, -10.0f, 0);


	tClosestLineResult result;
	closestDistanceBetweenLines(&l1.p0.v[0], &l1.p1.v[0], &l2.p0.v[0], &l2.p1.v[0], 1, 0, 0, 0, 0, &result);

	t3Point a, b;
	a = t3Point(result.pA[0], result.pA[1], result.pA[2]);
	b = t3Point(result.pB[0], result.pB[1], result.pB[2]);

	if (result.d < 1.0f)
	{
		cout << "collision true" << endl;
		cout << a.v[0] << ", " << a.v[1] << ", " << a.v[2] << endl;
		cout << b.v[0] << ", " << b.v[1] << ", " << b.v[2] << endl;
	}


	cout << "TEST #1: intersection be true" << endl;
	tBonePosDef b1 = tBonePosDef(glm::vec3(-10.0f, 0.0f, 0.0f), glm::vec3(10.0f, 0.0f, 0.0f));
	tBonePosDef b2 = tBonePosDef(glm::vec3(0.0f, 10.0f, 0.0f), glm::vec3(0.0f, -10.0f, 0.0f));
	tClosestLine res;
	closestDistanceBetweenLines(b1.a, b1.b, b2.a, b2.b, 1, 0, 0, 0, 0, &res);

	if (res.length < 1.0f)
	{
		cout << "Closest points: " << endl;
		cout << res.p0.x << ", " << res.p0.y << ", " << res.p0.z << endl;
		cout << res.p1.x << ", " << res.p1.y << ", " << res.p1.z << endl;
	}

	cout << "TEST #2: intersection false, but within bounding radius" << endl;
	b1 = tBonePosDef(glm::vec3(-10.0f, 0.5f, 0.0f), glm::vec3(10.0f, 0.5f, 0.0f));
	b2 = tBonePosDef(glm::vec3(0.0f, 10.0f, 0.0f), glm::vec3(0.0f, -10.0f, 0.0f));
	closestDistanceBetweenLines(b1.a, b1.b, b2.a, b2.b, 1, 0, 0, 0, 0, &res);

	if (res.length < 1.0f)
	{
		cout << "Closest points: " << endl;
		cout << res.p0.x << ", " << res.p0.y << ", " << res.p0.z << endl;
		cout << res.p1.x << ", " << res.p1.y << ", " << res.p1.z << endl;
	}

}









/*
	Summery:
	Calculate the determinate

	a	:	pointer to a float3
	v1	:	pointer to a float3
	v2	:	pointer to a float3

*/
float	CColliderSystem::Determinate3(float* a, float* v1, float* v2)
{
	return a[0] * (v1[1] * v2[2] - v1[2] * v2[1]) + 
		   a[1] * (v1[2] * v2[0] - v1[0] * v2[2]) + 
		   a[2] * (v1[0] * v2[1] - v1[1] * v2[0]);
}



/*
	Summary:
	calculate the cross product between the 2 vectors

*/
float*	CColliderSystem::cross3(float* v1, float* v2, float* v)
{
//	float* v = (float*)malloc(3 * sizeof(float));
	v[0] = v1[1] * v2[2] - v1[2] * v2[1];
	v[1] = v1[2] * v2[0] - v1[0] * v2[2];
	v[2] = v1[0] * v2[1] - v1[1] * v2[0];
	return v;
}



/*
	Summary:
	Calculate the dot product between the 2 vectors

*/
float	CColliderSystem::dot3(float* v1, float* v2)
{
	return v1[0] * v2[0] + v1[1] * v2[1] + v1[2] * v2[2];
}



float	CColliderSystem::norma3(float* v1)
{
	return sqrtf(dot3(v1, v1));

//	float soma = 0;
//	for (int i = 0; i < 3; i++) {
//		soma += pow(v1[i], 2);
//	}
//	return sqrt(soma);
}



float*	CColliderSystem::multiplica3(float* v1, float v, float* v2)
{
//	float* v2 = (float*)malloc(3 * sizeof(float));
	for (int i = 0; i < 3; i++) {
		v2[i] = v1[i] * v;
	}
	return v2;
}



float*	CColliderSystem::soma3(float* v1, float* v2, int sinal, float* v)
{
	for (int i = 0; i < 3; i++) {
		v[i] = v1[i] + sinal * v2[i];
	}
	return v;
}




void CColliderSystem::closestDistanceBetweenLines(float* a0, float* a1, float* b0, float* b1, 
													int clampAll, 
													int clampA0, 
													int clampA1, 
													int clampB0, 
													int clampB1,
													tClosestLineResult* rd)
{
	float denom, det0, det1, t0, t1, d;

	float *A = NULL;
	float *B = NULL;
	float *_A = NULL;
	float *_B = NULL;
	float *cross = NULL;
	float *t = NULL;
	float *pA = NULL;
	float *pB = NULL;


	float _cross[3];
	float v_A[3];
	float v_B[3];
	float vA[3];
	float vB[3];
	float pvA[3];
	float pvB[3];
	float vT[3];

//	tClosestLineResult *rd = (tClosestLineResult *)malloc(sizeof(tClosestLineResult));

	if (clampAll) {
		clampA0 = 1;
		clampA1 = 1;
		clampB0 = 1;
		clampB1 = 1;
	}

	A = soma3(a1, a0, -1, vA);
	B = soma3(b1, b0, -1, vB);
	_A = multiplica3(A, 1 / norma3(A), v_A);
	_B = multiplica3(B, 1 / norma3(B), v_B);
	cross = cross3(_A, _B, _cross);
	denom = pow(norma3(cross), 2);

	if (denom == 0) {
		float temp1[3];
		float soma1[3];
		float soma2[3];

		float d0 = dot3(_A, soma3(b0, a0, -1, temp1));

		d = norma3(soma3(soma3(multiplica3(_A, d0, temp1), a0, 1, soma1), b0, -1, soma2));
		if (clampA0 || clampA1 || clampB0 || clampB1) {
			double d1 = dot3(_A, soma3(b1, a0, -1, soma1));
			if (d0 <= 0 && 0 >= d1) {
				if (clampA0 && clampB1) {
					if (abs(d0) < abs(d1)) {
						rd->pA = b0;
						rd->pB = a0;
						rd->d = norma3(soma3(b0, a0, -1, soma1));
					}
					else {
						rd->pA = b1;
						rd->pB = a0;
						rd->d = norma3(soma3(b1, a0, -1, soma1));
					}
				}
			}
			else if (d0 >= norma3(A) && norma3(A) <= d1) {
				if (clampA1 && clampB0) {
					if (abs(d0) <abs(d1)) {
						rd->pA = b0;
						rd->pB = a1;
						rd->d = norma3(soma3(b0, a1, -1, soma1));
					}
					else {
						rd->pA = b1;
						rd->pB = a1;
						rd->d = norma3(soma3(b1, a1, -1, soma1));
					}
				}
			}
		}
		else {
			rd->pA = NULL;
			rd->pB = NULL;
			rd->d = d;
		}
	}
	else {
		float temp1[3];
		float temp2[3];
		float da, db;
		t = soma3(b0, a0, -1, vT);
		det0 = Determinate3(t, _B, cross);
		det1 = Determinate3(t, _A, cross);

#if false
		if (true)
		{
			glm::vec3 va = glm::vec3(t[0], t[1], t[2]);
			glm::vec3 vb = glm::vec3(_B[0], _B[1], _B[2]);
			glm::vec3 vc = glm::vec3(cross[0], cross[1], cross[2]);

			da = Determinate3(va, vb, vc);
		}





		if (true)
		{
			glm::vec3 va = glm::vec3(t[0], t[1], t[2]);
			glm::vec3 vb = glm::vec3(_A[0], _A[1], _A[2]);
			glm::vec3 vc = glm::vec3(cross[0], cross[1], cross[2]);

			db = Determinate3(va, vb, vc);
		}


		if (da != det0 || db != det1)
		{
			cout << "we have a difference of determinate" << endl;
		}
#endif


		t0 = det0 / denom;
		t1 = det1 / denom;
		pA = soma3(a0, multiplica3(_A, t0, temp1), 1, pvA);
		pB = soma3(b0, multiplica3(_B, t1, temp2), 1, pvB);

		if (clampA0 || clampA1 || clampB0 || clampB1) {
			if (t0 < 0 && clampA0)
				pA = a0;
			else if (t0 > norma3(A) && clampA1)
				pA = a1;
			if (t1 < 0 && clampB0)
				pB = b0;
			else if (t1 > norma3(B) && clampB1)
				pB = b1;
		}

		d = norma3(soma3(pA, pB, -1, temp1));

		rd->pA = pA;
		rd->pB = pB;
		rd->d = d;
	}
}






// GLM functions
inline float CColliderSystem::Determinate3(const glm::vec3& a, const glm::vec3& v1, const glm::vec3& v2)
{
	glm::mat3 mat = glm::mat3(a, v1, v2);
	return glm::determinant(mat);
}




/* ----------------------------------------------------------------------------
	Summary:
	Given 2 lines with starting points in space, calculate the shortest 
	line segment between those 2 lines and return it back to the caller.

	The function will clamp the shortest distance  between lines, if
	the caller requests it.

	Parameters:
	[in] a0		:		starting position of line a
	[in] a1		:		ending position of line a
	[in] b0		:		starting position of line b
	[in] b1		:		ending position of line b

	[in] clampAll	:	clamp positions to beginning and ending of lines.
	[in] clampA0	:	clamp shortest line to start point of line a
	[in] clampA1	:	clamp shortest line to end point of line a
	[in] clampB0	:	clamp shortest line to starting of line b
	[in] clampB1	:	clamp shortest line to end point of line a

	[in] result*	:	result structure defining the shortest line between
						lines.

---------------------------------------------------------------------------- */
void CColliderSystem::closestDistanceBetweenLines(const glm::vec3& a0, const glm::vec3& a1,
													const glm::vec3& b0, const glm::vec3& b1,
													int clampAll,
													int clampA0,
													int clampA1,
													int clampB0,
													int clampB1,
													tClosestLine* rd)

{
	float denom, det0, det1, d;

	glm::vec3 vA, vB;
	glm::vec3 nA, nB;
	glm::vec3 vCross;



	if (clampAll) 
	{
		clampA0 = 1;
		clampA1 = 1;
		clampB0 = 1;
		clampB1 = 1;
	}



	vA = a1 - a0;							// create the A line vector
	vB = b1 - b0;							// create the B line vector

	float lenA = glm::length(vA);			// get the length of vA
	float lenB = glm::length(vB);			// get the length of vB

	nA = glm::normalize(vA);				// normalize vA
	nB = glm::normalize(vB);				// normalize vB

	vCross = glm::cross(nA, nB);			// get perpendicular vector to A & B
	float lenCross = glm::length(vCross);	// what is the length of this

	denom = pow(lenCross, 2.0f);			// create a denom value ????



	// is length of cross product is 0.0 ....
	if (denom == 0) 
	{
		float d0 = glm::dot(nA, (b0 - a0));

		d = glm::length(((nA * d0) * a0) * b0);


		// are all the clamps in place?
		if (clampA0 || clampA1 || clampB0 || clampB1) 
		{
			float d1 = glm::dot(nA, (b1 - a0));
			if (d0 <= 0.0f && 0.0f >= d1) 
			{
				if (clampA0 && clampB1) 
				{
					if (abs(d0) < abs(d1)) 
					{
						rd->p0 = b0;
						rd->p1 = a0;
						rd->length = glm::length(b0 - a0);
					}
					else {
						rd->p0 = b1;
						rd->p1 = a0;
						rd->length = glm::length(b1 - a0);
					}
				}
			}
			else if (d0 >= lenA && lenA <= d1) 
			{
				if (clampA1 && clampB0) 
				{
					if (abs(d0) <abs(d1)) 
					{
						rd->p0 = b0;
						rd->p1 = a1;
						rd->length = glm::length(b0 - a1);
					}
					else 
					{
						rd->p0 = b1;
						rd->p1 = a1;
						rd->length = glm::length(b1 - a1);
					}
				}
			}
		}
		else 
		{
			rd->p0 = glm::vec3(0.0f, 0.0f, 0.0f);
			rd->p1 = glm::vec3(0.0f, 0.0f, 0.0f);
			rd->length = d;
		}
	}
	else 
	{
		glm::vec3 vS = (b0 - a0);					// vector between starting points of lines.

		det0 = Determinate3(vS, nB, vCross);		// get the determinant between 3 vectors
		det1 = Determinate3(vS, nA, vCross);
		
		// using length of perpendicular vector, calculate scalars for t0, t1
		float t0 = det0 / denom;
		float t1 = det1 / denom;

		glm::vec3 pA = (a0 + (nA * t0));			// closest point on line a
		glm::vec3 pB = (b0 + (nB * t1));			// closest point on line b


		// if we are clamping values, ensure we are within lines
		if (clampA0 || clampA1 || clampB0 || clampB1) 
		{
			if (t0 < 0 && clampA0)
			{
				pA = a0;
			}
			else if (t0 > lenA && clampA1)
			{
				pA = a1;
			}
			if (t1 < 0 && clampB0)
			{
				pB = b0;
			}
			else if (t1 > lenB && clampB1)
			{
				pB = b1;
			}
		}

		rd->p0 = pA;
		rd->p1 = pB;
		rd->length = glm::length(rd->p1 - rd->p0);
	}
}

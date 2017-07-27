#include <string>
#include <iostream>
#include <assert.h>

#include "AssetManager.h"


CAssetManager::CAssetManager()
{
}


CAssetManager::~CAssetManager()
{


}



/* ----------------------------------------------------------------------------
Summary:
returns an instance of the asset manager

---------------------------------------------------------------------------- */
CAssetManager* CAssetManager::Instance()
{
	static CAssetManager instance;
	return &instance;
}



/* ----------------------------------------------------------------------------
	Summary:
	Clear out all of the motion data out of the asset manager

---------------------------------------------------------------------------- */
void CAssetManager::Clear()
{
	// loop through all of the motiondata and delete them from the system
	assets.clear();
}




/* ----------------------------------------------------------------------------
Summary:
search the entity list and return the pointer to the entity back
to the caller.

Parameters:
[in] name : string containing name of the entity

---------------------------------------------------------------------------- */
BVH* CAssetManager::FindMotion(const string& name)
{
	AssetMap::const_iterator ent = assets.find(name);

	assert((ent != assets.end()) && "invalid name");

	return ent->second;
}



/* ----------------------------------------------------------------------------
	Summary:
	add a motion into the asset manager

	Parameters:
	[in] data : pointer to the BVH instance

---------------------------------------------------------------------------- */
bool CAssetManager::AddMotion(BVH* data)
{
	string motionName = data->GetMotionName();
	assets.insert(std::pair<string, BVH*>(motionName, data));

	cout << "AssetMgr added: " << motionName << endl;
	return true;
}


bool CAssetManager::AddMotion(const char* motionName)
{
	BVH* bvh = new BVH(motionName);
	if (bvh != NULL)
	{
		bvh->StripBVHFile(6.0f);			// 6 frames a second
		return AddMotion(bvh);
	}

	cout << "AssetMgr failed to create: " << motionName << endl;
	return false;
}



/* ----------------------------------------------------------------------------
	Summary:
	delete motion data from the manager

	Parameters:
	[in] data : pointer to the BVH motion data

---------------------------------------------------------------------------- */
void CAssetManager::DelMotion(BVH* data)
{
	string name = data->GetMotionName();
	DelMotion(name);
}




/* ----------------------------------------------------------------------------
	Summary:
	delete motion data from the manager

	Parameters:
	[in] name : name of the bvh motion data

---------------------------------------------------------------------------- */
void CAssetManager::DelMotion(const string& name)
{
	assets.erase(name);
}


void CAssetManager::ToggleRawData()
{
	AssetMap::iterator	it;

	for (it = assets.begin(); it != assets.end(); it++)
	{
		it->second->ToggleRawData();
	}
}
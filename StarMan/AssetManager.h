#pragma once
#include <vector>
#include <map>
#include "BVH\BVH.h"


// short-cut to get the entity manager class
#define AssetMgr	CAssetManager::Instance()


class CAssetManager
{
	typedef std::map<string, BVH*> AssetMap;

	// facilitate a quick lookup for motion assets stored in the std::map
	AssetMap assets;

public:
	CAssetManager();
	~CAssetManager();


public:
	BVH* FindMotion(const string& name);
	bool AddMotion(BVH* data);
	bool AddMotion(const char* motionName);
	void DelMotion(BVH* data);
	void DelMotion(const string& name);

	void Clear();


	static CAssetManager* Instance();
};


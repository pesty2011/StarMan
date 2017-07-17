#pragma once

#include <d3d9.h>
#include <d3dx9math.h>



class Terrain
{
	LPDIRECT3DDEVICE9	device;

	struct CUSTOMVERTEX
	{
		float x, y, z;
		DWORD color;
	};


public:
	Terrain(LPDIRECT3DDEVICE9 d3dDevice);
	~Terrain();

	void Init(UINT16 width, UINT16 height, D3DCOLOR color, float size);

};


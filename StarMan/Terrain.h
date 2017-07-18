#pragma once

#include <d3d9.h>
#include <d3dx9math.h>



class CTerrain
{
	LPDIRECT3DDEVICE9	device;

	struct CUSTOMVERTEX
	{
		float x, y, z;
		DWORD color;
	};

	D3DXVECTOR3 vertexGridList[40];					// 10x10 grid

public:
	CTerrain(LPDIRECT3DDEVICE9 d3dDevice);
	~CTerrain();

	void Init(UINT16 width, UINT16 height, D3DCOLOR color, float size);
	void Update(float deltaTime);
	void Render();

protected:
	void CreateGrid();
};


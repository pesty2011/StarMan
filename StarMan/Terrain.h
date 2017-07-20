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

	
public:
	CTerrain(LPDIRECT3DDEVICE9 d3dDevice);
	~CTerrain();

	void Update(float deltaTime);
	void Render();
	void DrawGrid(LPD3DXLINE line);
	void DrawPlus(LPD3DXLINE line);
};


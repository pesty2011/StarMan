#pragma once
#include <d3d9.h>

class CMainCamera
{
	LPDIRECT3DDEVICE9	d3dDevice;

public:
	CMainCamera(LPDIRECT3DDEVICE9 d3dDevice);
	~CMainCamera();
};


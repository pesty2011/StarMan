#pragma once
#include <d3d9.h>

class MainCamera
{
	LPDIRECT3DDEVICE9	d3dDevice;

public:
	MainCamera(LPDIRECT3DDEVICE9 d3dDevice);
	~MainCamera();
};


#pragma once
#include <d3d9.h>
#include <d3dx9.h>


/*


	Reference:
	http://www.chadvernon.com/blog/resources/directx9/moving-around-a-3d-world/

*/
class CMainCamera
{
	LPDIRECT3DDEVICE9	d3dDevice;
	D3DXVECTOR3			lookAt, eye, up;
	D3DXMATRIX			view, proj;

public:
	CMainCamera(LPDIRECT3DDEVICE9 d3dDevice);
	~CMainCamera();

	void Update(float deltaTime);
	void LookAt(D3DVECTOR lookat);

	void CreateProjectionMatrix(UINT16 width, UINT16 height);
};


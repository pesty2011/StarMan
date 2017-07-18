#include "MainCamera.h"



CMainCamera::CMainCamera(LPDIRECT3DDEVICE9 d3dDevice)
{
	this->d3dDevice = d3dDevice;

	up = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	eye = D3DXVECTOR3(0.0f, 4.0f, -8.0f);
	lookAt = D3DXVECTOR3(100.0f, 100.0f, 100.0f);
	D3DXMatrixIdentity(&proj);
	D3DXMatrixIdentity(&view);

	CreateProjectionMatrix(800, 600);
	LookAt(lookAt);

//	Update(0);
}


CMainCamera::~CMainCamera()
{
}


void CMainCamera::Update(float deltaTime)
{
//	d3dDevice->SetTransform(D3DTS_VIEW, &view);
}




/*
	Summary:	
		Create a View Matrix
	Parameters:
		[in] lookat			: position to lookat in world space

	Output:
		[out] view			: view matrix is defined

*/
void CMainCamera::LookAt(D3DVECTOR lookat)
{
	lookAt = lookat;

	// update the view matrix
	D3DXMatrixLookAtLH(&view, &eye, &lookAt, &up);
	d3dDevice->SetTransform(D3DTS_VIEW, &view);
}






/*
	Summary:	Create a projection Matrix
	Parameters:
		[in] width/height	: calculates aspect (width/height)

*/
void CMainCamera::CreateProjectionMatrix(UINT16 width, UINT16 height)
{
	float aspect = ((float)width / (float)height);
	float fov = D3DX_PI / 4;
	D3DXMatrixPerspectiveFovLH(&proj, fov, aspect, 1.0f, 1.0f);
	d3dDevice->SetTransform(D3DTS_PROJECTION, &proj);
}


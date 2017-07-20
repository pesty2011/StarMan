#include "MainCamera.h"

extern int						screenWidth;
extern int						screenHeight;


CMainCamera::CMainCamera(LPDIRECT3DDEVICE9 d3dDevice)
{
	this->d3dDevice = d3dDevice;

	up = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	eye = D3DXVECTOR3(5.0f, 30.0f, 5.0f);
	lookAt = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXMatrixIdentity(&proj);
	D3DXMatrixIdentity(&view);
	D3DXMatrixIdentity(&world);


	CreateProjectionMatrix(screenWidth, screenHeight);
	CreateViewMatrix();
	CreateWorldMatrix();

}


CMainCamera::~CMainCamera()
{
}


void CMainCamera::Update(float deltaTime)
{
//	d3dDevice->SetTransform(D3DTS_VIEW, &view);
}


void CMainCamera::Move(WPARAM key)
{
	const float inc = 5.0f;
	switch (key)
	{
	case VK_LEFT:
		eye.x -= inc;
		break;

	case VK_RIGHT:
		eye.x += inc;
		break;

	case VK_UP:
		eye.y += inc;
		break;

	case VK_DOWN:
		eye.y -= inc;
		break;

	default:
		break;
	}
	CreateViewMatrix();
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
	D3DXMatrixPerspectiveFovLH(&proj, fov, aspect, 0.1f, 2000.0f);
	d3dDevice->SetTransform(D3DTS_PROJECTION, &proj);
}



void CMainCamera::CreateWorldMatrix()
{
	// no rotation, or scaling
	D3DXMatrixIdentity(&world);
	//	D3DXMatrixMultiply(&world, &proj, &view);
	d3dDevice->SetTransform(D3DTS_WORLD, &world);

	CreateViewMatrix();
//	D3DXMatrixMultiply(&world, &proj, &view);

	d3dDevice->SetTransform(D3DTS_PROJECTION, &proj);

}


void CMainCamera::CreateViewMatrix()
{
	D3DXMatrixLookAtLH(&view, &eye, &lookAt, &up);
	d3dDevice->SetTransform(D3DTS_VIEW, &view);
}

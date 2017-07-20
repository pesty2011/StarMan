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
	D3DXMATRIX			view, proj, world;

	float m_pitch, m_yaw;

public:
	CMainCamera(LPDIRECT3DDEVICE9 d3dDevice);
	~CMainCamera();

	void Update(float deltaTime);
	void LookAt(D3DVECTOR lookat);

	void CreateProjectionMatrix(UINT16 width, UINT16 height);
	void CreateWorldMatrix();
	void CreateViewMatrix();

	const D3DXMATRIX*		GetWorldMatrix() { CreateWorldMatrix(); return &world; }
	const D3DXMATRIX*		GetProjectionMatrix() { return &proj; }
	const D3DXMATRIX*		GetViewMatrix() { return &view; }

	void Move(WPARAM key);


};


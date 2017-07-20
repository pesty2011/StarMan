#include "Terrain.h"
#include "MainCamera.h"

extern CMainCamera*		mCamera;




CTerrain::CTerrain(LPDIRECT3DDEVICE9 d3dDevice)
{
	device = d3dDevice;
}


CTerrain::~CTerrain()
{
}




void CTerrain::DrawPlus(LPD3DXLINE line)
{
	D3DXVECTOR3 vertexHorzList[2];
	D3DXVECTOR3 vertexVertList[2];
	const float gs = 500.0f;

	D3DCOLOR lineColor;
	const D3DXMATRIX* world = mCamera->GetWorldMatrix();

	lineColor = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
	vertexHorzList[0] = D3DXVECTOR3(-gs, 0, 0);
	vertexHorzList[1] = D3DXVECTOR3(+gs, 0, 0);
	line->DrawTransform(vertexHorzList, 2, world, lineColor);


	lineColor = D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f);
	vertexVertList[0] = D3DXVECTOR3(0, 0, -gs);
	vertexVertList[1] = D3DXVECTOR3(0, 0, +gs);
	line->DrawTransform(vertexVertList, 2, world, lineColor);
}


void CTerrain::Update(float deltaTime)
{
}


void CTerrain::Render()
{
	// Draw the line.
	LPD3DXLINE _line;
	D3DXCreateLine(device, &_line);
	
	const D3DXMATRIX* world = mCamera->GetWorldMatrix();
	_line->Begin();
	_line->SetWidth(20.0f);

	DrawGrid(_line);
	DrawPlus(_line);


	_line->End();
}




void CTerrain::DrawGrid(LPD3DXLINE line)
{
	const float gs = 500.0f;

	D3DXVECTOR3 vertexHorzList[2];
	D3DXVECTOR3 vertexVertList[2];
	D3DCOLOR lineColor;
	const D3DXMATRIX* world = mCamera->GetWorldMatrix();


	for (float i = -gs; i <= gs; i += gs / 10.0f)
	{
		if (i)
		{
			lineColor = D3DXCOLOR(0.1f, 0.1f, 0.1f, 1.0f);
			vertexHorzList[0] = D3DXVECTOR3(-gs, 0, i);
			vertexHorzList[1] = D3DXVECTOR3(+gs, 0, i);

			vertexVertList[0] = D3DXVECTOR3(i, 0, -gs);
			vertexVertList[1] = D3DXVECTOR3(i, 0, +gs);

			line->DrawTransform(vertexHorzList, 2, world, lineColor);
			line->DrawTransform(vertexVertList, 2, world, lineColor);
		}
	}

#if false
	lineColor = D3DXCOLOR(0.0f, 0.0f, 0.8f, 1.0f);
	vertexHorzList[0] = D3DXVECTOR3(-gs, 0, 0);
	vertexHorzList[1] = D3DXVECTOR3(+gs, 0, 0);

	lineColor = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
	vertexVertList[0] = D3DXVECTOR3(0, 0, 0);
	vertexVertList[1] = D3DXVECTOR3(0, 0, +gs);


	line->DrawTransform(vertexHorzList, 2, world, lineColor);
	line->DrawTransform(vertexVertList, 2, world, lineColor);
#endif
}


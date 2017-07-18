#include "Terrain.h"



CTerrain::CTerrain(LPDIRECT3DDEVICE9 d3dDevice)
{
	device = d3dDevice;
}


CTerrain::~CTerrain()
{
}




void CTerrain::Init(UINT16 gridX, UINT16 gridZ, D3DCOLOR color, float size)
{
	// Check if the size of the grid is null
	if (size <= 0)
		return;

	// Calculate the data
	float GridXStep = size / gridX;
	float GridZStep = size / gridZ;
	float halfSize = size * 0.5f;

	// Set the attributes to the paint device
	device->SetTexture(0, NULL);
//	device->SetFVF(CUSTOMVERTEX::getFlags());

	// Draw the lines of the X axis
	for (float i = -halfSize; i <= halfSize; i += GridXStep)
	{
		CUSTOMVERTEX v[] =
		{ { i, 0.0f, -halfSize, 0 },{ i, 0.0f, halfSize, 0 } };

		device->DrawPrimitiveUP(D3DPT_LINELIST, 1, v, sizeof(CUSTOMVERTEX));
	}

	// Draw the lines of the Z axis
	for (float i = -halfSize; i <= halfSize; i += GridZStep)
	{
		CUSTOMVERTEX v[] =
		{ { -halfSize, 0.0f, i, 0 },{ halfSize, 0.0f, i, 0 } };

		device->DrawPrimitiveUP(D3DPT_LINELIST, 1, v, sizeof(CUSTOMVERTEX));
	}
}


void CTerrain::Update(float deltaTime)
{



}


void CTerrain::Render()
{
	D3DXVECTOR3 vertexList[2];
	vertexList[0].x = 0;
	vertexList[0].y = 0;
	vertexList[0].z = 0;

	vertexList[1].x = 0.1f;
	vertexList[1].y = 0;
	vertexList[1].z = 0;

	// Draw the line.
	LPD3DXLINE _line;
	D3DXCreateLine(device, &_line);
	D3DXMATRIX tempFinal;
	
	device->GetTransform(D3DTS_WORLD, &tempFinal);
//	D3DXMATRIX tempFinal = TheCamera.m_matView * matProj;


	_line->Begin();
	_line->SetWidth(20.0f);
	_line->DrawTransform(vertexList, 2, &tempFinal, D3DCOLOR_RGBA(0x40, 0xFF, 0x80, 0xFF));
	_line->End();
}

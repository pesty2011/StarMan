#include "Terrain.h"



Terrain::Terrain(LPDIRECT3DDEVICE9 d3dDevice)
{
	device = d3dDevice;
}


Terrain::~Terrain()
{
}




void Terrain::Init(UINT16 gridX, UINT16 gridZ, D3DCOLOR color, float size)
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



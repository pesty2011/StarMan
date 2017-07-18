// include the basic windows header file
#include <windows.h>
#include <windowsx.h>
#include <d3d9.h>
#include "Skeleton.h"
#include "Terrain.h"
#include "MainCamera.h"
#include "GlobalTimer.h"


// include the Direct3D Library file
#pragma comment (lib, "d3d9.lib")



// global declarations
LPDIRECT3D9			d3d;			// the pointer to our Direct3D interface
LPDIRECT3DDEVICE9	d3ddev;			// the pointer to the device class

D3DCOLOR			bgColor;		// background color



CSkeleton*			mSkeleton;
CMainCamera*		mCamera;
CTerrain*			mTerrain;


// function prototypes
void initD3D(HWND hWnd);			// sets up and initializes Direct3D
void render_frame(void);			// renders a single frame
void cleanD3D(void);				// closes Direct3D and releases memory
BOOL ProcessKeys(WPARAM wParam);
void InitGame();
void cleanGame();
void Update();

// the WindowProc function prototype
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);





// the entry point for any Windows program
int WINAPI WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow)
{
	// the handle for the window, filled by a function
	HWND hWnd;
	// this struct holds information for the window class
	WNDCLASSEX wc;

	// setup default background color
	bgColor = D3DCOLOR_XRGB(0, 40, 100);

	// clear out the window class for use
	ZeroMemory(&wc, sizeof(WNDCLASSEX));

	// fill in the struct with the needed information
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;				// redraw window if width/height changes
	wc.lpfnWndProc = WindowProc;
	wc.hInstance = hInstance;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wc.lpszClassName = L"WindowClass1";


	// register the window class
	RegisterClassEx(&wc);

	// create the window and use the result as the handle
	hWnd = CreateWindowEx(NULL,
		L"WindowClass1",			// name of the window class
		L"Star-Man",				// title of the window
		WS_OVERLAPPEDWINDOW,		// window style
		300,						// x-position of the window
		300,						// y-position of the window
		800,						// width of the window
		600,						// height of the window
		NULL,						// we have no parent window, NULL
		NULL,						// we aren't using menus, NULL
		hInstance,					// application handle
		NULL);						// used with multiple windows, NULL

				  // display the window on the screen
	ShowWindow(hWnd, nCmdShow);

	// setup the Direct3D
	initD3D(hWnd);
	InitGame();

	// enter the main loop:

	// this struct holds Windows event messages
	MSG msg;

	while (TRUE)
	{
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}



		if (msg.message == WM_QUIT)
			break;

		Update();
		render_frame();
	}

	// clean up DirectX and COM
	cleanD3D();
	cleanGame();

	// return this part of the WM_QUIT message to Windows
	return msg.wParam;
}



// this is the main message handler for the program
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	// sort through and find what code to run for the message given
	switch (message) {

	case WM_DESTROY:
	{
		// close the application entirely
		PostQuitMessage(0);
		return 0;
	} 
	break;

	case WM_CHAR:
		ProcessKeys(wParam);
		break;


	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE)
		{
			PostQuitMessage(0);
			return 0;
		}
		else
		{
			ProcessKeys(wParam);
		}
	}

	// Handle any messages the switch statement didn't
	return DefWindowProc(hWnd, message, wParam, lParam);
}



/*
	simple process of input keys for the starman demo

*/
BOOL ProcessKeys(WPARAM wParam)
{
	if (wParam == VK_NUMPAD0)
		bgColor = D3DCOLOR_XRGB(40, 0, 100);


	if (wParam == 'a')
		bgColor = D3DCOLOR_XRGB(100, 100, 100);


	return FALSE;
}




/*
	Init the direct3d screen

*/
void initD3D(HWND hWnd)
{
	d3d = Direct3DCreate9(D3D_SDK_VERSION);		// create the Direct3D interface

	D3DPRESENT_PARAMETERS d3dpp;				// create a struct to hold various device information

	ZeroMemory(&d3dpp, sizeof(d3dpp));			// clear out the struct for use
	d3dpp.Windowed = TRUE;						// program windowed, not fullscreen
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;	// discard old frames
	d3dpp.hDeviceWindow = hWnd;					// set the window to be used by Direct3D



	// create a device class using this information and information from the d3dpp stuct
	d3d->CreateDevice(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_SOFTWARE_VERTEXPROCESSING,
		&d3dpp,
		&d3ddev);
}



// this is the function used to render a single frame
void render_frame(void)
{
	// clear the window to a deep blue
	d3ddev->Clear(0, NULL, D3DCLEAR_TARGET, bgColor, 1.0f, 0);

	d3ddev->BeginScene();    // begins the 3D scene

							 // do 3D rendering on the back buffer here
	mTerrain->Render();

	d3ddev->EndScene();    // ends the 3D scene

	d3ddev->Present(NULL, NULL, NULL, NULL);    // displays the created frame
}





// this is the function that cleans up Direct3D and COM
void cleanD3D(void)
{
	d3ddev->Release();    // close and release the 3D device
	d3d->Release();    // close and release Direct3D
}



void InitGame()
{
	// create the camera
	mCamera = new CMainCamera(d3ddev);


	// create a terrain
	mTerrain = new CTerrain(d3ddev);


	// create a skeleton
	mSkeleton = new	CSkeleton();
	if (mSkeleton == NULL)
	{
		PostQuitMessage(0);
	}



	mSkeleton->mOffset.x = 0;
	mSkeleton->mOffset.y = 0;
	mSkeleton->mOffset.z = 0;

	CBone* bone = new CBone();
	bone->mName = "bone_1";
	bone->mOffset.x = 10;
	mSkeleton->InsertChild(bone);

	bone = new CBone();
	bone->mName = "bone_2";
	bone->mOffset.x = 10;
	mSkeleton->InsertChild(bone);
}

void cleanGame()
{
	delete mSkeleton;
}




void Update()
{
	mCamera->Update(0);
}



#if 0
void DrawLines()
{
	if (NULL == d3ddev)
		return;

	// Clear the backbuffer to a blue color
	d3ddev->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 255), 1.0f, 0);

	// Begin the scene
	if (SUCCEEDED(g_pd3dDevice->BeginScene()))
	{
		LPD3DXLINE line;
		D3DXCreateLine(g_pd3dDevice, &line);
		D3DXVECTOR2 lines[] = { D3DXVECTOR2(0.0f, 50.0f), D3DXVECTOR2(400.0f, 50.0f) };
		line->Begin();
		line->Draw(lines, 2, 0xFFFFFFFF);
		line->End();
		line->Release();

		// End the scene
		g_pd3dDevice->EndScene();
	}

	// Present the backbuffer contents to the display
	g_pd3dDevice->Present(NULL, NULL, NULL, NULL);
}

#endif
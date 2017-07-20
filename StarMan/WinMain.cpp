// include the basic windows header file
#include <windows.h>
#include <windowsx.h>
#include <d3d9.h>
#include <dinput.h>
#include "Skeleton.h"
#include "Terrain.h"
#include "MainCamera.h"
#include "GlobalTimer.h"


// include the Direct3D Library file
#pragma comment (lib, "d3d9.lib")
#pragma comment (lib, "dinput8.lib")



// global declarations
LPDIRECT3D9				d3d;			// the pointer to our Direct3D interface
LPDIRECT3DDEVICE9		d3ddev;			// the pointer to the device class
LPDIRECTINPUT8			di;
LPDIRECTINPUTDEVICE8	DIKeyboard;
D3DCOLOR				bgColor;		// background color
int						screenWidth = 1024;
int						screenHeight = 768;

CSkeleton*				mSkeleton;
CMainCamera*			mCamera;
CTerrain*				mTerrain;


// function prototypes
void initD3D(HWND hWnd);			// sets up and initializes Direct3D
void render_frame(void);			// renders a single frame
void cleanD3D(void);				// closes Direct3D and releases memory
BOOL ProcessKeys(WPARAM wParam);
void InitGame();
void cleanGame();
void Update(float dt);

// the WindowProc function prototype
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);


#if false

void InitDirectInput(HWND handle)
{
	DirectInput8Create(GetModuleHandle(NULL), DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&di, NULL);
	if (di != NULL)
	{
		di->CreateDevice(GUID_SysKeyboard, &DIKeyboard, NULL);
		if (DIKeyboard != NULL)
		{
			DIKeyboard->SetDataFormat(&c_dfDIKeyboard);
			DIKeyboard->SetCooperativeLevel(handle, DISCL_FOREGROUND | DISCL_EXCLUSIVE);
			DIKeyboard->Acquire();
		}
	}
}

#endif






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
		screenWidth,				// width of the window
		screenHeight,				// height of the window
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
	__int64 cntsPerSec = 0;
	QueryPerformanceFrequency((LARGE_INTEGER*)&cntsPerSec);
	float secsPerCnt = 1.0f / (float)cntsPerSec;

	__int64 prevTimeStamp = 0;
	QueryPerformanceFrequency((LARGE_INTEGER*)&prevTimeStamp);



	while (TRUE)
	{
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}



		if (msg.message == WM_QUIT)
			break;

		__int64 currTimeStamp = 0;
		QueryPerformanceFrequency((LARGE_INTEGER*)&currTimeStamp);			// what is the current time stamp
		float dt = (currTimeStamp - prevTimeStamp) * secsPerCnt;			// get out delta time


		Update(dt);
		render_frame();


		prevTimeStamp = currTimeStamp;										// save off the current time to be previous
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
			mCamera->Move(wParam);
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

//	d3ddev->SetTransform(D3DTS_PROJECTION, mCamera->GetProjectionMatrix());
//	d3ddev->SetTransform(D3DTS_VIEW, mCamera->GetViewMatrix());
							 
	// do 3D rendering on the back buffer here
	mTerrain->Render();




	d3ddev->EndScene();							// ends the 3D scene

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




void Update(float dt)
{
	mCamera->Update(dt);
}




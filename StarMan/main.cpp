
#include <Windows.h>
#include <GL\glew.h>
#include <GL\freeglut.h>
#include <iostream>
#include <cmath>
#include "Camera.h"
#include "Helpers\GameTimer.h"
#include "BVH\BVH.h"




#pragma comment(lib, "glew32.lib")
#pragma comment(lib, "freeglut.lib")




using namespace std;


void Display();
void Reshape(int w, int h);
void Keyboard(unsigned char key, int x, int y);
void KeyboardUp(unsigned char key, int x, int y);
void MouseMotion(int x, int y);
void Mouse(int button, int state, int x, int y);
void Timer(int value);
void Idle();
void Update(int value);

void DrawGrid();
void Grid();


void LoadBVH();
BVH g_bvh;
float animationTime = 0.0f;
int frameNum = 0;

Camera g_camera;
GameTimer g_timer;

bool g_key[256];
bool g_shift_down = false;
bool g_fps_mode = false;
int g_viewport_width = 0;
int g_viewport_height = 0;
bool g_mouse_left_down = false;
bool g_mouse_right_down = false;



// Movement settings
const float g_translation_speed = 0.05f;
//const float g_rotation_speed = M_PI / (180.0f * 0.2f);
const float g_rotation_speed = M_PI / (180.0f * 2.5f);



int main(int argc, char **argv) 
{
	LoadBVH();


	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(1280, 1024);
	glutCreateWindow("StarMan: Hit SPACEBAR to START");
	glDepthRange(0.1f, 1000.0f);

	glutIgnoreKeyRepeat(1);

	glutDisplayFunc(Display);
	glutIdleFunc(Display);
	glutReshapeFunc(Reshape);
	glutMouseFunc(Mouse);
	glutMotionFunc(MouseMotion);
	glutPassiveMotionFunc(MouseMotion);
	glutKeyboardFunc(Keyboard);
	glutKeyboardUpFunc(KeyboardUp);
	glutIdleFunc(Idle);

	
	glutTimerFunc(1, Timer, 0);
	glutTimerFunc(1, Update, 1);
	glutMainLoop();

	return 0;
}


void LoadBVH()
{
	g_bvh.Load("C:\\Users\\Rob\\Desktop\\ChaCha001.bvh");

//	g_bvh.Load(".\\bvh\\ChaCha001.bvh");

}


void Grid()
{
	glPushMatrix();
	glColor3f(1, 1, 1);

	for (int i = -50; i < 50; i++) {
		glBegin(GL_LINES);
		glVertex3f((float)i, (float)0, (float)-50);
		glVertex3f((float)i, (float)0, (float)50);
		glEnd();
	}

	for (int i = -50; i < 50; i++) {
		glBegin(GL_LINES);
		glVertex3f((float)-50, (float)0, (float)i);
		glVertex3f((float)50, (float)0, (float)i);
		glEnd();
	}

	glPopMatrix();
}



void Display(void) {
	glClearColor(0.3f, 0.3f, 0.3f, 1.0f); //clear the screen to black
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //clear the color buffer and the depth buffer
	glLoadIdentity();

	g_camera.Refresh();

	DrawGrid();

//	glutWireTeapot(0.5);
	glColor3f(0, 1.0f, 0.8f);
	glutWireCube(1.0f);

	if (g_bvh.IsLoadSuccess())
	{
		glColor3f(0.8, 0.8f, 1.0f);
		g_bvh.RenderFigure(frameNum, 0.1f);
	}

	glutSwapBuffers(); //swap the buffers
}



void Reshape(int w, int h) {
	g_viewport_width = w;
	g_viewport_height = h;

	glViewport(0, 0, (GLsizei)w, (GLsizei)h); //set the viewport to the current window specifications
	glMatrixMode(GL_PROJECTION); //set the matrix to projection

	glLoadIdentity();
	gluPerspective(60, (GLfloat)w / (GLfloat)h, 0.1, 100.0); //set the perspective (angle of sight, width, height, ,depth)
	glMatrixMode(GL_MODELVIEW); //set the matrix back to model
}



void Keyboard(unsigned char key, int x, int y)
{
	if (key == 27) {
		exit(0);
	}

	if (key == ' ') {
		g_fps_mode = !g_fps_mode;

		if (g_fps_mode) {
			glutSetCursor(GLUT_CURSOR_NONE);
			glutWarpPointer(g_viewport_width / 2, g_viewport_height / 2);
		}
		else {
			glutSetCursor(GLUT_CURSOR_LEFT_ARROW);
		}
	}

	if (glutGetModifiers() == GLUT_ACTIVE_SHIFT) {
		g_shift_down = true;
	}
	else {
		g_shift_down = false;
	}

	g_key[key] = true;
}



void KeyboardUp(unsigned char key, int x, int y)
{
	g_key[key] = false;
}




void Update(int value)
{
	// Some camera timing stuff
	float x, y, z;
	g_camera.GetPos(x, y, z);
	float yaw = g_camera.GetYaw();
	float pitch = g_camera.GetPitch();
	//	cout << "Camera: " << x << ", " << y << ", " << z << " Yaw: " << yaw << " Pitch: " << pitch << endl;


	// calculate the timer for the system
	g_timer.Tick();

	float dtime = g_timer.DeltaTime();
	float ttime = g_timer.TotalTime();

//	cout << "Total Time: " << ttime << " DeltaTime: " << dtime << endl;


	// animation timers
	if (g_bvh.IsLoadSuccess())
	{
		float interval = g_bvh.GetInterval();
		interval /= 2.0f;

		animationTime += dtime;
		if (animationTime >= interval)
		{
			frameNum += 2;
			frameNum = frameNum % g_bvh.GetNumFrame();
			animationTime = animationTime - interval;
		}
	}



	glutTimerFunc(1, Update, 1);
}





void Timer(int value)
{
	if (g_fps_mode) {
		if (g_key['w'] || g_key['W']) {
			g_camera.Move(g_translation_speed);
		}
		else if (g_key['s'] || g_key['S']) {
			g_camera.Move(-g_translation_speed);
		}
		else if (g_key['a'] || g_key['A']) {
			g_camera.Strafe(g_translation_speed);
		}
		else if (g_key['d'] || g_key['D']) {
			g_camera.Strafe(-g_translation_speed);
		}
		else if (g_mouse_left_down) {
			g_camera.Fly(-g_translation_speed);
		}
		else if (g_mouse_right_down) {
			g_camera.Fly(g_translation_speed);
		}
		else if (g_key['R']) 
		{
			// Reset the scene ...
			g_camera.Reset();
		}
		else if (g_key['l'])
		{
#if false
			const int  file_name_len = 256;
			char  file_name[file_name_len] = "";


			OPENFILENAME	open_file;
			memset(&open_file, 0, sizeof(OPENFILENAME));
			open_file.lStructSize = sizeof(OPENFILENAME);
			open_file.hwndOwner = NULL;
			open_file.lpstrFilter = "BVH Motion Data (*.bvh)\0*.bvh\0All (*.*)\0*.*\0";
			open_file.nFilterIndex = 1;
			open_file.lpstrFile = file_name;
			open_file.nMaxFile = file_name_len;
			open_file.lpstrTitle = "Select a BVH file";
			open_file.lpstrDefExt = "bvh";
			open_file.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
			BOOL  ret = GetOpenFileName(&open_file);
#endif

		}
	}

	glutTimerFunc(1, Timer, 0);
}

void Idle()
{
	Display();
}




void Mouse(int button, int state, int x, int y)
{
	if (state == GLUT_DOWN) {
		if (button == GLUT_LEFT_BUTTON) {
			g_mouse_left_down = true;
		}
		else if (button == GLUT_RIGHT_BUTTON) {
			g_mouse_right_down = true;
		}
	}
	else if (state == GLUT_UP) {
		if (button == GLUT_LEFT_BUTTON) {
			g_mouse_left_down = false;
		}
		else if (button == GLUT_RIGHT_BUTTON) {
			g_mouse_right_down = false;
		}
	}
}


void MouseMotion(int x, int y)
{
	// This variable is hack to stop glutWarpPointer from triggering an event callback to Mouse(...)
	// This avoids it being called recursively and hanging up the event loop
	static bool just_warped = false;

	if (just_warped) {
		just_warped = false;
		return;
	}

	if (g_fps_mode) {
		int dx = x - g_viewport_width / 2;
		int dy = y - g_viewport_height / 2;

		if (dx) {
			g_camera.RotateYaw(g_rotation_speed*dx);
		}

		if (dy) {
			g_camera.RotatePitch(g_rotation_speed*dy);
		}

		glutWarpPointer(g_viewport_width / 2, g_viewport_height / 2);

		just_warped = true;
	}
}



void DrawGrid()
{
	glPushMatrix();
#if true
	const double gs = 100;



	glColor3d(0.0, 0.0, 0.0);		// create the grid
	for (double i = -gs; i <= gs; i += (gs / 50))
	{
		if (i)
		{

			glBegin(GL_LINES);
			glVertex3d(-gs, 0, i);
			glVertex3d(+gs, 0, i);
			glVertex3d(i, 0, -gs);
			glVertex3d(i, 0, +gs);
			glEnd();
		}
	}

	glBegin(GL_LINES);

	//### west east line
	glColor3d(0.0, 0.0, 0.8);				// east/west lines
	glVertex3d(-gs, 0, 0);
	glVertex3d(+gs, 0, 0);
	glVertex3d(0, 0, -gs);
	glVertex3d(0, 0, 0);


	//### north line
	glColor3d(1.0, 0.0, 0.0);				// red line
	glVertex3d(0, 0, 0);
	glVertex3d(0, 0, +gs);

	glEnd();

#else

	float  size = 1.5f;
	int  num_x = 10, num_z = 10;
	double  ox, oz;
	glBegin(GL_QUADS);
	glNormal3d(0.0, 1.0, 0.0);
	ox = -(num_x * size) / 2;
	for (int x = 0; x<num_x; x++, ox += size)
	{
		oz = -(num_z * size) / 2;
		for (int z = 0; z<num_z; z++, oz += size)
		{
			if (((x + z) % 2) == 0)
				glColor4f(1.0, 1.0, 1.0, 0.5f);
			else
				glColor4f(0.8, 0.8, 0.8, 0.5f);
			glVertex3d(ox, 0.0, oz);
			glVertex3d(ox, 0.0, oz + size);
			glVertex3d(ox + size, 0.0, oz + size);
			glVertex3d(ox + size, 0.0, oz);
		}
	}
	glEnd();

#endif
	glPopMatrix();

}



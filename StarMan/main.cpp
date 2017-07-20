
#include <Windows.h>
#include <GL\glew.h>
#include <GL\freeglut.h>
#include <iostream>
#include <cmath>
#include "Camera.h"


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


void DrawGrid();
void Grid();

Camera g_camera;
bool g_key[256];
bool g_shift_down = false;
bool g_fps_mode = false;
int g_viewport_width = 0;
int g_viewport_height = 0;
bool g_mouse_left_down = false;
bool g_mouse_right_down = false;



// Movement settings
const float g_translation_speed = 0.05f;
const float g_rotation_speed = M_PI / (180.0f * 0.2f);





int main(int argc, char **argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(1280, 1024);
	glutCreateWindow("StarMan - Hit SPACEBAR to START");
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
	glutMainLoop();


	return 0;
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

	glColor3f(0, 1, 0);

//	glutWireTeapot(0.5);

	glutWireCube(1.0);
	DrawGrid();

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



void Update(float deltaTime)
{


}




void DrawGrid()
{
	const double gs = 100;

	glPushMatrix();


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

	glPopMatrix();
}



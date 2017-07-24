#include <Windows.h>
#include <GL\glew.h>
#include <GL\freeglut.h>
#include <iostream>
#include <cmath>
#include "Camera.h"
#include "Helpers\GameTimer.h"
#include "BVH\BVH.h"
#include "Entities\EntityManager.h"
#include "EntityNames.h"
#include "AssetManager.h"


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

void Print(string text);


/*
	GLOBAL APPLICATION VARIABLES 

*/
Camera			g_camera;			// the main camera
GameTimer		g_timer;			// global timer for the application
bool			bStartGame = false;


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



/* ----------------------------------------------------------------------------
	Summary:
	initialize the starman application by creating all of the entities
	that will be involved in the actual demo.


---------------------------------------------------------------------------- */
void InitStarMan()
{
	LoadBVH();
	CBaseEntity* entity;

	entity = new CBaseEntity(StarMan_1);
	EntityMgr->AddEntity(entity);

//	entity = new CBaseEntity(StarMan_2);
//	EntityMgr->AddEntity(entity);
}



/* ----------------------------------------------------------------------------
	Summary:
	Update all of the entities in the entity manager

---------------------------------------------------------------------------- */
void UpdateStarMan(float dTime)
{
	EntityMgr->Update(dTime);
}



/* ----------------------------------------------------------------------------
	Summary:
	Destroy starman and remove all of the elements of the 
	application.

---------------------------------------------------------------------------- */
void DestroyStarMan()
{
}





int main(int argc, char **argv) 
{
	InitStarMan();

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(1280, 1024);
	glutCreateWindow("StarMan: Hit SPACEBAR to TOGGLE Camera control");
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
	// load up the assets for the demo ...
	AssetMgr->AddMotion("C:\\Users\\Rob\\Desktop\\bvh\\ChaCha001.bvh");


	// karate moves
	AssetMgr->AddMotion("C:\\Users\\Rob\\Desktop\\bvh\\karate-01.bvh");
	AssetMgr->AddMotion("C:\\Users\\Rob\\Desktop\\bvh\\karate-02.bvh");
	AssetMgr->AddMotion("C:\\Users\\Rob\\Desktop\\bvh\\karate-03.bvh");
	AssetMgr->AddMotion("C:\\Users\\Rob\\Desktop\\bvh\\karate-04.bvh");
	AssetMgr->AddMotion("C:\\Users\\Rob\\Desktop\\bvh\\karate-05.bvh");
	AssetMgr->AddMotion("C:\\Users\\Rob\\Desktop\\bvh\\karate-06.bvh");
	AssetMgr->AddMotion("C:\\Users\\Rob\\Desktop\\bvh\\karate-07.bvh");
	AssetMgr->AddMotion("C:\\Users\\Rob\\Desktop\\bvh\\karate-08.bvh");
	AssetMgr->AddMotion("C:\\Users\\Rob\\Desktop\\bvh\\karate-09.bvh");
	AssetMgr->AddMotion("C:\\Users\\Rob\\Desktop\\bvh\\karate-10.bvh");
	AssetMgr->AddMotion("C:\\Users\\Rob\\Desktop\\bvh\\karate-11.bvh");
	AssetMgr->AddMotion("C:\\Users\\Rob\\Desktop\\bvh\\karate-12.bvh");
	AssetMgr->AddMotion("C:\\Users\\Rob\\Desktop\\bvh\\karate-13.bvh");
	AssetMgr->AddMotion("C:\\Users\\Rob\\Desktop\\bvh\\karate-14.bvh");
	AssetMgr->AddMotion("C:\\Users\\Rob\\Desktop\\bvh\\karate-15.bvh");
	AssetMgr->AddMotion("C:\\Users\\Rob\\Desktop\\bvh\\karate-16.bvh");
	AssetMgr->AddMotion("C:\\Users\\Rob\\Desktop\\bvh\\karate-17.bvh");
	AssetMgr->AddMotion("C:\\Users\\Rob\\Desktop\\bvh\\karate-18.bvh");
	AssetMgr->AddMotion("C:\\Users\\Rob\\Desktop\\bvh\\karate-19.bvh");
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
	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);			
	glLoadIdentity();

	g_camera.Refresh();

	float		light0_position[] = { 10.0, 10.0, 10.0, 1.0 };
	glLightfv(GL_LIGHT0, GL_POSITION, light0_position);

	DrawGrid();

//	glutWireTeapot(0.5);
	glColor3f(0, 1.0f, 0.8f);
	glutWireCube(1.0f);

	// display the entities on screen
	EntityMgr->Display();


	Print("STARMAN");
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
	else if (key == 'p')
	{
		// trigger playing an animation ...
		CBaseEntity* entity = EntityMgr->FindEntity(StarMan_1);
		if (entity != NULL)
		{
//			entity->Play("karate-02");
			bStartGame = true;
		}
	}
	else if (key == '0')
	{
		//StopAnimation();
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
#if false
	// Some camera timing stuff
	float x, y, z;
	g_camera.GetPos(x, y, z);
	float yaw = g_camera.GetYaw();
	float pitch = g_camera.GetPitch();
	//	cout << "Camera: " << x << ", " << y << ", " << z << " Yaw: " << yaw << " Pitch: " << pitch << endl;
#endif
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
	}

	glutTimerFunc(1, Timer, 0);
}

void Idle()
{
	// update all of the entities
	g_timer.Tick();									// Update timer
	float dTime = g_timer.DeltaTime();				// calculate the delta timer

	if (bStartGame == true)
	{
		UpdateStarMan(dTime);
	}
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

	glColor3d(0.1, 0.1, 0.1);		// create the grid
	for (double i = -gs; i <= gs; i += 5)
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



void Print(string text)
{
	glPushMatrix();

	glRasterPos2f(5.0f, 5.0f);
	int len = (int)text.length();

	for (int i = 0; i < len; i++)
	{
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, text[i]);
	}

	glPopMatrix();
}



/*
 * Student: Geneva Smith (0964909)
 * Course: Software Engineering 3GC3
 * Professor: Dr. R. Teather
 * Assignment 2: Terrrain Generator
 * File: Terrain Generator
 * --------------------------------------
 * This  program generates a terrain using the HeightMapMesh class. It adds extra features, such as lighting,
 * and allows a user to toggle between different lighting and shading modes. It also allows for a mobile
 * viewing angle.
 */

//C++ libraries
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <ctime>
#include <math.h>

//OPenGL libraries
#include <OpenGL/OpenGL.h>
#include <GLUT/GLUT.h>

//Programmer defined libraries and classes
#include "Height Map Mesh.h"

using namespace std;	//Allows the use of vectors and I/O

/*	Global Variables	*/
vector <HeightMesh> maps;

//Map parameters
int width, height;
bool start = false;

//Wireframe options
int wireframeOption = 0;
bool wireframeToggle = false;

//Shading and light options
bool shader = true;
bool lights = false;
bool light0on = true;
GLfloat light0pos [] = {1, 10, 0, 1};
GLfloat light1pos [] = {-1, 10, 0, 1};

//Viewpoint rotation
int xrot = 0;
int yrot = 0;

//Drawing the 2D overview map
void OverviewMap(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	maps[0].DrawOverview();

	//Clearing the pipeline
	glFlush();
	glutPostRedisplay();
}

//Drawing the main terrain graphics
void display(void)
{
	//Clear screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Camera
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//Transforms
	glRotated(xrot, 1, 0, 0);
	glRotated(yrot, 0, 1, 0);
	glTranslated(0, -1 * maps[0].GetMaxHeight() / 2, 0);

	//Lighting
	if (lights == true)
	{
		glEnable(GL_LIGHTING);
		glEnable(GL_COLOR_MATERIAL);

		glLightfv(GL_LIGHT0, GL_POSITION, light0pos);
		glEnable(GL_LIGHT0);

		glLightfv(GL_LIGHT1, GL_POSITION, light1pos);
		glEnable(GL_LIGHT1);
	}
	else glDisable(GL_LIGHTING);

	//Axis for referencing (x, y, z)
	/*glPushMatrix();
	glBegin(GL_LINES);
	glColor3d(1, 0, 0);
	glVertex3d(-50, 0, 0);
	glVertex3d(50, 0, 0);

	glColor3d(0, 1, 0);
	glVertex3d(0, -50, 0);
	glColor3d(0,0,0);
	glVertex3d(0, 50, 0);

	glColor3d(0, 0, 1);
	glVertex3d(0, 0, -30);
	glColor3d(1, 1, 1);
	glVertex3d(0, 0, 30);
	glEnd();
	glPopMatrix();*/

	//Draw Terrain
	maps[0].DrawHeightMesh(wireframeToggle, shader);
	if (wireframeOption == 2) maps[0].DrawHeightMesh(false, shader);
	
	//Flush pipeline
	glFlush();

	//Double Buffering
	glutSwapBuffers();

	//Flush pipeline
	glutPostRedisplay();
}

//Initializing the terrain after the initial parameters are given
void CreateTerrain(void)
{
	//Generate flat terrain
	HeightMesh terrain(width + 1, height + 1);

	//Create hills
	terrain.CircleTerrain();

	//Force mesh to be global
	maps.push_back(terrain);

	//Get map parameters - width, length and maximum height
	width = maps[0].GetWidth();
	height = maps[0].GetLength();
	light0pos[1] = maps[0].GetMaxHeight() + 8;
	light1pos[1] = maps[0].GetMaxHeight() + 8;
}

void Help()
{
	cout << endl;
	cout << "W/w -> Turn on wireframe with/without the terrain itself underneath" << endl;
	cout << "K/k -> Choose the shading mode (Flat(default) or Gouraud" << endl;
	cout << "L/l -> Turn on/off the lights" << endl;
	cout << "I/i -> Choose which light to control (two options)" << endl;
	cout << "T/t and B/b -> move the light along the y axis (up and down respectively)" << endl;
	cout << "G/g and H/h -> move the light along the x axis (left and right respectively)" << endl;
	cout << "V/v and Y/y -> move the light along the z axis (forward and backward respectively)" << endl;
	cout << "R/r -> Generate a new terrain" << endl;
	cout << "Q/q -> Exit the generator" << endl;
	cout << endl;
	cout << "If you ever need to see the control list again, just hit A/a for Assistance" << endl;
	cout << endl;
	
	return;
}

//Initiates the creation of the terrain - user triggered
void getTerrainSize(void)
{
	double iwidth, iheight;

	cout << "Please enter the width of the terrain: ";
	cin >> iwidth;
	width = floor(iwidth);
	cout << "Please enter the length of the terrain: ";
	cin >> iheight;
	height = floor(iheight);

	CreateTerrain();
}

/* kbd -- the GLUT keyboard function 
 *  key -- the key pressed
 *  x and y - mouse x and y coordinates at the time the function is called
 *	Q/q -> Quit
 *	R/r -> create a new terrain
 *	I/i -> Toggle between light0 and light1 controls
 *	T/t -> Move light up y axis
 *	B/b -> Move light down the y axis
 *	G/g -> Move light left along the x axis
 *	H/h -> Move light right along the x axis
 *	V/v -> Move light forward along the z axis
 *	Y/y -> Move light backwards along the z axis
 *	L/l -> Toggle lighting on or off
 *	W/w -> Toggle wireframe modes
 *	K/k -> Toggle between flat and smooth shading
 *	A/a -> Display help script
 */
void kbd(unsigned char key, int x, int y)
{
	//Quit
	if(key == 'q' || key == 'Q')
	{
		exit(0);
	}

	//Reset
	else if (key == 'r' || key == 'R')
	{
		maps.erase(maps.begin());
		getTerrainSize();
	}
	
	//Help
	else if (key == 'A' || key == 'a') Help();
	
	//Toggle light controls
	else if (key == 'i' || key == 'I') light0on = !light0on;

	//Light controls
	//y axis
	else if (key == 't' || key == 'T') 
	{
		if (light0on == true) light0pos[1] += 1;
		else light1pos[1] += 1;
	}

	else if (key == 'b' || key == 'B')
	{
		if (light0on == true) light0pos[1] -= 1;
		else light1pos[1] -= 1;
	}

	//x axis
	else if (key == 'g' || key == 'G')
	{
		if (light0on == true) light0pos[0] -= 1;
		else light1pos[0] -= 1;
	}

	else if (key == 'h' || key == 'H')
	{
		if (light0on == true) light0pos[0] += 1;
		else light1pos[0] += 1;
	}

	//z axis
	else if (key == 'v' || key == 'V')
	{
		if (light0on == true) light0pos[2] += 1;
		else light1pos[2] += 1;
	}

	else if (key == 'y' || key == 'Y')
	{
		if (light0on == true) light0pos[2] -= 1;
		else light1pos[2] -= 1;
	}

	//Toggle lighting
	else if (key == 'l' || key == 'L') lights = !lights;

	//Toggle wireframes
	else if (key == 'w' || key == 'W')
	{
		wireframeOption = (wireframeOption + 1) % 3;
		if (wireframeOption != 0) wireframeToggle = true;
		else wireframeToggle = false;
	}

	//Toggle shading
	else if (key == 'k' || key == 'K') shader = !shader;
}

//Controlling what the arrow keys trigger
void arrows(GLint specialKey, GLint xMouse, GLint ymouse)
{
	//Control the up/down rotation of the viewpoint
	if (specialKey == GLUT_KEY_UP)
	{
		if (xrot - 1 > -90) xrot--;
	}

	else if (specialKey == GLUT_KEY_DOWN)
	{
		if (xrot + 1 < 90) xrot++;
	}

	//Control the left/right rotation of the viewpoint
	else if (specialKey == GLUT_KEY_LEFT) yrot++;

	else if (specialKey == GLUT_KEY_RIGHT) yrot--;
}

/*	MAIN PROGRAM	*/

int main(int argc, char** argv)
{
	//Creating initial terrain
	if (start == false)
	{
		cout << "Welcome to the Terrain Generator! This is an interactive environment where you can control how the ";
		cout << "terrain appears. Here are a few controls that you should know about:" << endl;
		cout << endl;
		Help();
		getTerrainSize();
		start = true;
	}

	//Initialization of GLUT windows
	glutInit(&argc, argv);

	//Overview window
	glutInitWindowSize(height, width);
	int subWin = glutCreateWindow("Overview");
	glutPositionWindow(700, 70);
	glClear(GL_COLOR_BUFFER_BIT);
	glClearColor(0, 0, 0, 0);

	glMatrixMode(GL_PROJECTION);
	glOrtho(0, height, width, 0, -5, 5);

	glutDisplayFunc(OverviewMap);	

	//Terrain window
	glutInitWindowSize(600, 600);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	int mainWin = glutCreateWindow("Terrain Generator");

	//Depth testing
	glEnable(GL_DEPTH_TEST);

	//Face culling
	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);

	glMatrixMode(GL_PROJECTION);
	glOrtho(-height / 2, height / 2, -width / 2, width / 2, -width * height / 2, width * height / 2);

	//Black Background
	glClearColor(0, 0, 0, 0);

	//GLUT Callbacks
	glutKeyboardFunc(kbd);
	glutSpecialFunc(arrows);
	glutDisplayFunc(display);

	glutMainLoop();

	return 0;
}
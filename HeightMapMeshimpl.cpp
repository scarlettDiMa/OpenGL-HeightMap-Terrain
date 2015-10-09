/*
 * Geneva Smith
 * Terrrain Generator
 * File: Height Mesh Class Implementation
 * --------------------------------------
 * This  program implements all functions that can be found in the Height Map Mesh class header
 * file. It references private class variables from the HeightMeshpriv header file.
 */

//Graphic libraries
#include <GLUT/GLUT.h>
#include <OpenGL/OpenGL.h>

//C++ libraries
#include <iostream>
#include <cmath>
#include <vector>

//Associated header file
#include "Height Map Mesh.h"

using namespace std;

//Constant values
const double MAP_SCALE_X = 1.5;
const double MAP_SCALE_Z = 1.5;
const double DISPLACEMENT = 0.3;
const int MAX_RADIUS = 20;

//Maximum height to calculate colours
double maxHeight = 0;


/*	Class Constructor	*/
/*
 * The class constructor sets the private class variables for width and depth to the parameters given to it
 * by the user. It then dynamically creates three 2D arrays - one for each heights. face ormals and vertex
 * normals. It fills in the height cells with an intial value of zero in preparation for the summations
 * computed by the terrain algorithm.
 */

HeightMesh::HeightMesh(int x, int z)
{
	mapWidth = x;
	mapLength = z;

	//Creating a 2D array dynamically to match map xz plane
	heights = new double *[mapLength];

	for (int i = 0; i < mapLength; i++)
	{
		heights[i] = new double [mapWidth];
	}

	//Initializing the height values to 0 for summations later
	for (int j = 0; j < mapLength; j++)
	{
		for (int k = 0; k < mapWidth; k++)
		{
			heights[j][k] = 0;
		}
	}

	//Dynamic array creation for Face normals
	faceIndex = (mapWidth - 1) * 2;

	faceNormals = new vector <double> *[mapLength - 1];
	for (int i = 0; i < mapLength; i++) faceNormals[i] = new vector <double> [faceIndex];

	//Dynamic array creation for Point normals
	vertexNormals = new vector <double> *[mapLength];
	for (int i = 0; i < mapLength; i++) vertexNormals[i] = new vector <double> [mapWidth];
}

/*	Class Destructor	*/
HeightMesh::~HeightMesh()
{
	/*
	if (heights != NULL)
	{
		for (int i = 0; i < mapWidth; i++) delete [] heights[i];
		delete [] heights;
	}
	*/
}


/*	Get Functions	*/
/*
 * These functions allow the user to retrieve private map parameters as they need.
 */

int HeightMesh::GetLength()
{
	return mapLength;
}

int HeightMesh::GetWidth()
{
	return mapWidth;
}

double HeightMesh::GetMaxHeight()
{
	return maxHeight;
}

/*	Drawing Functions	*/
/*
 * These functions are used to draw the actual height mesh. It allows for toggling between shading types
 * flat or Gouraud) and between wireframe modes (on or off).
 * The overview function is used to draw a 2D overhead view of the terrain. It is coloured in the same fashion
 * as the 3D map.
 */

void HeightMesh::DrawHeightMesh(bool wireframe, bool shadeflat)
{
	for (int i = 0; i < mapLength - 1; i++)
	{
		//Determine if it is wireframe or filled terrain
		if (wireframe == true) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		else glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		//Shade modelling
		if (shadeflat == true) glShadeModel(GL_FLAT);
		else glShadeModel(GL_SMOOTH);

		//Triangle strips
		glBegin(GL_TRIANGLE_STRIP);
			for (int j = 0; j < mapWidth; j++)
			{
				//Colouring the first vertex
				if (wireframe == false)
				{
					if (heights[i][j] < 0) glColor3d(0, 1, 0);
					else if (heights[i][j] < maxHeight * 0.1) glColor3d(0.05, 0.95, 0);
					else if (heights[i][j] < maxHeight * 0.2) glColor3d(0.1, 0.9, 0);
					else if (heights[i][j] < maxHeight * 0.3) glColor3d(0.15, 0.85, 0);
					else if (heights[i][j] < maxHeight * 0.4) glColor3d(0.2, 0.8, 0);
					else if (heights[i][j] < maxHeight * 0.5) glColor3d(0.25, 0.75, 0);
					else if (heights[i][j] < maxHeight * 0.6) glColor3d(0.3, 0.7, 0);
					else if (heights[i][j] < maxHeight * 0.7) glColor3d(0.35, 0.65, 0);
					else if (heights[i][j] < maxHeight * 0.8) glColor3d(0.4, 0.6, 0);
					else if (heights[i][j] < maxHeight * 0.9) glColor3d(0.45, 0.55, 0);
					else glColor3d(0.5, 0.5, 0.5);
				}
				else glColor3d(1, 1, 1);

				//Normals
				if (shadeflat == true)
				{
					if (j > 0) glNormal3d(faceNormals[i][j - 1][0], faceNormals[i][j - 1][1], faceNormals[i][j - 1][2]);
				}
				else glNormal3d(vertexNormals[i][j][0], vertexNormals[i][j][1], vertexNormals[i][j][2]);

				//Draw first vertex
				if (wireframe == false) glVertex3d(double(i - mapLength / 2), heights[i][j], double(j - mapWidth / 2));
				else glVertex3d(double(i - mapLength / 2), (heights[i][j] + 0.1), double(j - mapWidth / 2));

				//Colouring the second vertex
				if (wireframe == false)
				{
					if (heights[i + 1][j] < 0) glColor3d(0, 1, 0);
					else if (heights[i + 1][j] < maxHeight * 0.1) glColor3d(0.05, 0.95, 0);
					else if (heights[i + 1][j] < maxHeight * 0.2) glColor3d(0.1, 0.9, 0);
					else if (heights[i + 1][j] < maxHeight * 0.3) glColor3d(0.15, 0.85, 0);
					else if (heights[i + 1][j] < maxHeight * 0.4) glColor3d(0.2, 0.8, 0);
					else if (heights[i + 1][j] < maxHeight * 0.5) glColor3d(0.25, 0.75, 0);
					else if (heights[i + 1][j] < maxHeight * 0.6) glColor3d(0.3, 0.7, 0);
					else if (heights[i + 1][j] < maxHeight * 0.7) glColor3d(0.35, 0.65, 0);
					else if (heights[i + 1][j] < maxHeight * 0.8) glColor3d(0.4, 0.6, 0);
					else if (heights[i + 1][j] < maxHeight * 0.9) glColor3d(0.45, 0.55, 0);
					else glColor3d(0.5, 0.5, 0.5);
				}
				else glColor3d(1, 1, 1);

				//Normals
				if (shadeflat == true)
				{
					if (j > 0) glNormal3d(faceNormals[i][j][0], faceNormals[i][j][1], faceNormals[i][j][2]);
				}
				else glNormal3d(vertexNormals[i + 1][j][0], vertexNormals[i + 1][j][1], vertexNormals[i + 1][j][2]);

				//Draw second vertex (finally -.-;;;)
				if (wireframe == false) glVertex3d(double(i + 1 - mapLength / 2), heights[i + 1][j], double(j - mapWidth / 2));
				else glVertex3d(double(i + 1 - mapLength / 2), (heights[i + 1][j] + 0.1), double(j - mapWidth / 2));
			}
		glEnd();
	}
	return;
}

void HeightMesh::DrawOverview()
{
	glBegin(GL_POINTS);
		for (int i = 0; i < mapLength; i++)
		{
			for (int j = 0; j < mapWidth; j++)
			{
				if (heights[i][j] < 0) glColor3d(0, 1, 0);
				else if (heights[i][j] < maxHeight * 0.1) glColor3d(0.05, 0.95, 0);
				else if (heights[i][j] < maxHeight * 0.2) glColor3d(0.1, 0.9, 0);
				else if (heights[i][j] < maxHeight * 0.3) glColor3d(0.15, 0.85, 0);
				else if (heights[i][j] < maxHeight * 0.4) glColor3d(0.2, 0.8, 0);
				else if (heights[i][j] < maxHeight * 0.5) glColor3d(0.25, 0.75, 0);
				else if (heights[i][j] < maxHeight * 0.6) glColor3d(0.3, 0.7, 0);
				else if (heights[i][j] < maxHeight * 0.7) glColor3d(0.35, 0.65, 0);
				else if (heights[i][j] < maxHeight * 0.8) glColor3d(0.4, 0.6, 0);
				else if (heights[i][j] < maxHeight * 0.9) glColor3d(0.45, 0.55, 0);
				else glColor3d(0.5, 0.5, 0.5);
				
				glVertex2d(i, j);
			}
		}
	glEnd();
	return;
}

/*	Terrain Algorithm	*/
/*
 * These functions intialiaize the terrain. filling in height values and computing the face and vertex normals.
 * These should only be called once by the user, unless they want to compound the effects of the terrain
 * generator.
 */

vector <double> ComputeUnitNormal(vector <double> p1, vector <double> p2, vector <double> p3)
{
	double x, y, z;
	double length;
	vector <double> u, v;
	vector <double> normal;
	
	//u vector
	x = p1[0] - p2[0]; u.push_back(x);
	y = p1[1] - p2[1]; u.push_back(y);
	z = p1[2] - p2[2]; u.push_back(z);

	//v vector
	x = p3[0] - p2[0]; v.push_back(x);
	y = p3[1] - p2[1]; v.push_back(y);
	z = p3[2] - p2[2]; v.push_back(z);

	//Normal
	x = u[1] * v[2] - u[2] * v[1];
	y = u[2] * v[0] - u[0] * v[2];
	z = u[0] * v[1] - u[1] * v[0];

	length = sqrt(x*x + y*y + z*z);

	normal.push_back(-1 * x / length);
	normal.push_back(-1 * y / length);
	normal.push_back(-1 * z / length);

	return normal;
}

void HeightMesh::CircleTerrain()
{
	//Determine number of hills in terrain
	const int area = mapWidth * mapLength;
	const int num = floor(area / 1000.0);

	const int numCircles = rand() % area + num * (mapWidth);

	//Adjust height values
	for (int a = 0; a < numCircles; a++)
	{
		//Circle parameters
		const int x = rand() % mapWidth;
		const int z = rand() % mapLength;
		const double radius = rand() % MAX_RADIUS + 1;

		//Progress bar (text version)
		cout << "Drawing progress: " << a << "/" << numCircles - 1 << endl;

        const int startx = (x - radius < 0) ? 0 : (x - radius);
        const int startz = (z - radius < 0) ? 0 : (z - radius);
        const int endx = (x + radius > mapWidth) ? mapWidth : (x + radius);
        const int endz = (z + radius > mapLength) ? mapLength : (z + radius);
        
		//Each terrain point
		for (int i = startz; i < endz; i++)
		{
            const int dz = z - i;
			for (int j = startx; j < endx; j++)
			{
				//Calculate y of points
				const int dx = x - j;

				//Calculate distance between point and circle centre
				const double distance = sqrt(double((dz*dz) + (dx*dx)));

				const double test = distance * 2 / radius;

				if (fabs(test) <= 1.0)
				{
					const double cosVal = test * 3.14 / 180;
					heights[i][j] += (DISPLACEMENT / 2.0)  + (cos(cosVal) * DISPLACEMENT / 2.0);

					if (heights[i][j] > maxHeight) maxHeight = heights[i][j];
				}
			}
		}
	}

	cout << "Calculating normals...might want to pull up a YouTube video or something" << endl;
	cout << "Calculating face normals..." << endl;
	vector <double> point;
	point.push_back(0); point.push_back(0); point.push_back(0);
	
	vector <double> p1, p2, p3;
	p1 = point; p2 = point; p3 = point;

	vector <double> face;

	for (int i = 0; i < mapLength - 1; i++)
	{
		for (int j = 0; j < faceIndex - 1; j++)
		{
			p1[0] = i + 1; p1[1] = heights[i + 1][j]; p1[2] = j;
			p2[0] = i; p2[1] = heights[i][j]; p2[2] = j;
			p3[0] = i; p3[1] = heights[i][j + 1]; p3[2] = j + 1;
			face = ComputeUnitNormal(p1, p2, p3);
			faceNormals[i][j] = face;

			p1[0] = i; p1[1] = heights[i][j + 1]; p1[2] = j + 1;
			p2[0] = i + 1; p2[1] = heights[i + 1][j]; p2[2] = j;
			p3[0] = i + 1; p3[1] = heights[i + 1][j + 1]; p3[2] = j + 1;
			face = ComputeUnitNormal(p1, p2, p3);
			faceNormals[i][j + 1] = face;
		}
	}

	cout << "Face normals complete...calculating point normals..." << endl;
	int index;

	//First row
	vertexNormals[0][0] = faceNormals[0][0];

	//Middle points first row
	for (int i = 1; i < mapWidth - 1; i++)
	{
		index = i * 2;
		point[0] = (faceNormals[0][index - 2][0] + faceNormals[0][index - 1][0] + faceNormals[0][index][0]) / 3;
		point[1] = (faceNormals[0][index - 2][1] + faceNormals[0][index - 1][1] + faceNormals[0][index][1]) / 3;
		point[2] = (faceNormals[0][index - 2][2] + faceNormals[0][index - 1][2] + faceNormals[0][index][2]) / 3;

		vertexNormals[0][i] = point;
	}

	//Last point, first row
	point[0] = (faceNormals[0][(mapWidth * 2) - 4][0] + faceNormals[0][(mapWidth * 2) - 3][0]) / 2;
	point[1] = (faceNormals[0][(mapWidth * 2) - 4][1] + faceNormals[0][(mapWidth * 2) - 3][1]) / 2;
	point[2] = (faceNormals[0][(mapWidth * 2) - 4][2] + faceNormals[0][(mapWidth * 2) - 3][2]) / 2;

	vertexNormals[0][mapWidth - 1] = point;

	//Middle rows
	for (int i = 1; i < mapLength - 1; i++)
	{
		//First point in the row
		point[0] = (faceNormals[i - 1][0][0] + faceNormals[i - 1][1][0] + faceNormals[i][0][0]) / 3;
		point[1] = (faceNormals[i - 1][0][1] + faceNormals[i - 1][1][1] + faceNormals[i][0][1]) / 3;
		point[2] = (faceNormals[i - 1][0][2] + faceNormals[i - 1][1][2] + faceNormals[i][0][2]) / 3;

		vertexNormals[i][0] = point;

		//Middle row points (columns)
		for (int j = 1; j < mapWidth - 1; j++)
		{
			index = j * 2;

			point[0] = (faceNormals[i - 1][index - 1][0] + faceNormals[i - 1][index][0] + faceNormals[i - 1][index + 1][0] + faceNormals[i][index - 2][0] + faceNormals[i][index - 1][0] + faceNormals[i][index][0]) / 6;
			point[1] = (faceNormals[i - 1][index - 1][1] + faceNormals[i - 1][index][1] + faceNormals[i - 1][index + 1][1] + faceNormals[i][index - 2][1] + faceNormals[i][index - 1][1] + faceNormals[i][index][1]) / 6;
			point[2] = (faceNormals[i - 1][index - 1][2] + faceNormals[i - 1][index][2] + faceNormals[i - 1][index + 1][2] + faceNormals[i][index - 2][2] + faceNormals[i][index - 1][2] + faceNormals[i][index][2]) / 6;
			
			vertexNormals[i][j] = point;
		}

		//Last row point
		point[0] = (faceNormals[i - 1][(mapWidth * 2) - 3][0] + faceNormals[i][(mapWidth * 2) - 4][0] + faceNormals[i][(mapWidth * 2) - 3][0]) / 3;
		point[1] = (faceNormals[i - 1][(mapWidth * 2) - 3][1] + faceNormals[i][(mapWidth * 2) - 4][1] + faceNormals[i][(mapWidth * 2) - 3][1]) / 3;
		point[2] = (faceNormals[i - 1][(mapWidth * 2) - 3][2] + faceNormals[i][(mapWidth * 2) - 4][2] + faceNormals[i][(mapWidth * 2) - 3][2]) / 3;
	
		vertexNormals[i][mapWidth - 1] = point;
	}

	//Last row, first point
	point[0] = (faceNormals[mapLength - 2][0][0] + faceNormals[mapLength - 2][1][0]) / 2;
	point[1] = (faceNormals[mapLength - 2][0][1] + faceNormals[mapLength - 2][1][1]) / 2;
	point[2] = (faceNormals[mapLength - 2][0][2] + faceNormals[mapLength - 2][1][2]) / 2;

	vertexNormals[mapLength - 1][0] = point;

	//Last row, middle points
	for (int i = 1; i < mapWidth - 1; i++)
	{
		index = i * 2;

		point[0] = (faceNormals[mapLength - 2][index - 1][0] + faceNormals[mapLength - 2][index][0] + faceNormals[mapLength - 2][index + 1][0]) / 3;
		point[1] = (faceNormals[mapLength - 2][index - 1][1] + faceNormals[mapLength - 2][index][1] + faceNormals[mapLength - 2][index + 1][1]) / 3;
		point[2] = (faceNormals[mapLength - 2][index - 1][2] + faceNormals[mapLength - 2][index][2] + faceNormals[mapLength - 2][index + 1][2]) / 3;
	
		vertexNormals[mapLength - 1][i] = point;
	}

	//Last row, last point
	vertexNormals[mapLength - 1][mapWidth - 1] = faceNormals[mapLength - 2][(mapWidth * 2) - 3];

	cout << "Done...(whew!)...drawing terrain..." << endl;

	return;
}
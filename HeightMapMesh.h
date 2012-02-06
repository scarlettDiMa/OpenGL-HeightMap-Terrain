/*
 * Student: Geneva Smith (0964909)
 * Course: Software Engineering 3GC3
 * Professor: Dr. R. Teather
 * Assignment 2: Terrrain Generator
 * File: Height Mesh Class Header
 * --------------------------------------
 * This  program defines class functions that will later be implemented in the
 * HeightMapMeshimpl.cpp program.
 */

#ifndef Height_Map_Mesh_h
#define Height_Map_Mesh_h

using namespace std;

class HeightMesh
{
public:

	/*	Constructor and Destructor	*/
	HeightMesh(int x, int z);
	~HeightMesh();

	/*	Value Retrieval	*/
	int GetWidth();
	int GetLength();
	double GetMaxHeight();

	/*	Drawing	*/
	void DrawHeightMesh(bool wireframe, bool shadeflat);
	void DrawOverview();

	/*	Terrain Generation	*/
	void CircleTerrain();

private:
	#include "HeightMeshpriv.h"
};

#endif
/*
 * Geneva Smith
 * Terrrain Generator
 * File: Height Mesh Class Private Header
 * --------------------------------------
 * This  program defines all private class variables and functions. Some of these will be visible to the
 * user through functions defined in the public class header definition.
 */

#include <vector>

/*	Private HeightMapMesh class variables	*/

double **heights;

int mapWidth;  //x plane
int mapLength; //z plane

vector <double> **vertexNormals;
vector <double> **faceNormals;
int faceIndex;
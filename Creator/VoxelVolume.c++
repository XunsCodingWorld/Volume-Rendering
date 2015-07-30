#include <iostream>
#include <fstream>
#include <cstring>
#include <algorithm>

#include "VoxelVolume.h"

const cryph::AffPoint VoxelVolume::Corner(0.0, 0.0, 0.0);
const cryph::AffVector VoxelVolume::uEdge(1.0, 0.0, 0.0);
const cryph::AffVector VoxelVolume::vEdge(0.0, 1.0, 0.0);
const cryph::AffVector VoxelVolume::wEdge(0.0, 0.0, 1.0);

VoxelVolume::VoxelVolume()
{
	
}

VoxelVolume::~VoxelVolume()
{
}

void VoxelVolume::readRawFile()
{
	int i, j, k;
	i = nColsVertices;
	j = nRowsVertices;
	k = nSheetsVertices;
	std::string s = fileName;
	char *name = new char[s.length()+1];
	strcpy(name, s.c_str());
	std::ifstream img(name);
	voxelgrid = new unsigned int **[nColsVertices];
	for (int x = 0; x < nColsVertices; x++) {
		voxelgrid[x] = new unsigned int *[nRowsVertices];
		for (int y = 0; y < nRowsVertices; y++) {
			voxelgrid[x][y] = new unsigned int[nSheetsVertices];
			for (int z = 0; z < nSheetsVertices; z++) {
				unsigned char val = img.get();
				voxelgrid[x][y][z] = val;
			}
		}
	}
	delete[] name;
	setUnit();	
}

void VoxelVolume::setUnit()
{
	xColUnit = xColWCDim / (nColsVertices - 1);
	yRowUnit = yRowWCDim / (nRowsVertices - 1);
	zSheetUnit = zSheetWCDim / (nSheetsVertices - 1);
}

double VoxelVolume::getDiameter()
{
	diameter = sqrt(xColWCDim * xColWCDim + yRowWCDim * yRowWCDim + zSheetWCDim * zSheetWCDim);
	return diameter;
}

void VoxelVolume::classify(cryph::AffPoint p, int& i, int& j, int& k, double& fi, double& fj, double& fk)
{
	i = (int)p.x / xColUnit; fi = (p.x - i) / xColUnit;
	j = (int)p.y / yRowUnit; fj = (p.y - j) / yRowUnit;
	k = (int)p.z / zSheetUnit; fk = (p.z - k) / zSheetUnit;
}

double VoxelVolume::getValue(int i, int j, int k, double fi, double fj, double fk)
{
	double c00, c10, c01, c11;
	double c0, c1;
	double value;
	c00 = (1 - fi) * voxelgrid[i][j][k] + fi * voxelgrid[i + 1][j][k];
	c10 = (1 - fi) * voxelgrid[i][j + 1][k] + fi * voxelgrid[i + 1][j + 1][k];
	c01 = (1 - fi) * voxelgrid[i][j][k + 1] + fi * voxelgrid[i + 1][j][k + 1];
	c11 = (1 - fi) * voxelgrid[i][j + 1][k + 1] + fi * voxelgrid[i + 1][j + 1][k + 1];
	c0 = (1 - fj) * c00 + fj * c10;
	c1 = (1 - fj) * c01 + fj * c11;
	value = (1 - fk) * c0 + fk * c1;
	return value;
}

bool VoxelVolume::getEnterExit(const cryph::AffPoint &RayBase, const cryph::AffVector &unitRayDir, cryph::AffPoint &tEnter, cryph::AffPoint &tExit)
{
//	double t;
//	cryph::AffVector n;
	cryph::AffPoint apex(xColWCDim, yRowWCDim, zSheetWCDim);
	cryph::AffPoint enter, exit;

	if (unitRayDir.dz == -1.0)
	{
		if ((0<= RayBase.x && RayBase.x <= xColWCDim) && (0 <= RayBase.y && RayBase.y <= yRowWCDim))
		{
			enter = RayBase + (1.5 * this->getDiameter() - 0.5 * zSheetWCDim) * unitRayDir;	
			exit = RayBase + (1.5 * this->getDiameter() + 0.5 * zSheetWCDim) * unitRayDir;	
			tEnter = enter;
			tExit = exit;
			return true;
		}
		else //ray does not hit the voxelvolume
			return false;
	}
	else 
		return false;
}

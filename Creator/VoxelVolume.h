#include <string>

#include "AffPoint.h"
#include "AffVector.h"

class VoxelVolume
{
	friend class Reader;

public:

	VoxelVolume();
	~VoxelVolume();

	std::string dirName;
	double xColWCDim, yRowWCDim, zSheetWCDim;
	int nColsVertices, nRowsVertices, nSheetsVertices;
	std::string fileName;
	double diameter;
	double xColUnit, yRowUnit, zSheetUnit; //length of the voxel unit

	void readRawFile(); //read .raw files

	void classify(cryph::AffPoint p, int& i, int& j, int& k, double& fi, double& fj, double& fk);
	//if the point is not in the volume, set the indices to out of range(e.g., negative) values.
	double getValue(int i, int j, int k, double fi, double fj, double fk);
	bool getEnterExit(const cryph::AffPoint &RayBase, const cryph::AffVector &unitRayDir, cryph::AffPoint &tEnter, cryph::AffPoint &tExit);
	//if the ray misses the volume,  return false; otherwise return true

	double getDiameter();
	void setUnit();

private:
	
	static const cryph::AffPoint Corner;
	static const cryph::AffVector uEdge, vEdge, wEdge;

	unsigned int ***voxelgrid; //voxelgrid which stores the values of the voxel read from the raw file
};

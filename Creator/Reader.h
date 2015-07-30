#include <string>

#include"VoxelVolume.h"

class Reader
{

public:

	Reader();
	~Reader();

	void readFromFile(char *inFile, VoxelVolume *voxel); //read input file

	double r, g, b;
	double eyeX, eyeY, eyeZ;
	double centerX, centerY, centerZ;
	double upX, upY, upZ;
	double PPwidth, PPheight;
	int xRes, yRes; //number of pixels in the x (horizontal) and y (vertical) directions
	std::string mode;
	double norMin, norMax;
	double stepSize;
	VoxelVolume voxelInfo; //a voxelvolume stack stores the voxel information read from the file
	
	int flagBG, flagE, flagC, flagUp, flagPP, flagRes, flagMode, flagSS;
	
private:

};


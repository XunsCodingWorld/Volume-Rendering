#include <iostream>
#include <fstream>
#include <sstream>

#include"Reader.h"

Reader::Reader(): flagBG(0), flagE(0), flagC(0), flagUp(0), flagPP(0), flagRes(0), flagMode(0), flagSS(0)
{
	
}

Reader::~Reader()
{
}

void Reader::readFromFile(char *inFile, VoxelVolume *voxel)
{
	std::ifstream is(inFile);
	std::string line;
	
	while (is.good())
	{
		std::getline(is, line, ' ');
		if (line == "background")
		{
			std::getline(is, line);
			std::stringstream iss(line);
			iss >> r >> g >> b;
			flagBG = 1;
			continue;
		}
		if (line == "eye")
		{
			std::getline(is, line);
			std::stringstream iss(line);
			iss >> eyeX >> eyeY >> eyeZ;
			flagE = 1;
			continue;
		}
		
		if (line == "center")
		{
			std::getline(is, line);
			std::stringstream iss(line);
			iss >> centerX >> centerY >> centerZ;
			flagC = 1;
			continue;
		}
		if (line == "up")
		{
			std::getline(is, line);
			std::stringstream iss(line);
			iss >> upX >> upY >> upZ;
			flagUp = 1;
			continue;
		}
		if (line == "PPWindowSize")
		{
			std::getline(is, line);
			std::stringstream iss(line);
			iss >> PPwidth >> PPheight;
			flagPP = 1;
			continue;
		}
		if (line == "resolution")
		{
			std::getline(is, line);
			std::stringstream iss(line);
			iss >> xRes >> yRes;
			flagRes = 1;
			continue;
		}
		if (line == "rayProcessingSpecs")
		{
			std::getline(is, mode, ' ');
			if(mode == "AVERAGE")
			{
				std::getline(is, line, ' ');
				std::stringstream iss(line);
				iss >> norMin;
				std::getline(is, line, ' ');
				std::stringstream isss(line);
				isss >> norMax;
			}
			flagMode = 1;
			std::getline(is, line, ' ');
			if (line == "STEPSIZE")
			{
				std::getline(is, line, '$');
				std::stringstream iss(line);
				iss >> stepSize;
			}
			std::getline(is, line);
			flagSS = 1;
			continue;
		}
		if (line == "xyzVoxelVertices")
		{
			std::getline(is, line);
			std::stringstream iss(line);
			iss 
			>> voxelInfo.xColWCDim >> voxelInfo.yRowWCDim >> voxelInfo.zSheetWCDim
			>> voxelInfo.nColsVertices >> voxelInfo.nRowsVertices >> voxelInfo.nSheetsVertices
			>> voxelInfo.fileName;
			continue;
		}
		else break;
	}
	std::cout << "Input file information:" << std::endl;
	if (flagBG == 1)	std::cout << "background -> " << r << ' ' << g << ' ' << b << std::endl;
	else	std::cout << "background ->  parameter not defined, using default" << std::endl;
	if (flagE == 1)	std::cout << "eye -> " << eyeX << ' ' << eyeY << ' ' << eyeZ << std::endl;
	else	std::cout << "eye ->  parameter not defined, using default" << std::endl;
	if (flagC == 1)	std::cout << "center -> " << centerX << ' ' << centerY << ' ' << centerZ << std::endl;
	else	std::cout << "center ->  parameter not defined, using default" << std::endl;
	if (flagUp == 1)	std::cout << "up -> " << upX << ' ' << upY << ' ' << upZ << std::endl;
	else 	std::cout << "up ->  parameter not defined, using default" << std::endl;
	if (flagPP == 1)	std::cout << "PPSize -> " << PPwidth << ' ' << PPheight << std::endl;
	else 	std::cout << "PPWindowSize ->  parameter not defined, using default" << std::endl;
	if (flagRes == 1)	std::cout << "resolution -> " << xRes << ' ' << yRes << std::endl;
	else	std::cout << "resolution ->  parameter not defined, using default" << std::endl;
	if (flagMode == 1 && flagSS == 1)	std::cout << "ray mode -> " << mode << ";" << "tracing stepsize -> " << stepSize << std::endl;
	else	std::cout << "rayProcessingSpecs ->  parameter not defined, using default" << std::endl;
	std::cout << "voxelvolume infor -> " 
	<< voxelInfo.xColWCDim << ' ' << voxelInfo.yRowWCDim << ' ' << voxelInfo.zSheetWCDim
	<< ' ' << voxelInfo.nColsVertices << ' ' << voxelInfo.nRowsVertices << ' ' << voxelInfo.nSheetsVertices
	<< ' ' << voxelInfo.fileName << std::endl;

	*voxel = voxelInfo;
	
	is.close();
}

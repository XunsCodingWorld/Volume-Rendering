#include <iostream>
#include <fstream>
#include <cstring>
#include <algorithm>
#include <cstdlib>

#include "Matrix4x4.h"
#include "Reader.h"
#include "ImageWriter.h"

cryph::AffPoint trace(const cryph::AffPoint &rayorig, const cryph::AffVector &raydir, Reader rd,
	 VoxelVolume *voxel, const int option)
{
	double t;
	cryph::AffPoint t0, t1;
	cryph::AffPoint curPoint;
	int count(0);
	double avg(0), sum(0);
	int i, j, k;
	double fi, fj, fk;
	cryph::AffPoint surfaceColor;
	cryph::AffPoint backColor;
	double step;
	//********************************************************************
	if (rd.flagBG == 1)	backColor = cryph::AffPoint(rd.r, rd.g, rd.b);
	else backColor = cryph::AffPoint(1.0, 1.0, 1.0);
	if (rd.flagSS == 1)	step = rd.stepSize;
	else step = 1.0;
	//********************************************************************
	if (option == 0)	//Average mode
	{
		double r, g, b; //RGB
		double avg(0), sum(0);
		// find intersection of this ray with the box in the scene
		if (voxel->getEnterExit(rayorig, raydir, t0, t1))
		{
			for (t = t1.z; t < t0.z; t+=step)
			{
				curPoint = t0 + t * raydir;
				voxel->classify(curPoint, i, j, k, fi, fj, fk);
				sum += voxel->getValue(i, j, k, fi, fj, fk);
				count++;
			}
			avg = sum / count;
			// transfer function
			if (avg < 15)
			{
				surfaceColor = cryph::AffPoint(0.0, 0.0, 0.0);
			}
			else if (avg >= 15 && avg < 25)
			{
				r = 0.04 * avg - 0.6;
				g = 0.04 * avg - 0.6;
				b = 0.04 * avg - 0.6;
				surfaceColor = cryph::AffPoint(r, g, b);
			}
			else if (avg >= 25 && avg < 35)
			{
				r = 0.01 * avg + 0.05;
				g = 0.06 * avg - 1.5;
				b = 0.0;
				surfaceColor = cryph::AffPoint(r, g, b);
			}
			else if (avg >= 35 && avg < 55)
			{
				r = 0.02 * avg - 0.3;
				g = 0.0;
				b = 0.02 * avg - 0.3;
				surfaceColor = cryph::AffPoint(r, g, b);
			}
			else if (avg >= 55 && avg < 255)
			{
				r = 0.005 * avg - 0.275;
				g = 0.005 * avg - 0.275;
				b = 0.005 * avg - 0.275;
				surfaceColor = cryph::AffPoint(r, g, b);
			}
			else if (avg >= 255)
			{
				surfaceColor = cryph::AffPoint(1.0, 1.0, 1.0);
			}
		}
		// if there's no intersection return set it to background color
		else  
			surfaceColor = backColor;
	}
	//*******************************************************************
	else if (option == 1)	//AccumRGB mode
	{
		double voxelvalue;
		double accumR(0), accumG(0), accumB(0), accumA(0);
		double curR, curG, curB, curA;
		// find intersection of this ray with the box in the scene
		if (voxel->getEnterExit(rayorig, raydir, t0, t1))
		{
			for (t = t1.z; t < t0.z; t+=step)
			{
				if (accumA > 1 || accumR > 1 || accumG > 1 || accumB > 1)
				{
					surfaceColor = backColor;
					break;
				}
				curPoint = t0 + t * raydir;
				voxel->classify(curPoint, i, j, k, fi, fj, fk);
				voxelvalue = voxel->getValue(i, j, k, fi, fj, fk);
				if (voxelvalue < 15)
				{
					curR = 0.0;
					curG = 0.0;
					curB = 0.0;
					curA = 0.0;
				}
				else if (voxelvalue >= 15 && voxelvalue < 25)
				{
					curR = 0.04 * voxelvalue - 0.6;
					curG = 0.04 * voxelvalue - 0.6;
					curB = 0.04 * voxelvalue - 0.6;
					curA = 0.06 * voxelvalue - 0.9;
				}
				else if (voxelvalue >= 25 && voxelvalue < 35)
				{
					curR = 0.01 * voxelvalue + 0.05;
					curG = 0.06 * voxelvalue - 1.5;
					curB = 0.0;
					curA = 0.03 * voxelvalue - 0.45;
				}
				else if (voxelvalue >= 35 && voxelvalue < 55)
				{
					curR = 0.02 * voxelvalue - 0.3;
					curG = 0.0;
					curB = 0.02 * voxelvalue - 0.3;
					curA = 0.02 * voxelvalue - 0.3;
				}
				else if (voxelvalue >= 55 && voxelvalue < 255)
				{
					curR = 0.005 * voxelvalue - 0.275;
					curG = 0.005 * voxelvalue - 0.275;
					curB = 0.005 * voxelvalue - 0.275;
					curA = 0.005 * voxelvalue - 0.275;
				}
				else if (voxelvalue >= 255)
				{
					curR = 1.0;
					curG = 1.0;
					curB = 1.0;
					curA = 1.0;
				}
				accumR += curA * curR * (1 - accumA);
				accumG += curA * curG * (1 - accumA);
				accumB += curA * curB * (1 - accumA);
				accumA += curA * (1 - accumA);
			}
			surfaceColor = cryph::AffPoint(accumR, accumG, accumB);
		}
		// if there's no intersection return set it to background color
		else
			surfaceColor = backColor;
	}
	//*******************************************************************
	else if (option == 2) //Max mode
	{
		double r, g, b; //RGB
		double max(0);
		double voxelvalue;
		// find intersection of this ray with the box in the scene
		if (voxel->getEnterExit(rayorig, raydir, t0, t1))
		{
			for (t = t1.z; t < t0.z; t+=step)
			{
				curPoint = t0 + t * raydir;
				voxel->classify(curPoint, i, j, k, fi, fj, fk);
				voxelvalue = voxel->getValue(i, j, k, fi, fj, fk);
			if (voxelvalue > max) max = voxelvalue;
			}
			// transfer function
			if (max < 15)
			{
				surfaceColor = cryph::AffPoint(0.0, 0.0, 0.0);
			}
			else if (max >= 15 && max < 25)
			{
				r = 0.04 * max - 0.6;
				g = 0.04 * max - 0.6;
				b = 0.04 * max - 0.6;
				surfaceColor = cryph::AffPoint(r, g, b);
			}
			else if (max >= 25 && max < 35)
			{
				r = 0.01 * max + 0.05;
				g = 0.06 * max - 1.5;
				b = 0.0;
				surfaceColor = cryph::AffPoint(r, g, b);
			}
			else if (max >= 35 && max < 55)
			{
				r = 0.02 * max - 0.3;
				g = 0.0;
				b = 0.02 * max - 0.3;
				surfaceColor = cryph::AffPoint(r, g, b);
			}
			else if (max >= 55 && max < 255)
			{
				r = 0.005 * max - 0.275;
				g = 0.005 * max - 0.275;
				b = 0.005 * max - 0.275;
				surfaceColor = cryph::AffPoint(r, g, b);
			}
			else if (max >= 255)
			{
				surfaceColor = cryph::AffPoint(1.0, 1.0, 1.0);
			}
		}
		// if there's no intersection return set it to background color
		else
		surfaceColor = backColor;
	}
	return surfaceColor;
}

void render(const std::string& fileName,
	VoxelVolume *voxel,
	Reader rd,
	cryph::AffPoint projectionPlaneZ,
	cryph::AffVector u, cryph::AffVector v, cryph::AffVector w,
	int option)
{
	int xRes, yRes;
	double xResUnit, yResUnit;
	//********************************************************************************
	if(rd.flagRes == 1){
		xRes = rd.xRes;
		yRes = rd.yRes;
	}
	else{
		xRes = 256;
		yRes = 256;
	}
	if(rd.flagPP == 1){
		xResUnit = rd.PPwidth / rd.xRes;
		yResUnit = rd.PPheight / rd.yRes;
	}
	else{
		xResUnit = voxel->getDiameter() / xRes;
		yResUnit = voxel->getDiameter() / yRes;
	}
	//********************************************************************************

	cryph::AffPoint *pixel = new cryph::AffPoint[xRes * yRes];
	cryph::AffPoint curPixel;
	
	// Trace rays
	for (int y = 0; y < yRes; y++) {
		for (int x = 0; x < xRes; x++) {
			curPixel = projectionPlaneZ + y * xResUnit * v + x * yResUnit * u;
			cryph::AffVector raydir = - w;
			raydir.normalize();
			pixel[x+y*xRes] = trace(curPixel, raydir, rd, voxel, option);
		}
	}
	//create the image base on the filename extentione
	unsigned char *row = new unsigned char[3*xRes];
	ImageWriter *image = image->create(fileName, xRes, yRes);
	
	for(int i = 0; i < yRes; i++)
	{
		for(int j = 0; j < xRes; j++)
		{
			row[j*3] = 255 * pixel[j + i * xRes].x;
			row[j*3+1] = 255 * pixel[j + i * xRes].y; 
			row[j*3+2] = 255 * pixel[j + i * xRes].z;
		}
		image->addScanLine(row);
	}
	image->closeImageFile();
}

int main(int argc, char *argv[])
{
	if (argc != 3)
	{
		std::cerr << "Usage: " << std::endl;
		return -1;
	}
	
	Reader rd; //reader instance
	VoxelVolume voxel;
	double width, height, length; // width, height and length (measured in world coordinate units) of the voxelvolume
	double diameter;	
	rd.readFromFile(argv[1], &voxel);	
	
	diameter = voxel.getDiameter();

	voxel.readRawFile(); //read .raw file

	//******************************************************************************
	double halfPPwidth, halfPPheight;

	double xmid = 0.5 * voxel.xColWCDim;
	double ymid = 0.5 * voxel.yRowWCDim;
	double zmid = 0.5 * voxel.zSheetWCDim;
	
	if(rd.flagPP == 1){
		halfPPwidth = 0.5 * rd.PPwidth;
		halfPPheight = 0.5 * rd.PPheight;
	}
	else{
		halfPPwidth = 0.5 * diameter;
		halfPPheight = 0.5 * diameter;
	}
	//******************************************************************************

	double distEyeCenter = diameter;
	double halfdiameter = 0.5 * diameter;
	// arrange the eye and center points
	cryph::AffPoint center(xmid, ymid, zmid);
	cryph::AffPoint eye(xmid, ymid, zmid + distEyeCenter);
	cryph::AffVector up = cryph::AffVector::yu;
	cryph::AffVector u, v, w; //three vectors on camera basis

	if (cryph::Matrix4x4::getECvw(eye, center, up, v, w))
	{
		u = v.cross(w);
		std::cout << "Set up enviroment successfully" << std::endl;
		std::cout << "Wait for the image to be created, will notify you if it is done!" << std::endl;
	}
	// place the projection plane at e3Hat center.
	cryph::AffPoint projectionPlaneZ = eye + halfdiameter * w - halfPPwidth * u - halfPPheight * v;

	//*******************************************************************************
	int opt;
	if(rd.flagMode == 1){	
		if(rd.mode == "AVERAGE") opt = 0;
		else if (rd.mode == "ACCUMRGB_ADD_BACKGROUND") opt = 1;
		else if (rd.mode == "MAX") opt = 2;
	}
	else opt = 2;
	render(argv[2], &voxel, rd, projectionPlaneZ, u, v, w, opt);

	std::cout << "Find the image in the directory" << std::endl;
	return 0;
}


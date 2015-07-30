// testpattern.c++ -- create an image using the ImageWriter class
//                    Execute as: testpattern fName xres yres
//                    'testpattern' must end in a recognized image
//                    file suffix, currently one of: png, jpg, or jpeg.
// Example:
// testpattern mypattern.png 512 512

#include <iostream>
#include <cstdlib>
using namespace std;

#include "ImageWriter.h"

void writeTheFile(std::string fName, int xres, int yres)
{
	unsigned char* row = new unsigned char[3*xres];
	ImageWriter* w = ImageWriter::create(fName,xres,yres);
	for (int i=0 ; i<yres ; i++)
	{
		for (int j=0 ; j<3*xres ; j+=3)
		{
			bool firstHalf = (j < (3*xres/2));
			if (i < 10)
				if (firstHalf)
				{
					row[j] = 255; row[j+1] = 0; row[j+2] = 0;
				}
				else
				{
					row[j] = 0; row[j+1] = 255; row[j+2] = 0;
				}
			else if (i < 20)
				if (firstHalf)
				{
					row[j] = 255; row[j+1] = 255; row[j+2] = 0;
				}
				else
				{
					row[j] = 0; row[j+1] = 255; row[j+2] = 255;
				}
			else
				if (firstHalf)
				{
					row[j] = 0; row[j+1] = 0; row[j+2] = 255;
				}
				else
				{
					row[j] = 255; row[j+1] = 0; row[j+2] = 255;
				}
		}
		w->addScanLine(row);
	}
	w->closeImageFile();
}

int main(int argc, char* argv[])
{
	if (argc < 4)
		cerr << "Usage: " << argv[0] << "fName xres yres\n";
	else
	{
		int xres = atoi(argv[2]);
		int yres = atoi(argv[3]);
		writeTheFile(argv[1], xres, yres);
	}
	return 0;
}

// PNGImageWriter.c++ -- Simple class to build png files

#include <iostream>
#include <fstream>
using namespace std;

#include "PNGImageWriter.h"

PNGImageWriter::PNGImageWriter(string fName, int width, int height, int nChannels) :
	ImageWriter(fName,width,height,nChannels), fp(NULL), png_ptr(NULL), info_ptr(NULL),
	theImage(NULL), row_pointers(NULL), nextScanLine(0)
{
	fp = fopen(fName.c_str(), "wb");
	if (fp == NULL)
	{
		cerr << "Could not open " << fName << " for writing.\n";
		return;
	}
	png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (png_ptr == NULL)
	{
		cerr << "Could not create png_ptr\n";
		return;
	}
	info_ptr = png_create_info_struct(png_ptr);
	if (info_ptr == NULL)
	{
		png_destroy_write_struct(&png_ptr, NULL);
		png_ptr = NULL;
		cerr << "Could not create png_info\n";
		return;
	}
	png_init_io(png_ptr, fp);
	// specify the header information
	const int bitDepth = 8; // for now... (see also uses of "255" and "unsigned char")
	int color_type = PNG_COLOR_TYPE_RGB;
	if (nChannels == 4)
		color_type = PNG_COLOR_TYPE_RGB_ALPHA;
	png_set_IHDR(png_ptr, info_ptr, width, height, bitDepth, color_type,
				 PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT,
				 PNG_FILTER_TYPE_DEFAULT);
	// need to flip order of the rows:
	theImage = new cryph::Packed3DArray<unsigned char>(height, width, nChannels);
	unsigned char* p = theImage->getModifiableData();
	row_pointers = new png_byte*[height];
	int rpi = height;
	for (int i=0 ; i<height ; i++)
		row_pointers[--rpi] = p + i*width*nChannels*sizeof(unsigned char);
	png_set_rows(png_ptr, info_ptr, row_pointers);
}

PNGImageWriter::PNGImageWriter(const PNGImageWriter& rte) :
	ImageWriter("",0,0), fp(NULL), png_ptr(NULL), info_ptr(NULL),
	theImage(NULL), row_pointers(NULL)
{
	// disallowed!
}

PNGImageWriter::~PNGImageWriter()
{
	closeImageFile();
	if (row_pointers != NULL)
		delete [] row_pointers;
	if (theImage != NULL)
		delete theImage;
}

void PNGImageWriter::addScanLine(const double* sLine)
{
	if (theImage == NULL)
		return;
	int pos = 0;
	for (int col=0 ; col<mXRes ; col++)
		for (int channel=0 ; channel<mNumChannels ; channel++)
		{
			unsigned char x = static_cast<unsigned char>(sLine[pos++]*255.0 + 0.5);
			theImage->setDataElement(nextScanLine, col, channel, x);
		}
	nextScanLine++;
}

void PNGImageWriter::addScanLine(const unsigned char* sLine)
{
	if (theImage == NULL)
		return;
	int pos = 0;
	for (int col=0 ; col<mXRes ; col++)
		for (int channel=0 ; channel<mNumChannels ; channel++)
			theImage->setDataElement(nextScanLine, col, channel, sLine[pos++]);
	nextScanLine++;
}

void PNGImageWriter::closeImageFile()
{
	if (fp != NULL)
	{
		// first write the buffered-up file, then close it.
		const int TRANSFORM_BIT_MASK = 0;
		png_write_png(png_ptr, info_ptr, TRANSFORM_BIT_MASK, NULL);
		png_write_end(png_ptr, info_ptr);
		fclose(fp);
		fp = NULL;
		png_destroy_write_struct(&png_ptr, &info_ptr);
	}
}

bool PNGImageWriter::openImageFile()
{
}

void PNGImageWriter::writeImage(const unsigned char* fb)
{
	if (theImage == NULL)
		return;
	int pos = 0;
	for (int row=0 ; row<mYRes ; row++)
		for (int col=0 ; col<mXRes ; col++)
			for (int channel=0 ; channel<mNumChannels ; channel++)
				theImage->setDataElement(row, col, channel, fb[pos++]);
}

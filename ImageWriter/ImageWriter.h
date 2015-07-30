// ImageWriter.h -- Abstract Base Class for writing image files

#ifndef IMAGEWRITER_H
#define IMAGEWRITER_H

#include <string>

class ImageWriter
{
public:
	virtual ~ImageWriter();

	virtual void	closeImageFile() = 0;

	// Results are undefined if you try to mix line-by-line writing
	// with "write all at once"!

	// Image files can be written EITHER scan line by scan line
	// using addScanLine....
	virtual void	addScanLine(const double* sLine) = 0;
	virtual void	addScanLine(const unsigned char* sLine) = 0;

	// ....OR all at once using:
	virtual void	writeImage(const unsigned char* fb) = 0;

	// A factory method that will create an ImageWriter based on the file name suffix
	static ImageWriter* create(std::string fileName, int xres, int yres, int numChannels=3);

protected:
	ImageWriter(std::string fName, int xres, int yres, int numChannels=3);
	ImageWriter(const ImageWriter& iw); // disallow copy constructor

	static ImageWriter* guessFileType(const std::string& fileName, int xres, int yres, int numChannels);

	std::string			mImageFileName;
	int					mXRes;
	int					mYRes;
	int					mNumChannels;
};

#endif

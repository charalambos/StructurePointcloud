////////////////////////////////////////////////////////////////////////////////////
// Copyright © Charalambos "Charis" Poullis, charalambos@poullis.org    	  //
// This work can only be used under an exclusive license of the author.           //
////////////////////////////////////////////////////////////////////////////////////
#ifndef __IMAGE_H__
#define __IMAGE_H__


//#include <opencv2/opencv.hpp>
//using namespace cv;

#include <ostream>
#include <float.h>
#include "Color.h"
#include "Magick++.h"
#include "FileUtilities.h"

#define REVERSE2BYTES(source, dest)		\
{										\
	char *j = (char *) source;			\
	char *dj = (char *) dest;			\
	dj[0] = j[1];						\
	dj[1] = j[0];						\
}


#define REVERSEBYTES(source, dest)		\
{										\
	char *j = (char *) source;			\
	char *dj = (char *) dest;			\
	dj[0] = j[3];						\
	dj[1] = j[2];						\
	dj[2] = j[1];						\
	dj[3] = j[0];						\
}

#define REVERSE2BYTES_INPLACE(sourcedest)		\
{												\
	char *j = (char *) sourcedest;				\
	j[0] ^= j[1]; j[1] ^= j[0]; j[0] ^= j[1];	\
}

#define REVERSEBYTES_INPLACE(sourcedest)		\
{												\
	char *j = (char *) sourcedest;				\
	j[0] ^= j[3]; j[3] ^= j[0]; j[0] ^= j[3];	\
	j[1] ^= j[2]; j[2] ^= j[1]; j[1] ^= j[2];	\
}

class Image	{
	public:
		///Constructor
		Image();
		Image(Image const &img);
		Image(int _width, int _height);
		Image(int _width, int _height, float r, float g, float b, float a);

		///Destructor
		~Image();

		///Returns true if the index (x,y) is outside the image boundaries; false otherwise
		friend bool outOfBounds(Image *img, int x, int y, int offset_x=0, int offset_y=0, int error_code=-1);

		///Load image
		bool loadImage(std::string const &_file_name);
		///Save image
		bool saveImage(std::string const &_file_name);

		///Returns the width of the image
		int getWidth() const;
		///Returns the height of the image
		int getHeight() const;

		///Returns the pixel value at the specified location
		Color getPixel(int x, int y) const;
		Color getPixelBilinear(float sx, float sy);
		Color getPixelBicubic(float sx, float sy);
		///Returns the corresponding grayscale value for an RGB pixel. If an index is given the particular channel is used for all channels.
		float getPixelGrayscale(int x, int y, int index=-1);


		///Sets the pixel value at the specified location
		void setPixel(int x, int y, Color const &color);

		///Blurs the image
		void blur(float variance);

		///Returns the image file name
		std::string getFileName();
		///Returns the file type
		FILE_TYPE getFileType() const;
		///Sets the file type
		void setFileType(FILE_TYPE _type);

		///Normalizes the image
		void normalize(float _range=1.0f);
		///Normalizes the image in a specific range
		void normalize(Color *_range);
		///Per pixel normalization i.e. each RGB is normalized to 1
		void perPixelNormalization(float _range=1.0f);

		///Clear the image to the given color
		void clear(Color const &val);
		void clear(float _r, float _g, float _b);

		///Adds another image to the existing one provided they are the same size - per pixel addition
		void add(Image *_img);

		///Adds to pixel the given color value
		void add(int x, int y, Color const &val);

		///Copies an image to the existing data
		void copy(Image *_img);

		///Scales the images by a factors of f
		void scale(float f);

		///Halfsize the image
		void halfSize();

		///Doublesize the image
		void doubleSize();
		void doubleSizeBilinear();

		///Makes the image binary
		void binary(Color const &threshold = Color(COLOR_EPSILON, COLOR_EPSILON, COLOR_EPSILON));

		///Normalizes over a set of images i.e. the min and max values are for the entire set
		friend void normalizeSet(std::vector<Image *> &images, float range=1.0f, bool ignore_value=false, Color value=Color(-1.0f));

		///Converts to grayscale
		void grayscale(int index=-1);
		///Converts to XYZ color space
		void RGBtoXYZ();
		///Converts to Luv color space
		void RGBtoLUV();
		///Converts to Lab color space
		void RGBtoLab();

		///Returns the range of the image
		Color *range();

		typedef enum IMAGE_MATH_OP {
			IMAGE_MATH_OP_ADD,
			IMAGE_MATH_OP_SUBTRACT,
			IMAGE_MATH_OP_MULTIPLY,
			IMAGE_MATH_OP_DIVIDE
		} IMAGE_MATH_OP;

		///Math operations
		Image *mathOp(Image *img, IMAGE_MATH_OP const &type);
		Image *negate();

	private:
		///The width of the image
		unsigned int width;
		///The height of the image
		unsigned int height;
		///File name
		std::string file_name;
		///File type
		FILE_TYPE type;

		///The image data
		Color **data;
		///Magick image
		//Magick::Image data;
		///The gamma table
		static float gamma_table[65536];
		///The lookup table. Used when converting from HDR to LDR
		static unsigned short lookup_table[65536];
		///The current gamma
		static float current_gamma;

		///Initialize the gamma table
		void initializeGammaTable(float gamma);
		///Initialize the lookup table
		void initializeLookupTable();

		///Image loaders
		bool loadJPG(std::string const &file_name);
		bool loadPNG(std::string const &file_name);
		bool loadTGA(std::string const &file_name);
		bool loadPFM(std::string const &file_name);
		bool loadTIF(std::string const &file_name);
		bool loadBMP(std::string const &file_name);

		///Image exporters
		bool saveJPG(std::string const &file_name);
		bool savePNG(std::string const &file_name);
		bool saveTGA(std::string const &file_name);
		bool savePFM(std::string const &file_name);
		bool saveTIF(std::string const &file_name);
		bool saveBMP(std::string const &file_name);

};

#endif

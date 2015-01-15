////////////////////////////////////////////////////////////////////////////////////
// Copyright © Charalambos "Charis" Poullis, charalambos@poullis.org    	      //
// This work can only be used under an exclusive license of the author.           //
////////////////////////////////////////////////////////////////////////////////////

#ifndef __IMAGE_PROCESSING_H__
#define __IMAGE_PROCESSING_H__

#include "fftw3.h"

///Bilateral filtering
#include <cmath>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
//#define CHRONO
#include "geom.h"
#include "fast_color_bf.h"
#include "linear_bf.h"

///Forward declarations
class Image;


class ImageProcessing	{
	public:
		///Hole filling
		static void holeFill(Image *input_image, int max_neighbourhood_size = 6, int min_neighbourhood_size = 2, bool dominant_neighbour=false);
		static Image *holeFill2(Image *input_image, int max_neighbourhood_size = 6, int min_neighbourhood_size = 2, bool dominant_neighbour=false);
		static void pointReduction(Image *non_hole_filled_map, Image *hole_filled_map, int radius);

		///Convolution
		static Image *convolve(Image *input_image, Image *kernel);

		///Discrete Fourier Transform and inverse
		static fftw_complex *dft(Image *input_image, int channel, bool padding = false, int width = -1, int height = -1);
		static fftw_complex *idft(fftw_complex *input_image, int width, int height, Image *existing_image=0x00);

		///Bilateral filtering
		static Image *bilateralFilteringGrayscale(Image *grayscale_image, double sigma_s = 16.0, double sampling_s=0.1, double sigma_r = 0.1, double sampling_r=0.1 );
	private:
		///Consructor
		ImageProcessing();
		///Destructor
		~ImageProcessing();

		///Helper function for fftw
		static Image *copy(fftw_complex *data, int channel, int width, int height, Image *existing_image=0x00);
		static fftw_complex *copy(Image *data, int channel, bool padding=false, int width = -1, int height = -1);

		static Image *padImage(Image *input_image, int width, int height);
		static Image *unpadImage(Image *input_image, int width, int height);

};

#endif

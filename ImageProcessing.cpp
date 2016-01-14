////////////////////////////////////////////////////////////////////////////////////
// Copyright © Charalambos "Charis" Poullis, charalambos@poullis.org    	  //
// This work can only be used under an exclusive license of the author.           //
////////////////////////////////////////////////////////////////////////////////////

#ifndef __IMAGE_PROCESSING_CPP__
#define __IMAGE_PROCESSING_CPP__

#include "ImageProcessing.h"
#include "Image.h"
#include "Color.h"

ImageProcessing::ImageProcessing()	{

}


ImageProcessing::~ImageProcessing()	{

}

void ImageProcessing::holeFill(Image *input_image, int max_neighbourhood_size, int min_neighbourhood_size, bool dominant_neighbour)	{

	Image *result = holeFill2(input_image, max_neighbourhood_size, min_neighbourhood_size, dominant_neighbour);
	input_image->copy(result);
	delete result;
	return;
}

Image *ImageProcessing::holeFill2(Image *input_image, int max_neighbourhood_size, int min_neighbourhood_size, bool dominant_neighbour)	{


	Image *img = new Image(*input_image);
	bool have_holes = true;
	int neighbourhood_size = max_neighbourhood_size;
	while (have_holes)	{
		have_holes = false;
		///go through the image and find an empty pixel
		for (int y=0;y<img->getHeight();y++)	{
			for (int x=0;x<img->getWidth();x++)	{
				///if the pixel is valid then no need to process it
				if (img->getPixel(x,y) != Color(0.0f))	continue;
				///otherwise compute a value from the local neighbourhood
				Vector3f avg_value = Vector3f(0.0f,0.0f,0.0f);
				std::vector<Vector3f> neighbour_values;
				std::vector<int> neighbour_votes;
				int number_of_valid_neighbours = 0;
				for (int i=y-1;i<=y+1;i++)	{
					for (int j=x-1;j<=x+1;j++)	{
						///check if its out of bounds
						if (outOfBounds(img,j,i))	continue;
						///if its the pixel in question continue
						if (i==y && j==x)	continue;
						///if the pixel is empty don't count it
						if (img->getPixel(j,i)==Color(0.0f))	continue;
						///if its substituting by the most dominant neighbour then keep track of the votes
						if (dominant_neighbour)	{
							Vector3f neighbour_value = color2vector3(img->getPixel(j,i));
							///first check if the value is already there
							bool already_there = false;
							int pos = -1;
							for (int k=0;k<neighbour_values.size();k++)	{
								if (neighbour_values[k] == neighbour_value)	{
									already_there = true;
									pos = k;
									break;
								}
							}
							///if its already there then increase its votes by 1
							if (already_there)	{
								neighbour_votes[pos]++;
							}
							else	{
								///otherwise add it to the neighbour values
								neighbour_values.push_back(color2vector3(img->getPixel(j,i)));
								///add a single vote
								neighbour_votes.push_back(1);
							}

						}
						number_of_valid_neighbours++;
						avg_value += color2vector3(img->getPixel(j,i));
					}
				}
				///if there are more than 6 neighbours then fill in the value
				if (number_of_valid_neighbours < neighbourhood_size)	continue;
				if (number_of_valid_neighbours) avg_value /= float(number_of_valid_neighbours);

				///if its setting the dominant neighbour then find the one with max votes
				if (dominant_neighbour)	{
					int max_votes = 0;
					int pos = -1;
					for (int k=0;k<neighbour_votes.size();k++)	{
						if (neighbour_votes[k] > max_votes)	{
							max_votes = neighbour_votes[k];
							pos = k;
						}
					}
					///set the right value
					img->setPixel(x,y,vector2color3(neighbour_values[pos]));
				}
				else	{
					img->setPixel(x,y,vector2color3(avg_value));
				}
				have_holes = true;
			}
		}

		///if there are no changes and its above the min threshold then do it again with a smaller neighbourhood size
		if ((!have_holes) && (neighbourhood_size > min_neighbourhood_size))	{
			neighbourhood_size-=2;
			have_holes = true;
		}
	}

	return img;
}

Image *ImageProcessing::bilateralFilteringGrayscale(Image *grayscale_image, double sigma_s, double sampling_s, double sigma_r, double sampling_r)	{

	typedef Array_2D<double> image_type;

	///Convert to grayscale
	image_type grayscale_image_2d(grayscale_image->getWidth(), grayscale_image->getHeight());
	for (int y=0;y<grayscale_image->getHeight();y++)	{
		for (int x=0;x<grayscale_image->getWidth();x++)	{
			grayscale_image_2d(x,y) = double(grayscale_image->getPixel(x,y).r());
		}
	}

	///Run the Bilateral filtering
	image_type filtered_image_2d(grayscale_image->getWidth(),grayscale_image->getHeight());

	FFT::Support_3D::set_fftw_flags(FFTW_ESTIMATE);
	Image_filter::linear_BF(grayscale_image_2d,
				  sigma_s,sigma_r,
				  sampling_s,sampling_r,
				  &filtered_image_2d);
	///Convert back to regular image format
	Image *filtered_image = new Image(grayscale_image->getWidth() , grayscale_image->getHeight(), 0.0f,0.0f,0.0f,1.0f);
	for (int y=0;y<filtered_image->getHeight();y++)	{
		for (int x=0;x<filtered_image->getWidth();x++)	{
				filtered_image->setPixel(x,y,Color(float(filtered_image_2d(x,y))));
		}
	}

	return filtered_image;
}


void ImageProcessing::pointReduction(Image *non_hole_filled_map, Image *hole_filled_map, int radius)	{

	Image *markup_image = new Image(non_hole_filled_map->getWidth(), non_hole_filled_map->getHeight(), 0.0f, 0.0f, 0.0f, 1.0f);
	///Go through all the valid points in the non-hole-filled image and mark a neighbourhood of radius around them.
	for (int y=0;y<non_hole_filled_map->getHeight();y++)	{
		for (int x=0;x<non_hole_filled_map->getWidth();x++)	{
			///If there is no point then ignore
			if (non_hole_filled_map->getPixel(x,y) == Color(0.0f,0.0f,0.0f))	continue;
			///Otherwise mark a neighbourhood of radius around it
			for (int i=-radius/2;i<radius/2;i++)	{
				for (int j=-radius/2;j<radius/2;j++)	{
					if (outOfBounds(markup_image, x+j, y+i))	continue;
					markup_image->setPixel(x+j, y+i, Color(1.0f,1.0f,1.0f));
				}
			}
		}
	}

	///Go through the hole-filled image and keep only the points in the markup image
	for (int y=0;y<hole_filled_map->getHeight();y++)	{
		for (int x=0;x<hole_filled_map->getWidth();x++)	{
			if (markup_image->getPixel(x,y) == Color(0.0f,0.0f,0.0f))	{
				hole_filled_map->setPixel(x,y,Color(0.0f,0.0f,0.0f));
			}
		}
	}

	///Clean up
	delete markup_image;
	return;
}

Image *ImageProcessing::convolve(Image *input_image, Image *kernel)	{

	///Check if the input image is valid
	if (!input_image || !kernel)	return 0x00;

	///Get the range of the input image
	Color *range = new Color[2];
	range = input_image->range();
	//std::cout << range[0] << std::endl;
	//std::cout << range[1] << std::endl;

	///Kernel size is 2k + 1
	int kernel_half_sizex = (kernel->getWidth() - 1)/2;
	int kernel_half_sizey = (kernel->getHeight() - 1)/2;

	///Make the image into a square
	int max_dim = std::max(input_image->getWidth(), input_image->getHeight());
	Image *squared_input_image = new Image(max_dim, max_dim, 0.0f, 0.0f, 0.0f,1.0f);
	for (int y=0;y<input_image->getHeight();y++)	{
		for (int x=0;x<input_image->getWidth();x++)	{
			squared_input_image->setPixel(x,y,input_image->getPixel(x,y));
		}
	}

	Image * padded_input_image = padImage(squared_input_image, kernel_half_sizex, kernel_half_sizey);

	///Perform the DFT on the image
	fftw_complex *dft_image_red = dft(padded_input_image, 0, false, -1, -1);
	fftw_complex *dft_image_green = dft(padded_input_image, 1, false, -1, -1);
	fftw_complex *dft_image_blue = dft(padded_input_image, 2, false, -1, -1);

	///Perform the DFT on the kernel. Make sure the padding is set to 0 and it's the same size as the image
	fftw_complex *dft_kernel_red = dft(kernel, 0, true, padded_input_image->getWidth(), padded_input_image->getHeight());
	fftw_complex *dft_kernel_green = dft(kernel, 1, true, padded_input_image->getWidth(), padded_input_image->getHeight());
	fftw_complex *dft_kernel_blue = dft(kernel, 2, true, padded_input_image->getWidth(), padded_input_image->getHeight());

	///Multiply the two together
	for (int k=0,y=0;y<padded_input_image->getHeight();y++)	{
		for (int x=0;x<padded_input_image->getWidth();x++,k++)	{
			float red_val1 = dft_image_red[k][0] * dft_kernel_red[k][0] - dft_image_red[k][1] * dft_kernel_red[k][1];
			float red_val2 = dft_image_red[k][0] * dft_kernel_red[k][1] + dft_image_red[k][1] * dft_kernel_red[k][0];

			dft_image_red[k][0] = red_val1;
			dft_image_red[k][1] = red_val2;

			float green_val1 = dft_image_green[k][0] * dft_kernel_green[k][0] - dft_image_green[k][1] * dft_kernel_green[k][1];
			float green_val2 = dft_image_green[k][0] * dft_kernel_green[k][1] + dft_image_green[k][1] * dft_kernel_green[k][0];

			dft_image_green[k][0] = green_val1;
			dft_image_green[k][1] = green_val2;

			float blue_val1 = dft_image_blue[k][0] * dft_kernel_blue[k][0] - dft_image_blue[k][1] * dft_kernel_blue[k][1];
			float blue_val2 = dft_image_blue[k][0] * dft_kernel_blue[k][1] + dft_image_blue[k][1] * dft_kernel_blue[k][0];

			dft_image_blue[k][0] = blue_val1;
			dft_image_blue[k][1] = blue_val2;
		}
	}

	///Perform the inverse DFT on the multiplication result
	fftw_complex *idft_image_red = idft(dft_image_red, padded_input_image->getWidth(), padded_input_image->getHeight());
	fftw_complex *idft_image_green = idft(dft_image_green, padded_input_image->getWidth(), padded_input_image->getHeight());
	fftw_complex *idft_image_blue = idft(dft_image_blue, padded_input_image->getWidth(), padded_input_image->getHeight());

	///Convert to Image format
	Image *padded_output_image = copy(idft_image_red, 0,  padded_input_image->getWidth(), padded_input_image->getHeight(), 0x00);
	copy(idft_image_green, 1,  padded_input_image->getWidth(), padded_input_image->getHeight(), padded_output_image);
	copy(idft_image_blue, 2,  padded_input_image->getWidth(), padded_input_image->getHeight(), padded_output_image);

	//Unpad the image
	Image *squared_output_image = unpadImage(padded_output_image, kernel_half_sizex, kernel_half_sizey);
	Image *output_image = new Image(input_image->getWidth(), input_image->getHeight());
	for (int y=0;y<output_image->getHeight();y++)	{
		for (int x=0;x<output_image->getWidth();x++)	{
			output_image->setPixel(x,y,squared_output_image->getPixel(x,y));
		}
	}
	output_image->normalize();
	output_image->normalize(range);

	fftw_free(dft_image_red);
	fftw_free(dft_image_green);
	fftw_free(dft_image_blue);
	fftw_free(dft_kernel_red);
	fftw_free(dft_kernel_green);
	fftw_free(dft_kernel_blue);
	fftw_free(idft_image_red);
	fftw_free(idft_image_green);
	fftw_free(idft_image_blue);
	delete padded_input_image;
	delete padded_output_image;
	delete squared_input_image;
	delete squared_output_image;

	delete [] range;

	return output_image;
}

fftw_complex *ImageProcessing::dft(Image *input_image, int channel, bool padding, int width, int height)	{

	///Check if the input image is valid
	if (!input_image) return 0x00;

	///Input image
	fftw_complex *in_img = copy(input_image, channel, padding, width, height);

	///Output DFT of the input image
	fftw_complex *out_dft_img;
	///Create the plan for the DFT of the image
	fftw_plan dft_img_plan;
	if (padding)	{
		out_dft_img = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * width * height);
		dft_img_plan =  fftw_plan_dft_2d(width, height, in_img, out_dft_img, FFTW_FORWARD, FFTW_ESTIMATE);
	}
	else	{
		out_dft_img = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * input_image->getWidth() * input_image->getHeight());
		dft_img_plan =  fftw_plan_dft_2d(input_image->getWidth(), input_image->getHeight(), in_img, out_dft_img, FFTW_FORWARD, FFTW_ESTIMATE);
	}
	///Execute the plan
	fftw_execute(dft_img_plan);


	fftw_destroy_plan(dft_img_plan);
	fftw_free(in_img);

	return out_dft_img;
}


fftw_complex *ImageProcessing::idft(fftw_complex *input_image, int width, int height, Image *existing_image)	{
	//Check if the input image is valid
	if (!input_image) return 0x00;
	///Allocate memory for the structures used by FFTW.
	int dim = width * height;

	///Output inverse DFT of the input image
	fftw_complex *out_idft_img = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * dim);
	///Create the plan for the DFT of the image
	fftw_plan idft_img_plan =  fftw_plan_dft_2d(width, height, input_image, out_idft_img, FFTW_BACKWARD, FFTW_ESTIMATE);
	///Execute the plan
	fftw_execute(idft_img_plan);

	fftw_destroy_plan(idft_img_plan);

	return out_idft_img;
}

Image *ImageProcessing::copy(fftw_complex *data, int channel, int width, int height, Image *existing_image)	{

	if (existing_image)	{
		for (int k=0, y=0;y<height;y++)	{
			for (int x=0;x<width;x++,k++)	{
				Color val = existing_image->getPixel(x,y);
				switch(channel)	{
					case 0: existing_image->setPixel(x,y,Color(data[k][0],val.g(), val.b()));
							break;
					case 1: existing_image->setPixel(x,y,Color(val.r(), data[k][0], val.b()));
							break;
					case 2: existing_image->setPixel(x,y,Color(val.r(), val.g(), data[k][0]));
							break;
				}
			}
		}
		return 0x00;
	}
	else	{
		///Allocate the memory
		Image *new_image = new Image(width,height,0.0f,0.0f,0.0f,1.0f);
		for (int k=0, y=0;y<height;y++)	{
			for (int x=0;x<width;x++,k++)	{
				Color val = new_image->getPixel(x,y);
				switch(channel)	{
					case 0: new_image->setPixel(x,y,Color(data[k][0],val.g(), val.b()));
							break;
					case 1: new_image->setPixel(x,y,Color(val.r(), data[k][0], val.b()));
							break;
					case 2: new_image->setPixel(x,y,Color(val.r(), val.g(), data[k][0]));
							break;
				}
			}
		}
		return new_image;
	}
	return 0x00;
}

fftw_complex *ImageProcessing::copy(Image *data, int channel, bool padding, int width, int height)	{
    ///Allocate the memory
	fftw_complex *new_data = 0x00;
	if (padding)	{
		new_data = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * width * height);
		for (int k=0,y=0;y<height;y++)	{
				for (int x=0;x<width;x++,k++)	{
					if (y >= data->getHeight() || x >= data->getWidth())	{
						new_data[k][0] = new_data[k][1] = 0.0f;
					}
					else	{
						switch (channel)	{
							case 0: new_data[k][0] = data->getPixel(x,y).r();
									break;
							case 1: new_data[k][0] = data->getPixel(x,y).g();
									break;
							case 2: new_data[k][0] = data->getPixel(x,y).b();
									break;
						}
						new_data[k][1] = 0.0f;
					}
				}
			}
	}
	else	{
		new_data = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * data->getWidth() * data->getHeight());
		for (int k=0,y=0;y<data->getHeight();y++)	{
				for (int x=0;x<data->getWidth();x++,k++)	{
					switch (channel)	{
						case 0: new_data[k][0] = data->getPixel(x,y).r();
								break;
						case 1: new_data[k][0] = data->getPixel(x,y).g();
								break;
						case 2: new_data[k][0] = data->getPixel(x,y).b();
								break;
					}
					new_data[k][1] = 0.0f;
				}
			}
	}
	return new_data;
}

///Recreate the image to sizex + kernel_half_sizex etc
Image *ImageProcessing::padImage(Image *input_image, int width, int height)	{
    Image *padded_input_image = new Image(input_image->getWidth() + width, input_image->getHeight() + height);
	for (int y=0;y<padded_input_image->getHeight();y++)	{
		for (int x=0;x<padded_input_image->getWidth();x++)	{
			if (x < input_image->getWidth() && y < input_image->getHeight())	{
				padded_input_image->setPixel(x,y, input_image->getPixel(x,y));
			}
		}
	}

	return padded_input_image;
}

Image *ImageProcessing::unpadImage(Image *input_image, int width, int height)	{
    Image *unpadded_output_image = new Image(input_image->getWidth() - width, input_image->getHeight() - height);
	for (int y=0;y<unpadded_output_image->getHeight();y++)	{
		for (int x=0;x<unpadded_output_image->getWidth();x++)	{
			unpadded_output_image->setPixel(x,y, input_image->getPixel(x+width,y+height));
		}
	}
	return unpadded_output_image;
}

#endif

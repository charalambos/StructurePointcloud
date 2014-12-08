////////////////////////////////////////////////////////////////////////////////////
// Copyright © Charalambos "Charis" Poullis, charalambos@poullis.org    	      //
// This work can only be used under an exclusive license of the author.           //
////////////////////////////////////////////////////////////////////////////////////

#ifndef __GEOSPATIAL_BOUNDING_BOX_CPP__
#define __GEOSPATIAL_BOUNDING_BOX_CPP__

#include "GeospatialBoundingBox.h"

#define DEBUG	1

int GeospatialBoundingBox::global_id = 0;

GeospatialBoundingBox::GeospatialBoundingBox()	{
	file_name = _format("geo_bbox_%d.xyz",global_id++);

	normal_map = 0x00;
	xyz_map = 0x00;
	height_and_normal_variation_map = 0x00;
}

GeospatialBoundingBox::GeospatialBoundingBox(int index, std::string *base_file_name)	{

	normal_map = 0x00;
	xyz_map = 0x00;
	height_and_normal_variation_map = 0x00;


	load(index, base_file_name);
}

GeospatialBoundingBox::GeospatialBoundingBox(Vector3f const &_centroid, Vector3f const &_resolution, std::string *_file_name)	{

	resolution = _resolution;

	///mark the point as the centroid
	centroid = _centroid;

	///Initialize the min height
	min_height = FLT_MAX;

	///compute the min and max (allow +1 buffer zone)
	min_pt = centroid - (Vector3f(1.0f,1.0f,1.0f) + resolution/2.0f);
	max_pt = centroid + (Vector3f(1.0f,1.0f,1.0f) + resolution/2.0f);

	number_of_points = 0;

	if (_file_name)	{
		file_name = (*_file_name);
	}
	else	{
		file_name = _format("geo_bbox_%d.xyz",global_id++);
	}
	output_file.open(file_name.c_str());

	normal_map = 0x00;
	xyz_map = 0x00;
	height_and_normal_variation_map = 0x00;
}

GeospatialBoundingBox::~GeospatialBoundingBox()	{
 	if (output_file.is_open())	output_file.close();
	if (xyz_map)	delete xyz_map;
	if (normal_map)	delete normal_map;
	if (height_and_normal_variation_map) delete height_and_normal_variation_map;
}

int GeospatialBoundingBox::getNumberOfPoints()	const	{
 	return number_of_points;
}

std::vector<Vector3f> GeospatialBoundingBox::getPoints(bool offset) const	{

	std::string line;
	std::vector<Vector3f> points;
	///open the file for reading
	ifstream input_file(file_name.c_str());
	if (!input_file.is_open()) return points;

// 	Vector3f _centroid;
// 	getline (input_file,line);
// 	sscanf(line.c_str(),"centroid: %f %f %f\n",&_centroid(0),&_centroid(1),&_centroid(2));
//
// 	Vector3f _min_pt;
// 	getline (input_file,line);
// 	sscanf(line.c_str(),"min_pt: %f %f %f\n",&_min_pt(0),&_min_pt(1),&_min_pt(2));
//
// 	Vector3f _max_pt;
// 	getline (input_file,line);
// 	sscanf(line.c_str(),"max_pt: %f %f %f\n",&_max_pt(0),&_max_pt(1),&_max_pt(2));
//
	///Read all the points (-1 because the centroid is not stored in file)
	for (int i=0;i<number_of_points;i++)	{
		///read in the line
 		getline (input_file,line);
		if (!line.size()) continue;
		///parse it
		Vector3f point;
		sscanf(line.c_str(),"%f %f %f\n",&point(0),&point(1),&point(2));

		///Add to the list
		if (offset)	{
			points.push_back(point-min_pt);
		}
		else	{
			points.push_back(point);
		}
	}
	///close the file
	input_file.close();
	return points;
}

Vector3f GeospatialBoundingBox::getCentroid() const	{
 	return centroid;
}

void GeospatialBoundingBox::insert(Vector3f const &point)	{
	///write it out to the file
	output_file << point(0) << " " << point(1) << " " << point(2) << endl;

	///increment the number of points
	number_of_points++;

	///Check if it's the height is minimum
	if (min_height > point(2))	{
		min_height = point(2);
	}

	return;
}

void GeospatialBoundingBox::initialize(Image *resampled)	{

	if (!resampled)	{

		///Close the pointer to the output file
		if (output_file.is_open())	output_file.close();

		///Get the points from the file
		std::vector<Vector3f> points = getPoints(true);

		///Resample the data into an XYZ map
		if (!resample(EPSILON))	{
			return;
		}
		exportXYZMap("A");

		///Perform a hole filling on the XYZ map
		Image *non_hole_filled_map = new Image();
		non_hole_filled_map->copy(xyz_map);
		ImageProcessing::holeFill(xyz_map,6,4);
		exportXYZMap("B");

		///Perform point reduction by removing extra points added by the hole filling
		ImageProcessing::pointReduction(non_hole_filled_map, xyz_map, 100);
		exportXYZMap("C");
		delete non_hole_filled_map;

		///Apply bilateral filtering
		Image *zzz_map = new Image(xyz_map->getWidth(), xyz_map->getHeight());
		///Copy the XYZ map
		zzz_map->copy(xyz_map);
		///Convert from RGB to BBB
		zzz_map->grayscale(2);
		///Normalize the zzz_map
		zzz_map->normalize();

		Image *filtered_image = ImageProcessing::bilateralFilteringGrayscale(zzz_map,1.0,1.0,0.025,0.025);
		Color *min_max_val = xyz_map->range();
		for (int y=0;y<xyz_map->getHeight();y++)	{
			for (int x=0;x<xyz_map->getWidth();x++)	{
				Color val = xyz_map->getPixel(x,y);
				///If the point didn't have a valid point then don't add the smooth one
				if (val == Color(0.0f,0.0f,0.0f))	continue;
				float b = filtered_image->getPixel(x,y).b();
				b = min_max_val[0].b() + b*(min_max_val[1].b()-min_max_val[0].b());
				xyz_map->setPixel(x,y,Color(val.r(),val.g(),b));
			}
		}
		delete zzz_map;
		delete filtered_image;
		exportXYZMap("D");

		delete [] min_max_val;

	}
	else	{
		xyz_map = resampled;
	}

	///Compute a normal map based on the XYZ map of the geo_box
	if (normal_map)	delete normal_map;
	normal_map = GeometryProcessing::computeNormalMap(xyz_map, true);
	normal_map->blur(1);
	normal_map->saveImage(_format("%s_normal_map.pfm", file_name.c_str()));

	///Compute the height variation between the points
	//This is not used during the structure but later on for the clustering part
	//computeHeightAndNormalVariation();

	///Triangulate the points using the xyz map
    //object = GeometryProcessing::triangulate(xyz_map);

	return;
}

bool GeospatialBoundingBox::maxedOut()	{
	return (number_of_points >= MAX_CAPACITY);
}

Vector3f GeospatialBoundingBox::getResolution()	{
 	return resolution;
}

std::string GeospatialBoundingBox::getFileName()	{
 	return file_name;
}

void GeospatialBoundingBox::setFileName(std::string const &_file_name)	{
 	///Close the existing file
	output_file.close();
	///Erase it from disk
	if (remove(file_name.c_str()) == -1)	{
		std::cout << " Couldn't remove original file..." << std::endl;
	}

	///Assign the new name
	file_name = _file_name;

	///open the file with the new name
	output_file.open(file_name.c_str());
	return;
}

bool GeospatialBoundingBox::liesInside(Vector3f const &point)	{
 	return ( (point(0) + EPSILON >= min_pt(0)) && (point(0) <= max_pt(0) + EPSILON) &&
		 (point(1) + EPSILON >= min_pt(1)) && (point(1) <= max_pt(1) + EPSILON) &&
		 (point(2) + EPSILON >= min_pt(2)) && (point(2) <= max_pt(2) + EPSILON) );
}

void GeospatialBoundingBox::exportXYZMap(std::string const &post_script)	{

	if (post_script == "")	{
		xyz_map->saveImage(_format("%s.pfm",file_name.c_str()));
		std::cout << _format("XYZ map %s.pfm saved.",file_name.c_str()).c_str() << std::endl;
	}
	else	{
		xyz_map->saveImage(_format("%s_%s.pfm",file_name.c_str(),post_script.c_str()));
		std::cout << _format("XYZ map %s_%s.pfm saved.",file_name.c_str(),post_script.c_str()).c_str() << std::endl;
	}

	return;
}

Image *GeospatialBoundingBox::getXYZMap()	{
	return xyz_map;
}

Image *GeospatialBoundingBox::getNormalMap()	{
    return normal_map;
}

Image *GeospatialBoundingBox::getHeightAndNormalVariationMap()	{
    return height_and_normal_variation_map;
}

GeometricObject *GeospatialBoundingBox::getObject()	{

    std::string start_time = timestamp();
    GeometricObject *new_object = GeometryProcessing::triangulate(xyz_map, normal_map);
    std::string end_time = timestamp();

    std::cout << _format("Triangulation process done. (start time: %s, end time: %s)", start_time.c_str(), end_time.c_str()) << std::endl;

 	return new_object;

}

Vector3f GeospatialBoundingBox::getPoint(Vector2i const &point_index)	{
    return color2vector3<float>(xyz_map->getPixel(point_index(0),point_index(1)));
}

float GeospatialBoundingBox::getMinHeight()	{
    return min_height;
}

void GeospatialBoundingBox::cleanUp()	{

	if (xyz_map)	delete xyz_map;
	if (normal_map)	delete normal_map;
	if (height_and_normal_variation_map) delete height_and_normal_variation_map;
	xyz_map = normal_map = height_and_normal_variation_map = 0x00;
	return;
}

bool GeospatialBoundingBox::resample(double sampling_tolerance, double step)	{

	std::cout << _format("Re-sampling process initiated.").c_str() << std::endl;

	///Get all the points contained in the file
	std::vector<Vector3f> points = getPoints(true);

    ///Make 2 copies of the points
    std::vector<Vector3f> sorted_vertices_based_x,sorted_vertices_based_y;
    for (int i=0;i<points.size();i++)         {
            sorted_vertices_based_x.push_back(points[i]);
            sorted_vertices_based_y.push_back(points[i]);
    }

	std::cout << _format("Vertices sorted based on x:%d, vertices sorted based on y:%d",sorted_vertices_based_x.size(),sorted_vertices_based_y.size()).c_str() << std::endl;

    ///sort based on the x component
	SorterComplexType3vf::quicksort(sorted_vertices_based_x,0,sorted_vertices_based_x.size()-1,0);
    ///sort based on the y component
	SorterComplexType3vf::quicksort(sorted_vertices_based_y,0,sorted_vertices_based_y.size()-1,1);


    //go through the sorted arrays and count only unique number of x and y
    bool size_not_ok = true;
    int sizex,sizey;
    std::vector<Vector3f> unique_sorted_x,unique_sorted_y;
    while(size_not_ok)  {
             size_not_ok = false;
             for (unsigned int i=0;i<sorted_vertices_based_x.size();i++)     {
                     unsigned int starting_pos = i;
                     unique_sorted_x.push_back(sorted_vertices_based_x[starting_pos]);
                     while (i+1 < sorted_vertices_based_x.size() && fabs(sorted_vertices_based_x[starting_pos](0) - sorted_vertices_based_x[i+1](0)) <= sampling_tolerance)    i++;
                     //if (i+1 < sorted_vertices_based_x.size()) printf("%f\n",fabs(sorted_vertices_based_x[starting_pos](0) - sorted_vertices_based_x[i+1](0)));
             }
             sizex = unique_sorted_x.size();
		//if its bigger than then max size then increase the size of the cells
		if (sizex > MAX_SIZE_X) {
			size_not_ok = true;
			unique_sorted_x.clear();
			sampling_tolerance += step;
		}
     }
     size_not_ok = true;
     while(size_not_ok)  {
             size_not_ok = false;
             for (int j=0;j<sorted_vertices_based_y.size();j++)     {
                     int starting_pos = j;
                     unique_sorted_y.push_back(sorted_vertices_based_y[starting_pos]);
                     while (j+1 < sorted_vertices_based_y.size() && fabs(sorted_vertices_based_y[starting_pos](1) - sorted_vertices_based_y[j+1](1)) <= sampling_tolerance)            j++;
                     //if (j+1 < sorted_vertices_based_y.size()) printf("%f\n",fabs(sorted_vertices_based_y[starting_pos](1) - sorted_vertices_based_y[j+1](1)));
             }
             sizey = unique_sorted_y.size();
		//if its bigger than then max size then increase the size of the cells
		if (sizey > MAX_SIZE_Y)  {
			size_not_ok = true;
			unique_sorted_y.clear();
			sampling_tolerance += step;
		}
     }

     std::cout << _format("Chosen epsilon(bin size): %f. Map size will be %dx%d",sampling_tolerance,sizex,sizey).c_str() << std::endl;

     ///Check if the size of the map is more than the maximum size
     if (sizex > MAX_SIZE_X || sizey > MAX_SIZE_Y)	{
    	 std::cout << _format("The size of the map exceeds the maximum conditions.") << std::endl;
    	 return false;
     }

     //It will generate maps of size K pixels
     int map_sizex = min(sizex,MAX_SIZE_X);
     int map_sizey = min(sizey,MAX_SIZE_Y);

	 	 //create an XYZ map(2d grid) which will store all the points
	 xyz_map = new Image(map_sizex,map_sizey, 0.0f,0.0f,0.0f,1.0f);
	 Image *weights = new Image(map_sizex,map_sizey,0.0f,0.0f,0.0f,1.0f);

	 //go through the sorted points in x and y and distribute them in the grid
	 int hits = 0;
	 for (int i=0;i<map_sizex;i++)  {
			 if (i >= sizex) break;
			 for (int j=0;j<sorted_vertices_based_y.size();j++)     {
							 if (fabs(unique_sorted_x[i](0)-sorted_vertices_based_y[j](0)) <= sampling_tolerance)      {
									 //find where it should be positioned
									 int pos = -1;
									 for (int u=0;u<unique_sorted_y.size();u++)     {
											 if (fabs(unique_sorted_y[u](1) - sorted_vertices_based_y[j](1)) <= sampling_tolerance)    {
													 pos  = u;
													 //pos = sizey-1-pos; //flip it because of the upper left corner origin
													 break;
											 }
									 }
									 if (pos >= 0 && pos < map_sizey)        {/*printf("within range\n");*/;}
									 else {/*printf(" %d =<pos: %d < %d\n",ny*map_size,pos,(ny+1)*map_size);*/ continue;}
									 Color existing_value = weights->getPixel(i,pos);
									 weights->setPixel(i,pos, existing_value + Color(1.0f));
									 existing_value = xyz_map->getPixel(i,pos);
									 xyz_map->setPixel(i,pos, existing_value + Color(float(unique_sorted_x[i](0)),float(sorted_vertices_based_y[j](1)),float(sorted_vertices_based_y[j](2))));
									 hits++;
							 }
			 }
	 }
	 //averaging
	 for (int y=0;y<weights->getHeight();y++)     {
			 for (int x=0;x<weights->getWidth();x++)     {
					 if (weights->getPixel(x,y) != Color(0.0f,0.0f,0.0f,1.0f))        {
							 xyz_map->setPixel(x,y, xyz_map->getPixel(x,y)/weights->getPixel(x,y).r());
					 }
			 }
	 }
	 if (hits==0)	{
		 std::cout << _format("XYZ map contains no points.").c_str() << std::endl;
	 }

	 std::cout << _format("Re-sampling process complete.").c_str() << std::endl;

	delete weights;
	return true;
}

void GeospatialBoundingBox::computeHeightAndNormalVariation()	{

	int neighbourhood_size = 11;
	int neighbourhood_search_size = neighbourhood_size/2;
	///Create a map the same size as the xyz map
	if (height_and_normal_variation_map)	delete height_and_normal_variation_map;
	height_and_normal_variation_map = new Image(xyz_map->getWidth(), xyz_map->getHeight(),0.0f,0.0f,0.0f,0.0f);

	///Go through the xyz map and for each point compute the min max values in a 3x3 window area
	for (int y=0;y<xyz_map->getHeight();y++)	{
		for (int x=0;x<xyz_map->getWidth();x++)	{
			///Check only the valid points
			if ((xyz_map->getPixel(x,y) == Color(0.0f,0.0f,0.0f)) || (normal_map->getPixel(x,y) == Color(0.0f,0.0f,0.0f))) continue;
			float min_height = xyz_map->getPixel(x,y)(2);
			float max_height = xyz_map->getPixel(x,y)(2);

			float min_dot = 1.0f;
			float max_dot = 0.0f;

			///Get the normal of the point in question
			Vector3f normal = color2vector3<float>(normal_map->getPixel(x,y));

			///Search in a neighbourhood
			for (int i=y-neighbourhood_search_size;i<=y+neighbourhood_search_size;i++){
				for (int j=x-neighbourhood_search_size;j<=x+neighbourhood_search_size;j++)	{
					///If it's the same continue
					if (i==y && j==x)	continue;
					///Check for out of bounds
					if (outOfBounds(xyz_map, j, i))	continue;
					///if its a valid point
					if (xyz_map->getPixel(j,i) == Color(0.0f,0.0f,0.0f))	continue;

					///Check if max or min for the depth
					if (min_height > xyz_map->getPixel(j,i)(2))	min_height = xyz_map->getPixel(j,i)(2);
					if (max_height < xyz_map->getPixel(j,i)(2))	max_height = xyz_map->getPixel(j,i)(2);

					///Check if max or min for the dot product
					float dot_product = 1.0f - fabs(dot(normal, color2vector3<float>(normal_map->getPixel(j,i))));

					if (min_dot > dot_product)	min_dot = dot_product;
					if (max_dot < dot_product)	max_dot = dot_product;
				}
			}

			///Compute the height variation
			float height_var = max_height - min_height;
			///Compute the normal variation
			float normal_var = max_dot - min_dot;
			///Store it in the variation map
			height_and_normal_variation_map->setPixel(x,y,Color(height_var,normal_var,0.0f));
		}
	}


	///Go through the map again and compute the second derivative of differences
	for (int y=0;y<height_and_normal_variation_map->getHeight();y++)	{
		for (int x=0;x<height_and_normal_variation_map->getWidth();x++)	{
			///Check only the valid points
			if (height_and_normal_variation_map->getPixel(x,y).r() == 0.0f &&
				height_and_normal_variation_map->getPixel(x,y).g() == 0.0f)	continue;

			///Search in a neighbourhood
			float sum_of_differences_height = 0.0f;
			float sum_of_differences_normal= 0.0f;
			float neighbours = 0.0f;
			for (int i=y-neighbourhood_search_size;i<=y+neighbourhood_search_size;i++){
				for (int j=x-neighbourhood_search_size;j<=x+neighbourhood_search_size;j++)	{
					///If it's the same continue
					if (i==y && j==x)	continue;
					///Check for out of bounds
					if (outOfBounds(height_and_normal_variation_map, j, i))	continue;
					///if its a valid point
					///Check only the valid points
					if (height_and_normal_variation_map->getPixel(j,i).r() == 0.0f &&
						height_and_normal_variation_map->getPixel(j,i).g() == 0.0f)	continue;
					sum_of_differences_height += fabs(height_and_normal_variation_map->getPixel(x,y).r()-height_and_normal_variation_map->getPixel(j,i).r());
					sum_of_differences_normal+= fabs(height_and_normal_variation_map->getPixel(x,y).g()-height_and_normal_variation_map->getPixel(j,i).g());
					neighbours++;
				}
			}
			sum_of_differences_height /= max(1.0f,neighbours);
			sum_of_differences_normal /= max(1.0f,neighbours);

			Color val = height_and_normal_variation_map->getPixel(x,y);
			height_and_normal_variation_map->setPixel(x,y,Color(val.r(), val.g(), sum_of_differences_height, sum_of_differences_normal));
		}
	}

	if (DEBUG)	{
		Image *secondary_image = new Image(height_and_normal_variation_map->getWidth(), height_and_normal_variation_map->getHeight(), 0.0f,0.0f,0.0f,1.0f);
		for (int y=0;y<height_and_normal_variation_map->getHeight();y++)	{
			for (int x=0;x<height_and_normal_variation_map->getWidth();x++)	{
				if (height_and_normal_variation_map->getPixel(x,y).r() == 0.0f &&
					height_and_normal_variation_map->getPixel(x,y).g() == 0.0f)	continue;
				secondary_image->setPixel(x,y, Color(height_and_normal_variation_map->getPixel(x,y).b(), height_and_normal_variation_map->getPixel(x,y).a(),1.0f));
			}
		}
		height_and_normal_variation_map->saveImage(_format("%s_height_and_dot_variation_map_A.pfm", file_name.c_str()));
		secondary_image->saveImage(_format("%s_height_and_dot_variation_map_B.pfm", file_name.c_str()));
		delete secondary_image;
	}
	return;
}

void GeospatialBoundingBox::load(int index, std::string *base_file_name)	{

	if (base_file_name)	{
		file_name = (*base_file_name);
	}
	else	{
		file_name = _format("geo_bbox_%d.xyz.info",index);
	}

	FILE *file_ptr = fopen(file_name.c_str(), "r");
	fscanf(file_ptr,"centroid: %f %f %f\n",&centroid(0), &centroid(1), &centroid(2));
	fscanf(file_ptr,"resolution: %f %f %f\n",&resolution(0), &resolution(1), &resolution(2));
	fscanf(file_ptr,"min_pt: %f %f %f\n",&min_pt(0), &min_pt(1), &min_pt(2));
	fscanf(file_ptr,"max_pt: %f %f %f\n",&max_pt(0), &max_pt(1), &max_pt(2));
	fscanf(file_ptr,"points: %d\n", &number_of_points);
	fclose(file_ptr);
	return;
}

void GeospatialBoundingBox::exportInformationFile()	{

	FILE *file_ptr = fopen(_format("%s.info",file_name.c_str()).c_str(), "w");
	fprintf(file_ptr, "centroid: %f %f %f\n", centroid(0), centroid(1), centroid(2));
	fprintf(file_ptr, "resolution: %f %f %f\n",resolution(0), resolution(1), resolution(2));
	fprintf(file_ptr, "min_pt: %f %f %f\n", min_pt(0), min_pt(1), min_pt(2));
	fprintf(file_ptr, "max_pt: %f %f %f\n", max_pt(0), max_pt(1), max_pt(2));
	fprintf(file_ptr, "points: %d\n", number_of_points);
	fclose(file_ptr);
	return;
}

#endif


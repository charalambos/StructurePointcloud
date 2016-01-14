////////////////////////////////////////////////////////////////////////////////////
// Copyright © Charalambos "Charis" Poullis, charalambos@poullis.org    	      //
// This work can only be used under an exclusive license of the author.           //
////////////////////////////////////////////////////////////////////////////////////

#ifndef __GEOSPATIAL_BOUNDING_BOX_H__
#define __GEOSPATIAL_BOUNDING_BOX_H__


#include <float.h>
#include <iostream>
#include <fstream>

#include <Eigen/Eigen>
using namespace Eigen;

#include "BoundingBox.h"
#include "ImageProcessing.h"
#include "GeometryProcessing.h"
#include "Image.h"
#include "Utilities.h"
#include "Sorter.h"
#include <vector>

using namespace std;

#define MAX_SIZE_X	    2048
#define MAX_SIZE_Y		2048

#define MAX_CAPACITY	MAX_SIZE_X * MAX_SIZE_Y


/**		Geospatial Bounding Box
* This class defines a geospatial bounding box and related functions
* Subclasses the BoundingBox class
*/

class GeospatialBoundingBox : public BoundingBox	{
	public:
		///Constructors
		GeospatialBoundingBox();
		GeospatialBoundingBox(int index, std::string *base_file_name=0x00);
		GeospatialBoundingBox(Vector3f const &_centroid, Vector3f const &_resolution, std::string *_file_name=0x00);
		///Destructor
		~GeospatialBoundingBox();

		///Return the number of points contained in the bounding box
		int getNumberOfPoints() const;

		///Return the points
		std::vector<Vector3f> getPoints(bool offset=false) const;

		///Insert a new point in the bounding box
		void insert(Vector3f const &point);

		///Get the centroid
		Vector3f getCentroid() const;

		///Initializes the data required for further processing i.e. xyz map, triangulation, and closing the file
		void initialize(Image *resampled=0x00);

		///Returns true if the box has reached maximum capacity
		bool maxedOut();

		///Returns a 3D vector representing the resolution of the box
		Vector3f getResolution();

		///Returns the filename corresponding to this geospatial bounding box
		std::string getFileName();

		///Sets the filename
		void setFileName(std::string const &_file_name);

		///Returns true if the point lies inside the geospatial bounding box
		bool liesInside(Vector3f const &point);

		///Exports the XYZ map corresponding to the geobox
		void exportXYZMap(std::string const &post_script);

		///Returns the XYZ map corresponding to the geobox
		Image *getXYZMap();

		///Returns the normal map corresponding to the geobox
		Image *getNormalMap();

		///Returns the height and normal variation map corresponding to the geobox
		Image *getHeightAndNormalVariationMap();

		///Return the point corresponding to the index
		Vector3f getPoint(Vector2i const &point_index);

		///Returns the minimum height (actual value)
		float getMinHeight();

		///Clean up excess memory
		void cleanUp();

		///Resample into the right-sized XYZ in order to minimize information loss
		bool resample(double sampling_tolerance= 1.0, double step = 0.05);

		///Loads the information from disk
		void load(int index, std::string *base_file_name=0x00);

		///Exports an information about the geobox
		void exportInformationFile();

		///Returns the geometric object
		GeometricObject *getObject();

	private:
		///The number of points contained in this bounding box
		int number_of_points;
		///The file pointer containing the points
		ofstream output_file;
		///The filename of the bounding box's points
		std::string file_name;
		///Resolution on x,y,z
		Vector3f resolution;
		///The XYZ map corresponding to the bbox
		Image *xyz_map;
		///The normal map
		Image *normal_map;
		///The height and variation map
		Image *height_and_normal_variation_map;
		///The keypoint of the bounding box
		Vector3f centroid;
		///Minimum value for height
		float min_height;

		///Global static bounding box id
		static int global_id;


		///Computes the height and normal variation map
		void computeHeightAndNormalVariation();

};


#endif

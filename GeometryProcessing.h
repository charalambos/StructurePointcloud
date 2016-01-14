////////////////////////////////////////////////////////////////////////////////////
// Copyright © Charalambos "Charis" Poullis, charalambos@poullis.org    	  //
// This work can only be used under an exclusive license of the author.           //
////////////////////////////////////////////////////////////////////////////////////

#ifndef __GEOMETRY_PROCESSING_H__
#define __GEOMETRY_PROCESSING_H__

/**		Geometry Processing
* This class defines a set of functions to process geometry.
*
*/

#include <float.h>
#include "opencv.hpp"

#include "Image.h"
#include "Color.h"
#include <Eigen/Eigen>
using namespace Eigen;

#include "Utilities.h"
#include "GeometricObject.h"

class GeometryProcessing	{

	public:
		///Performs triangulation using an XYZ map
		static GeometricObject *triangulate(Image *xyz_map, Image *_normal_map=0x00);

		///Computes the normal at a point given an XYZ map
		static Vector3f computeLocalNormal(Image *map, Vector2i const &index, bool z_up=false);

		///Computes the normal map per point given an XYZ map
		static Image *computeNormalMap(Image *map, bool z_up=false);
};


#endif

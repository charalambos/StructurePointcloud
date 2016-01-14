////////////////////////////////////////////////////////////////////////////////////
// Copyright © Charalambos "Charis" Poullis, charalambos@poullis.org    	      //
// This work can only be used under an exclusive license of the author.           //
////////////////////////////////////////////////////////////////////////////////////


#ifndef __BOUNDING_BOX_H__
#define __BOUNDING_BOX_H__

/**		Bounding Box
* This class defines a bounding box and related functions
*
*/


#include <float.h>
#include "Eigen/Eigen"
using namespace Eigen;

class BoundingBox	{
	public:
		///Constructor
		BoundingBox();
		///Constructor
		BoundingBox(Vector3f const &_min_pt, Vector3f const &_max_pt);
		///Destructor
		~BoundingBox();

		///Get min point
		Vector3f getMinPt() const;

		///Get max point
		Vector3f getMaxPt() const;

		///Set min point
		void setMinPt(Vector3f const _min_pt);

		///Set max point
		void setMaxPt(Vector3f const _max_pt);

	protected:
		///The minimum point
		Vector3f min_pt;
		///The maximum point
		Vector3f max_pt;
};


#endif

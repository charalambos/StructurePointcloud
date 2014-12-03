////////////////////////////////////////////////////////////////////////////////////
// Copyright © Charalambos "Charis" Poullis, charalambos@poullis.org    	      //
// This work can only be used under an exclusive license of the author.           //
////////////////////////////////////////////////////////////////////////////////////

#ifndef __BOUNDING_BOX_CPP__
#define __BOUNDING_BOX_CPP__

#include "BoundingBox.h"

#include "Monitoring.h"

BoundingBox::BoundingBox()	{
    functions_used["BoundingBox: BoundingBox"] = 1;

	min_pt = Vector3f(FLT_MAX, FLT_MAX, FLT_MAX);
	max_pt = Vector3f(-FLT_MAX, -FLT_MAX, -FLT_MAX);
}

BoundingBox::BoundingBox(Vector3f const &_min_pt, Vector3f const &_max_pt)	{
    functions_used["BoundingBox: BoundingBox"] = 1;
	min_pt = _min_pt;
	max_pt = _max_pt;
}

BoundingBox::~BoundingBox()	{
    functions_used["BoundingBox: ~BoundingBox"] = 1;

}


Vector3f BoundingBox::getMinPt() const	{
    functions_used["BoundingBox: getMinPt"] = 1;
	return min_pt;
}

Vector3f BoundingBox::getMaxPt() const	{
    functions_used["BoundingBox: getMaxPt"] = 1;
	return max_pt;
}

void BoundingBox::setMinPt(Vector3f const _min_pt)	{
    functions_used["BoundingBox: setMinPt"] = 1;

	min_pt = _min_pt;
	return;
}

void BoundingBox::setMaxPt(Vector3f const _max_pt)	{
    functions_used["BoundingBox: setMaxPt"] = 1;

	max_pt = _max_pt;
	return;
}

#endif

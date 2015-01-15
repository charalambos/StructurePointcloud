////////////////////////////////////////////////////////////////////////////////////
// Copyright © Charalambos "Charis" Poullis, charalambos@poullis.org    	  //
// This work can only be used under an exclusive license of the author.           //
////////////////////////////////////////////////////////////////////////////////////

#ifndef __GEOMETRY_EXPORTER_H__
#define __GEOMETRY_EXPORTER_H__

#include <time.h>
#include "Face.h"
#include "Utilities.h"
#include "GeometricObject.h"

class GeometryExporter	{
	public:
		///Export geometry into an OBJ file
		static void exportToOBJ(const char *file_name, GeometricObject *object);
};


#endif

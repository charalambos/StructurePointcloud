////////////////////////////////////////////////////////////////////////////////////
// Copyright © Charalambos "Charis" Poullis, charalambos@poullis.org    	  //
// This work can only be used under an exclusive license of the author.           //
////////////////////////////////////////////////////////////////////////////////////


#ifndef __GEOMETRIC_OBJECT_H__
#define __GEOMETRIC_OBJECT_H__


/**
*	Geometric Object
* This class defines a geometric object with all it's properties
**/
#include <math.h>
#include <iostream>

#include "Edge.h"
#include "Face.h"
#include "Eigen/Eigen"
using namespace Eigen;

class GeometricObject {
	public:
		///Constructors
		GeometricObject(std::vector<Vector3f> const &_vertices,
				std::vector<Vector3f> const &_normals,
				std::vector<Vector2f> const &_tex_coords,
				std::vector<Face *> const &_faces,
				std::vector<Edge *> const &_edges);

		///Copy constructor
		GeometricObject(GeometricObject const &_object);
		///Destructor
		~GeometricObject();

		///Get methods
		std::vector<Vector3f> getVertices() const ;
		Vector3f getVertexAt(int index) const ;

		std::vector<Vector3f> getNormals() const ;
		Vector3f getNormalAt(int index) const ;

		std::vector<Vector2f> getTextureCoords() const ;
		Vector2f getTextureCoordAt(int index) const ;

		std::vector<Face *> getFaces() const ;
		Face *getFaceAt(int index) const ;

		std::vector<Edge *> getEdges() const ;
		Edge *getEdgeAt(int index) const;

        void translate(Vector3f _translation);

	private:
		///A list of vertices
		std::vector<Vector3f> vertices;
		///A list of normals
		std::vector<Vector3f> normals;
		///A list of texture coordinates
		std::vector<Vector2f> texture_coords;
		///A list of edges
		std::vector<Edge *> edges;
		///A list of faces
		std::vector<Face *> faces;
};

#endif

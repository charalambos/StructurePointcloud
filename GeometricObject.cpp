////////////////////////////////////////////////////////////////////////////////////
// Copyright © Charalambos "Charis" Poullis, charalambos@poullis.org    	  //
// This work can only be used under an exclusive license of the author.           //
////////////////////////////////////////////////////////////////////////////////////


#ifndef __GEOMETRIC_OBJECT_CPP__
#define __GEOMETRIC_OBJECT_CPP__


#include "GeometricObject.h"
#include "GeometryProcessing.h"

GeometricObject::GeometricObject(std::vector<Vector3f> const &_vertices,
				 std::vector<Vector3f> const &_normals,
				 std::vector<Vector2f> const &_tex_coords,
				 std::vector<Face *> const &_faces,
				 std::vector<Edge *> const &_edges):
				 vertices(_vertices),normals(_normals),texture_coords(_tex_coords),faces(_faces),edges(_edges)	{

}


GeometricObject::~GeometricObject()	{

	vertices.clear();
	normals.clear();
	texture_coords.clear();

	for (int i=0;i<edges.size();i++)	{
		delete edges[i];
	}
	edges.clear();

	for (int i=0;i<faces.size();i++)	{
		delete faces[i];
	}
	faces.clear();
}

std::vector<Vector3f> GeometricObject::getVertices() const	{

	return vertices;
}

Vector3f GeometricObject::getVertexAt(int index) const	{

	if (index>=0 && index<vertices.size())	{
		return vertices[index];
	}
	else	{
		std::cout << "here 4b: " << index << " out of " << vertices.size() << std::endl;
	}
	return Vector3f();
}

std::vector<Vector3f> GeometricObject::getNormals() const	{

	return normals;
}

Vector3f GeometricObject::getNormalAt(int index) const	{

	if (index>=0 && index<normals.size())	{
		return normals[index];
	}
	return Vector3f();
}

std::vector<Vector2f> GeometricObject::getTextureCoords() const	{

	return texture_coords;
}

Vector2f GeometricObject::getTextureCoordAt(int index) const	{

	if (index>=0 && index<texture_coords.size())	{
		return texture_coords[index];
	}
	return Vector2f();
}

std::vector<Face *> GeometricObject::getFaces() const	{

	return faces;
}

Face *GeometricObject::getFaceAt(int index) const	{

	if (index>=0 && index<faces.size())	{
		return faces[index];
	}
	return 0x00;
}

std::vector<Edge *> GeometricObject::getEdges() const	{

	return edges;
}

Edge *GeometricObject::getEdgeAt(int index) const	{

	if (index>=0 && index<edges.size())	{
		return edges[index];
	}
	return 0x00;
}

void GeometricObject::translate(Vector3f _translation)  {
    for (int i=0;i<vertices.size();i++) {
        vertices[i] += _translation;
    }
    return;
}

#endif


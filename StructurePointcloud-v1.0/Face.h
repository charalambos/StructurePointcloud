////////////////////////////////////////////////////////////////////////////////////
// Copyright © Charalambos "Charis" Poullis, charalambos@poullis.org    	  //
// This work can only be used under an exclusive license of the author.           //
////////////////////////////////////////////////////////////////////////////////////


#ifndef __FACE_H__
#define __FACE_H__

/**
* 	Face
* This class defines the properties of a polygon/face
**/

#include <vector>
#include "Edge.h"

class Face {

	public:
		///Constructor
		Face()	{
			vertex_indices.clear();
			normal_indices.clear();
			tex_coord_indices.clear();
			edges.clear();
		}
		///Copy constructor
		Face(Face *_face) {
			vertex_indices.clear();
			vertex_indices.insert(vertex_indices.begin(), _face->vertex_indices.begin(), _face->vertex_indices.end());
			normal_indices.clear();
			normal_indices.insert(normal_indices.begin(), _face->normal_indices.begin(), _face->normal_indices.end());
			tex_coord_indices.clear();
			tex_coord_indices.insert(tex_coord_indices.begin(), _face->tex_coord_indices.begin(), _face->tex_coord_indices.end());
			edges.clear();
			edges.insert(edges.begin(), _face->edges.begin(), _face->edges.end());

		}

		///Destructor
		~Face()	{
			vertex_indices.clear();
			normal_indices.clear();
			tex_coord_indices.clear();
			edges.clear();
		}

		///Get values
		int getNumberOfVertices() {
			return vertex_indices.size();
		}
		int getNumberOfNormals() {
			return normal_indices.size();
		}
		int getNumberOfTextureCoords()	{
			return tex_coord_indices.size();
		}

		///Vertex functions
		void setVertices(std::vector<int> const &vertices)	{
			vertex_indices.clear();
			vertex_indices.insert(vertex_indices.begin(), vertices.begin(), vertices.end());
			return;
		}
		void setVertexAt(int index,int vertex)	{
			if (index>=0 && index<vertex_indices.size())	{
				vertex_indices[index] = vertex;
			}
			return;
		}

		///Normal functions
		void setNormals(std::vector<int> const &normals)	{
			normal_indices.clear();
			normal_indices.insert(normal_indices.begin(), normals.begin(), normals.end());
			return;
		}
		void setNormalAt(int index,int normal)	{
			if (index>=0 && index<normal_indices.size()) {
				normal_indices[index] = normal;
			}
			return;
		}

		///Texture coordinate functions
		void setTextureCoords(std::vector<int> const &textureCoords)	{
			tex_coord_indices.clear();
			tex_coord_indices.insert(tex_coord_indices.begin(), textureCoords.begin(), textureCoords.end());
			return;
		}
		void setTextureCoord(int index, int tex_coord)	{
			if (index>=0 && index<tex_coord_indices.size())	{
				tex_coord_indices[index] = tex_coord;
			}
			return;
		}

		///Edge functions
		void setEdges(std::vector<Edge *> const &_edges)	{
			edges.clear();
			edges.insert(edges.begin(), _edges.begin(), _edges.end());
			return;
		}
		void setEdge(int index, Edge *edge)	{
			if (index>=0 && index<edges.size())	{
				edges[index] = edge;
			}
			return;
		}

		///Get vertices
		std::vector<int> getVertices()	{
			return vertex_indices;
		}
		///Get vertex at
		int getVertexAt(int index) {
			return vertex_indices[index];
		}
		///Get normals
		std::vector<int> getNormals()	{
			return normal_indices;
		}
		///Get normal at
		int getNormalAt(int index) {
			return normal_indices[index];
		}
		///Get textureCoords
		std::vector<int> getTextureCoords() {
			return tex_coord_indices;
		}
		///Get textureCoord at
		int getTextureCoordAt(int index) {
			return tex_coord_indices[index];
		}
		///Get edges
		std::vector<Edge *> getEdges()	{
			return edges;
		}
		///Get edge at
		Edge *getEdgeAt(int index)	{
			return edges[index];
		}

		///Set everything
		void setFace(std::vector<int> 		const &vertices,
			     std::vector<int> 		const &normals,
			     std::vector<int> 		const &tex_coords,
			     std::vector<Edge *>	const &_edges)	{

			vertex_indices.clear();
			vertex_indices.insert(vertex_indices.begin(), vertices.begin(), vertices.end());

			normal_indices.clear();
			normal_indices.insert(normal_indices.begin(), normals.begin(), normals.end());

			tex_coord_indices.clear();
			tex_coord_indices.insert(tex_coord_indices.begin(), tex_coords.begin(), tex_coords.end());

			edges.clear();
			edges.insert(edges.begin(), _edges.begin(), _edges.end());

			return;
		}

		void setFace(std::vector<int> 	const &vertices,
			     std::vector<int> 	const &normals,
			     std::vector<int> 	const &tex_coords)	{

			vertex_indices.clear();
			vertex_indices.insert(vertex_indices.begin(), vertices.begin(), vertices.end());

			normal_indices.clear();
			normal_indices.insert(normal_indices.begin(), normals.begin(), normals.end());

			tex_coord_indices.clear();
			tex_coord_indices.insert(tex_coord_indices.begin(), tex_coords.begin(), tex_coords.end());

			return;
		}

		void setFace(std::vector<int> 	const &vertices,
			     std::vector<int> 	const &normals)	{

			vertex_indices.clear();
			vertex_indices.insert(vertex_indices.begin(), vertices.begin(), vertices.end());

			normal_indices.clear();
			normal_indices.insert(normal_indices.begin(), normals.begin(), normals.end());

			return;
		}

		void setFace(std::vector<int> 	const &vertices)	{

			vertex_indices.clear();
			vertex_indices.insert(vertex_indices.begin(), vertices.begin(), vertices.end());

			return;
		}
	private:
		///A list of vertex indices
		std::vector<int>	vertex_indices;
		///A list of normal indices
		std::vector<int>	normal_indices;
		///A list of tex coord indices
		std::vector<int>	tex_coord_indices;
		///A list of edges
		std::vector<Edge *>	edges;

};


#endif

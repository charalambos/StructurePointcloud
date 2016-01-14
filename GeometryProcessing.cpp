////////////////////////////////////////////////////////////////////////////////////
// Copyright © Charalambos "Charis" Poullis, charalambos@poullis.org    	  //
// This work can only be used under an exclusive license of the author.           //
////////////////////////////////////////////////////////////////////////////////////
#ifndef __GEOMETRY_PROCESSING_CPP__
#define __GEOMETRY_PROCESSING_CPP__

#include "GeometryProcessing.h"
#include "Color.h"

GeometricObject *GeometryProcessing::triangulate(Image *xyz_map, Image *_normal_map)	{

	if (!xyz_map)	{
		return 0x00;
	}

	///Compute the normal map from the xyz map
	Image *normal_map = 0x00;
	if (_normal_map)	{
		normal_map = _normal_map;
	}
	else	{
		normal_map = GeometryProcessing::computeNormalMap(xyz_map, true);
	}

	///Measure the number of valid points and allocate memory
	int number_of_valid_points = 0;
	for (int y=0;y<xyz_map->getHeight();y++)	{
		for (int x=0;x<xyz_map->getWidth();x++)	{
			if (xyz_map->getPixel(x,y) == Color(0.0f,0.0f,0.0f))	continue;
			number_of_valid_points++;
		}
	}
	///create a connected mesh of the same size as the xyz map
	std::vector<Vector3f> new_vertices,normals;
	std::vector<Face *> new_faces;
	int vertex_count = 0 ;
	Image *already_added = new Image(xyz_map->getWidth(),xyz_map->getHeight(), -1.0f,-1.0f,-1.0f,1.0f);
	for (int y=0;y<xyz_map->getHeight();y++)	{
		for (int x=0;x<xyz_map->getWidth();x++)	{
			bool stop_it = false;
			int offset_x=1;
			int offset_y=1;
			if (xyz_map->getPixel(x,y)==Color(0.0f,0.0f,0.0f)) continue;
			if (y+offset_y >= xyz_map->getHeight() || x+offset_x >= xyz_map->getWidth()) continue;
			while (xyz_map->getPixel(x+offset_x,y)==Color(0.0f,0.0f,0.0f))	{
				offset_x++;
				if (x+offset_x>=xyz_map->getWidth()) {
					stop_it = true;
					break;
				}
			}
			if (stop_it) continue;
			while (xyz_map->getPixel(x,y+offset_y) == Color(0.0f,0.0f,0.0f))	{
				offset_y++;
				if (y+offset_y>=xyz_map->getHeight()) {
					stop_it = true;
					break;
				}
			}
			if (stop_it) continue;
			if (y+offset_y >= xyz_map->getHeight() || x+offset_x >= xyz_map->getWidth()) continue;
			if (xyz_map->getPixel(x+offset_x, y+offset_y) == Color(0.0f,0.0f,0.0f)) continue;
			std::vector<int> vertex_indices;
			int index1,index2,index3,index4;
			//add the points
			if (already_added->getPixel(x,y) == Color(-1.0f,-1.0f,-1.0f))	{
				Vector3f point1 = color2vector3(xyz_map->getPixel(x,y));
				new_vertices.push_back(point1);
				Vector3f normal1 = color2vector3(normal_map->getPixel(x,y));
				normals.push_back(normal1);
				index1 = vertex_count;
				already_added->setPixel(x,y,Color(float(vertex_count)));
				vertex_count++;
			}
			else	{
				index1 = int(already_added->getPixel(x,y).r());
			}
			if (already_added->getPixel(x+offset_x,y) == Color(-1.0f,-1.0f,-1.0f))	{
				Vector3f point2 = color2vector3(xyz_map->getPixel(x+offset_x,y));
				new_vertices.push_back(point2);
				Vector3f normal2 = color2vector3(normal_map->getPixel(x+offset_x,y));
				normals.push_back(normal2);
				index2 = vertex_count;
				already_added->setPixel(x+offset_x,y,Color(float(vertex_count)));
				vertex_count++;
			}
			else	{
				index2 = int(already_added->getPixel(x+offset_x,y).r());
			}
			if (already_added->getPixel(x,y+offset_y)==Color(-1.0f,-1.0f,-1.0f))	{
				Vector3f point3 = color2vector3(xyz_map->getPixel(x,y+offset_y));
				new_vertices.push_back(point3);
				Vector3f normal3 = color2vector3(normal_map->getPixel(x,y+offset_y));
				normals.push_back(normal3);
				index3 = vertex_count;
				already_added->setPixel(x,y+offset_y, Color(float(vertex_count)));
				vertex_count++;
			}
			else	{
				index3 = int(already_added->getPixel(x,y+offset_y).r());
			}
			if (already_added->getPixel(x+offset_x, y+offset_y) == Color(-1.0f,-1.0f,-1.0f))	{
				Vector3f point4 = color2vector3(xyz_map->getPixel(x+offset_x, y+offset_y));
				new_vertices.push_back(point4);
				Vector3f normal4 = color2vector3(normal_map->getPixel(x+offset_x,y+offset_y));
				normals.push_back(normal4);
				index4 = vertex_count;
				already_added->setPixel(x+offset_x,y+offset_y,Color(float(vertex_count)));
				vertex_count++;
			}
			else	{
				index4 = int(already_added->getPixel(x+offset_x, y+offset_y).r());
			}
			//Create 2 faces for each of the triangles form
			//printf("%d %d %d %d\n",index1,index2,index3,index4);
			vertex_indices.push_back(index1);
			vertex_indices.push_back(index3);
			vertex_indices.push_back(index2);
			//create a new face
			Face *new_face_a = new Face();
			new_face_a->setVertices(vertex_indices);
			new_faces.push_back(new_face_a);

			vertex_indices.clear();
			vertex_indices.push_back(index2);
			vertex_indices.push_back(index3);
			vertex_indices.push_back(index4);
			//create a new face
			Face *new_face_b = new Face();
			new_face_b->setVertices(vertex_indices);
			new_faces.push_back(new_face_b);
		}
	}

	///add a new object
	std::vector<Vector2f> tex_coords;
	std::vector<Edge *> edges;

	GeometricObject *new_object = new GeometricObject(new_vertices,normals,tex_coords,new_faces,edges);

	delete already_added;
	if (_normal_map == 0x00)	{
		delete normal_map;
	}

 	return new_object;
}

Image *GeometryProcessing::computeNormalMap(Image *xyz_map, bool z_up)      {
         Image *normal_map = new Image(xyz_map->getWidth(),xyz_map->getHeight(),0.0f,0.0f,0.0f,1.0f);
        for (int y=0;y<xyz_map->getHeight();y++) {
                for (int x=0;x<xyz_map->getWidth();x++) {
                		if (xyz_map->getPixel(x,y) != Color(0.0f,0.0f,0.0f))	{
                			normal_map->setPixel(x,y,vector2color3(computeLocalNormal(xyz_map,Vector2i(x,y),z_up)));
                		}
                }
        }
        return normal_map;
}

Vector3f GeometryProcessing::computeLocalNormal(Image *xyz_map, Vector2i const &index, bool z_up)        {

        //get an average normal for this point
        //check the  8 neighbours of the pixel in counter clockwise order
		std::vector<Vector2i> indices;
		indices.push_back(Vector2i(index(0)-1,index(1)+1));
		indices.push_back(Vector2i(index(0)-1,index(1)));
		indices.push_back(Vector2i(index(0)-1,index(1)-1));
		indices.push_back(Vector2i(index(0),index(1)-1));
		//index5 is the pixel in question
		indices.push_back(Vector2i(index(0)+1,index(1)-1));
		indices.push_back(Vector2i(index(0)+1,index(1)));
		indices.push_back(Vector2i(index(0)+1,index(1)+1));
		indices.push_back(Vector2i(index(0),index(1)+1));

		std::vector<Vector3f> good_points;
		for (int i=0;i<indices.size();i++)	{
			if (outOfBounds(xyz_map, indices[i](0),indices[i](1)))	continue;
			if (xyz_map->getPixel(indices[i](0), indices[i](1)) == Color(0.0f,0.0f,0.0f))	continue;
			good_points.push_back(color2vector3(xyz_map->getPixel(indices[i](0),indices[i](1))));
		}

		std::vector<Vector3f> diff_vectors;
		for (int i=0;i<good_points.size();i++)	{
			diff_vectors.push_back(good_points[i] - color2vector3(xyz_map->getPixel(index(0),index(1))));
		}

		Vector3f normal = Vector3f(0.0f,0.0f,0.0f);
		for (int i=0;i<diff_vectors.size();i++)	{
			int next = (i+1)%diff_vectors.size();
			Vector3f current_normal = diff_vectors[i].cross(diff_vectors[next]);
			if (current_normal.norm() > EPSILON)    current_normal.normalize();
	        ///THIS IS ADDED FOR AERIAL LIDAR DATA ONLY. THE Z SHOULD POINT UP
			if (z_up && current_normal(2) < 0.0f) {
				current_normal = -current_normal;
			}
			normal += current_normal;
		}

        normal /= float(std::max(1,int(diff_vectors.size())));
        if (normal.norm() > EPSILON)    {
            normal.normalize();
        }

	return normal;
}

#endif

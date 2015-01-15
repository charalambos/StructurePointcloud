////////////////////////////////////////////////////////////////////////////////////
// Copyright © Charalambos "Charis" Poullis, charalambos@poullis.org    	  //
// This work can only be used under an exclusive license of the author.           //
//////////////////////////////////////////////////////////////////////////////////// 

#ifndef __EDGE_H__
#define __EDGE_H__


/**	Edge
* This class defines an edge connecting 2 points
*
**/


class Edge	{
	public:	
		///Constructor 2
		Edge(Edge *_edge)	{
			vertex1 = _edge->vertex1; 
			vertex2 = _edge->vertex2;
		}
	
		///Copy constructor
		Edge(Edge const &_edge)	{ 
			vertex1 = _edge.vertex1; 
			vertex2 = _edge.vertex2;
		}

		///Constructor 3
		Edge(int v1, int v2)	{	
			vertex1 = v1; 
			vertex2 = v2;
		}
		
		///Get values
		int getVertex1()	{
			return vertex1;
		}

		int getVertex2()	{
			return vertex2;
		}

	private:
		///The two vertices
		int vertex1,vertex2;

		///Constructor
		Edge()	{;}

}; 

#endif


////////////////////////////////////////////////////////////////////////////////////
// Copyright © Charalambos "Charis" Poullis, charalambos@poullis.org    	  //
// This work can only be used under an exclusive license of the author.           //
//////////////////////////////////////////////////////////////////////////////////// 
#ifndef __SORTER_H__
#define __SORTER_H__

#include <stdio.h>
#include "Vector.h"
#include <vector>

template<class T, class R, int S>
class SorterComplexType	{
	public:
		static bool quicksort(std::vector<T> &array, int bottom, int top, std::vector<R> *helper_array=0x00)	{
			// uses recursion-the process of calling itself
			int middle;
			if(bottom<top)	   {
				middle = partitionArray(array, bottom, top,helper_array);
				quicksort(array, bottom, middle,helper_array);   // sort top partition
				quicksort(array, middle+1, top,helper_array);    // sort bottom partition
			}
			return true;
		}

		static bool quicksort(std::vector<Vector<T,S> > &array, int bottom, int top, int element, std::vector<R> *helper_array=0x00)	{
			// uses recursion-the process of calling itself
			int middle;
			if(bottom<top)	   {
				middle = partitionArray(array, bottom, top,element,helper_array);
				quicksort(array, bottom, middle,element,helper_array);   // sort top partition
				quicksort(array, middle+1, top,element,helper_array);    // sort bottom partition
			}
			return true;
		}
	private:
		SorterComplexType<T,R,S>()	{;}
		~SorterComplexType<T,R,S>()	{;}

		// partitions the array and returns the middle index (subscript)
		static int partitionArray(std::vector<T> &array, int bottom, int top, std::vector<R> *helper_array=0x00)	{
			T x = array[bottom];
			int i = bottom - 1;
			int j = top + 1;
			T temp;
			R temp2;
			do	{
				do	{
						j--;
					} while (x <array[j]);
				do	{
						i++;
					} while (x >array[i]);
				if (i < j)	{
						temp = array[i];    // switch elements at positions i and j
						array[i] = array[j];
						array[j] = temp;
						if (helper_array) {
							temp2 = (*helper_array)[i];    // switch elements at positions i and j
							(*helper_array)[i] = (*helper_array)[j];
							(*helper_array)[j] = temp2;
						}
				}
			} while (i < j);
			return j;           // returns middle index
		}

// partitions the array and returns the middle index (subscript)
		static int partitionArray(std::vector<Vector<T,S> > &array, int bottom, int top, int element, std::vector<R> *helper_array=0x00)	{
			T x = array[bottom](element);
			int i = bottom - 1;
			int j = top + 1;
			Vector<T,S> temp;
			R temp2;
			do	{
				do	{
						j--;
					} while (x <array[j](element));
				do	{
						i++;
					} while (x >array[i](element));
				if (i < j)	{
						temp = array[i];    // switch elements at positions i and j
						array[i] = array[j];
						array[j] = temp;
						if (helper_array) {
							temp2 = (*helper_array)[i];    // switch elements at positions i and j
							(*helper_array)[i] = (*helper_array)[j];
							(*helper_array)[j] = temp2;
						}
				}
			} while (i < j);
			return j;           // returns middle index
		}

};


//Custom types
typedef SorterComplexType<unsigned int,unsigned int,2>		SorterComplexType2ui;
typedef SorterComplexType<unsigned int,unsigned int,3>		SorterComplexType3ui;
typedef SorterComplexType<unsigned int,unsigned int,4>		SorterComplexType4ui;

typedef SorterComplexType<int,int,2>				SorterComplexType2i;
typedef SorterComplexType<int,int,3>				SorterComplexType3i;
typedef SorterComplexType<int,int,4>				SorterComplexType4i;

typedef SorterComplexType<float,float,2>			SorterComplexType2f;
typedef SorterComplexType<float,float,3>			SorterComplexType3f;
typedef SorterComplexType<float,float,4>			SorterComplexType4f;

typedef SorterComplexType<float,Vector2f,2>			SorterComplexType2vf;
typedef SorterComplexType<float,Vector3f,3>			SorterComplexType3vf;

typedef SorterComplexType<double,double,2>			SorterComplexType2d;
typedef SorterComplexType<double,double,3>			SorterComplexType3d;
typedef SorterComplexType<double,double,4>			SorterComplexType4d;

#endif

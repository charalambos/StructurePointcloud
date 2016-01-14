////////////////////////////////////////////////////////////////////////////////////
// Copyright © Charalambos "Charis" Poullis, charalambos@poullis.org    	  //
// This work can only be used under an exclusive license of the author.           //
////////////////////////////////////////////////////////////////////////////////////
#ifndef __SORTER_H__
#define __SORTER_H__

#include <stdio.h>
#include "Eigen/Eigen"
using namespace Eigen;
#include <vector>

class SorterComplexType	{
	public:
		static bool quicksort(std::vector<float> &_array, int bottom, int top, std::vector<Vector3f> *helper_array=0x00)	{
			// uses recursion-the process of calling itself
			int middle;
			if(bottom<top)	   {
				middle = partitionArray(_array, bottom, top,helper_array);
				quicksort(_array, bottom, middle,helper_array);   // sort top partition
				quicksort(_array, middle+1, top,helper_array);    // sort bottom partition
			}
			return true;
		}

		static bool quicksort(std::vector<Vector3f> &_array, int bottom, int top, int element, std::vector<Vector3f> *helper_array=0x00)	{
			// uses recursion-the process of calling itself
			int middle;
			if(bottom<top)	   {
				middle = partitionArray(_array, bottom, top,element,helper_array);
				quicksort(_array, bottom, middle,element,helper_array);   // sort top partition
				quicksort(_array, middle+1, top,element,helper_array);    // sort bottom partition
			}
			return true;
		}
	private:
		SorterComplexType()	{;}
		~SorterComplexType()	{;}

		// partitions the array and returns the middle index (subscript)
		static int partitionArray(std::vector<float> &_array, int bottom, int top, std::vector<Vector3f> *helper_array=0x00)	{
			float x = _array[bottom];
			int i = bottom - 1;
			int j = top + 1;
			float temp;
			Vector3f temp2;
			do	{
				do	{
						j--;
					} while (x <_array[j]);
				do	{
						i++;
					} while (x >_array[i]);
				if (i < j)	{
						temp = _array[i];    // switch elements at positions i and j
						_array[i] = _array[j];
						_array[j] = temp;
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
		static int partitionArray(std::vector<Vector3f> &_array, int bottom, int top, int element, std::vector<Vector3f> *helper_array=0x00)	{
			float x = _array[bottom][element];
			int i = bottom - 1;
			int j = top + 1;
			Vector3f temp;
			Vector3f temp2;
			do	{
				do	{
						j--;
					} while (x <_array[j][element]);
				do	{
						i++;
					} while (x >_array[i][element]);
				if (i < j)	{
						temp = _array[i];    // switch elements at positions i and j
						_array[i] = _array[j];
						_array[j] = temp;
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

#endif

////////////////////////////////////////////////////////////////////////////////////
// Copyright © Charalambos "Charis" Poullis, charalambos@poullis.org    	      //
// This work can only be used under an exclusive license of the author.           //
////////////////////////////////////////////////////////////////////////////////////

#ifndef __VECTOR_H__
#define __VECTOR_H__

#include <stdarg.h>
#include <boost/array.hpp>
#include <math.h>
#include <vector>

#define VECTOR_EPSILON 1e-03

template<typename T, int D>
class Vector	{
	public:
		/**
		 * 		Constructor:
		 *  Initializes all values to zero.
		 */
		Vector<T,D>()	{
			Zero();
		}

		/**
		 *		Copy constructor
		 *  Copies the data from an input vector.
		 * @param copy_vec A const reference to the vector which will be copied.
		 */
		template<class T1>
		Vector<T,D>(Vector<T1,D> const &copy_vec)	{
			for (unsigned int i=0;i<D;i++)	{
				data[i] = (T) copy_vec[i];
			}
		}

		/**
		 *		Constructor:
		 * Initializes the values of the vector to whatever the input is.
		 * @param first The input values which will be used to initialize the values of the vector.
		 * If the values are not of the same type as the vector they will be casted as such.
		 * If a value is of a different data type i.e "a" then it will be skipped.
		 */
		Vector<T,D>(unsigned int first, ...)	{
			va_list a_list;
			//Initialize the va_list i.e T variable 'list' by the address
			//of first unknown variable argument by a call of va_start() macro
			va_start(a_list, first);
			//In loop, retrieve each argument
			//second argument to va_arg is datatype
			//of expected argument
			data[0] = (unsigned int) first;
			for (int i=1;i<D;i++)	{
				unsigned int value = va_arg(a_list,unsigned int);
				data[i] = value;
			}
			va_end(a_list);
		}

		Vector<T,D>(int first, ...)	{
			va_list a_list;
			//Initialize the va_list i.e T variable 'list' by the address
			//of first unknown variable argument by a call of va_start() macro
			va_start(a_list, first);
			//In loop, retrieve each argument
			//second argument to va_arg is datatype
			//of expected argument
			data[0] = (int) first;
			for (int i=1;i<D;i++)	{
				int value = va_arg(a_list,int);
				data[i] = value;
			}
			va_end(a_list);
		}

		Vector<T,D>(float first, ...)	{
			va_list a_list;
			//Initialize the va_list i.e T variable 'list' by the address
			//of first unknown variable argument by a call of va_start() macro
			va_start(a_list, first);
			//In loop, retrieve each argument
			//second argument to va_arg is datatype
			//of expected argument
			data[0] = (float) first;
			for (int i=1;i<D;i++)	{
				float value = (float) va_arg(a_list,double);
				data[i] = value;
			}
			va_end(a_list);
		}

		Vector<T,D>(double first, ...)	{
			va_list a_list;
			//Initialize the va_list i.e T variable 'list' by the address
			//of first unknown variable argument by a call of va_start() macro
			va_start(a_list, first);
			//In loop, retrieve each argument
			//second argument to va_arg is datatype
			//of expected argument
			data[0] = (double) first;
			for (int i=1;i<D;i++)	{
				double value = va_arg(a_list,double);
				data[i] = value;
			}
			va_end(a_list);
		}


		/**
		 *               Destructor
		 */
		~Vector()	{
		}

		std::vector<T> toStdVector() {
			std::vector<T> v;
			for (int i=0;i<D;i++)
				v.push_back(data[i]);
			return v;
		}


			//Initialize to zero
			void Zero()						{for (int i=0;i<D;i++)	data[i] = (T) 0.0;}
			void Negate(void)					{for (int i=0;i<D;i++)	data[i] = -data[i];}

			//Normalize
			void Normalize(void)				{T mag = GetMagnitude(); if (mag < (T) VECTOR_EPSILON) mag=(T) 1.0; for (int i=0;i<D;i++)	data[i] /= mag;}

			//Scramble
					//scrabble for 2d vectors
			void scramble(void)	{		if (D!=2)	return;
										T _x;T _y = data[0];
										_x =  data[1] * 1234.12345054321; 	data[0] = _x - (int)_x;
										_y = _y * 7654.54321012345;				data[1] = _y - (int)_y;
								}
			//Magnitude functions
			T length(void)	const			{T mag=(T)0.0; for (int i=0;i<D;i++)  mag += data[i]*data[i]; return sqrt(mag);}
			T GetMagnitude(void) const {
                T mag=(T)0.0;
                for (int i=0;i<D;i++)
                    mag += data[i]*data[i];
                if (mag < (T) VECTOR_EPSILON) {
                    return (T) 0.0;
                }
                else {
                    return sqrt(mag);
                }
            }
			T squaredLength(void)	const		{T mag=(T)0.0; for (int i=0;i<D;i++)  mag += data[i]*data[i]; return mag;}
			T GetMagnitudeSquared(void)	const		{T mag=(T)0.0; for (int i=0;i<D;i++)  mag += data[i]*data[i]; return mag;}
			boost::array<T,D> GetArray(void) const					{return data;}

			//access the elements
			T operator() ( int elementNumber) const	{return data[elementNumber];}
			T operator[] ( int elementNumber) const	{return data[elementNumber];}
			T& operator() ( int elementNumber) 	{return data[elementNumber];}
			T& operator[] ( int elementNumber) 	{return data[elementNumber];}
			T x()	const							{return data[0];}
			T y()	const							{return data[1];}
			T z()	const							{return data[2];}
			T& x()								{return data[0];}
			T& y()								{return data[1];}
			T& z()								{return data[2];}

			bool operator<( const Vector<T,D> *vec) const {
				T diff = (T) 0.0;
				for (int i=0;i<D;i++)	{
					diff += (this->data[i] - vec->data[i])*(this->data[i] - vec->data[i]);
				}
				if (diff > VECTOR_EPSILON)	diff = (T) sqrt(diff);
				if (diff > 0)	return true;
				else 			return false;
			}

			void setX(T x)	{data[0] = x;}
			void setY(T y) 	{data[1] = y;}
			void setZ(T z)	{data[2] = z;}

			//operators
			Vector<T,D> &operator+= ( Vector<T,D> const &vec)	{for (int i=0;i<D;i++)	data[i] += vec.data[i];return *this;}
			Vector<T,D> &operator-= ( Vector<T,D> const &vec) {for (int i=0;i<D;i++)	data[i] -= vec.data[i];return *this;}
			Vector<T,D> &operator*= ( T scalar)			{for (int i=0;i<D;i++)	data[i] *= scalar;return *this;}
			Vector<T,D> &operator/= ( T scalar)			{for (int i=0;i<D;i++)	data[i] /= scalar;return *this;}

		/**
		 *               Accessor function with operator ()
		 * @param index Index to the element number.
		 * @return The value of the element at the index.
		 */
// 		T &operator() (unsigned int index) {
// 			if (index>=0 && index<S)	{
// 				return data[index];
// 			}
// 			else	{
// 				return (T) INVALID_VALUE;
// 			}
// 		}
//
// 		T const &operator() (unsigned int index) const {
// 			return data[index];
// 		}
//
// 		T &operator[] (unsigned int index) {
// 			return data[index];
// 		}
//
// 		T const &operator[] (unsigned int index) const {
// 			return data[index];
// 		}

		/** Prints out the contents of the vector data to an std::ostream object
		 *
		 * @param os Reference to the std::ostream object
		 * @param vec Const Reference to the vector which will be printed
		 * @return The std::ostream object with the printed data
		 */
// 		friend std::ostream &operator<<(std::ostream& os, Vector<T,D> const &vec)	{
// 			for (unsigned int i=0;i<S;i++)	{
// 				os  << vec[i] << " ";
// 			}
// 			return os;
// 		}
//
// 		T x() const	{ if (S >= 0) return data[0]; }
// 		T& x() 		{ if (S >= 0) return data[0]; }
//
// 		T y() const	{ if (S >= 1) return data[1]; }
// 		T& y() 		{ if (S >= 1) return data[1]; }
//
// 		T z() const	{ if (S >= 2) return data[2]; }
// 		T& z()		{ if (S >= 2) return data[2]; }

	protected:
		//Friend functions
		friend std::ostream &operator<<(std::ostream &os, Vector<T,D> const &vec) {
			os << "(";
			for (int i=0;i<D;i++)
				os << (T) vec.data[i] << ' ';
			os<< ")";
			return os;
		}
		friend std::istream &operator>>(std::istream& is, Vector<T,D> &vec)	{
			T temp;
			for (int i=0;i<D;i++)
				is >> vec.data[i];
			return is;
		}
		friend Vector<T,D> operator+ ( Vector<T,D> const &vec1,  Vector<T,D> const &vec2)	{
			Vector<T,D> result;
			for (int i=0;i<D;i++)
				result.data[i] = vec1.data[i]+vec2.data[i];
			return result;
		}
		friend Vector<T,D> operator-  ( Vector<T,D> const &vec)	{
			Vector<T,D> result;
			for (int i=0;i<D;i++)
				result.data[i] = -vec.data[i];
			return result;
		}

		friend Vector<T,D> operator- ( Vector<T,D> const &vec1,  Vector<T,D> const &vec2)	{
			Vector<T,D> result;
			for (int i=0;i<D;i++)
				result.data[i] = vec1.data[i]-vec2.data[i];
			return result;
		}
		friend Vector<T,D> operator* ( Vector<T,D> const &vec,  T const scalar)	{
			Vector<T,D> result;
			for (int i=0;i<D;i++)
				result.data[i] = vec.data[i]*scalar;
			return result;
		}
		friend Vector<T,D> operator* ( T const scalar,  Vector<T,D> const &vec)	{
			Vector<T,D> result;
			for (int i=0;i<D;i++)
				result.data[i] = vec.data[i]*scalar;
			return result;
		}
		friend Vector<T,D> operator/ ( Vector<T,D> const &vec,  T scalar)	{
			Vector<T,D> result;
			for (int i=0;i<D;i++)
				result.data[i] = vec.data[i]/scalar;
			return result;
		}
		friend bool operator== ( Vector<T,D> const &vec1,  Vector<T,D> const &vec2)	{
			for (int i=0;i<D;i++)
				if (fabs(vec1.data[i]-vec2.data[i]) > VECTOR_EPSILON)	return false;
			return true;
		}
		friend bool operator!= ( Vector<T,D> const &vec1,  Vector<T,D> const &vec2)	{
			for (int i=0;i<D;i++)
				if (fabs(vec1.data[i]-vec2.data[i]) > VECTOR_EPSILON)	return true;
			return false;
		}
		friend Vector<T,D> UnitVector(Vector<T,D> const &vec)	{
			T denom = sqrt(vec(0)*vec(0) + vec(1)*vec(1) + vec(2)*vec(2));
			T k;
			if (denom!= T(0)) k = T(1.0 /denom) ;	else k = T(1);
			return Vector<T,D>(vec(0)*k, vec(1)*k, vec(2)*k);
		}
		friend Vector<T,D> unitVector(Vector<T,D> const &vec)	{
			T k = 1.0 / sqrt(vec(0)*vec(0) + vec(1)*vec(1) + vec(2)*vec(2));
			return Vector<T,D>(vec(0)*k, vec(1)*k, vec(2)*k);
		}
		//reflect the vector around the normal
		friend Vector<T,D> reflect(Vector<T,D> const &in, Vector<T,D> const &normal)	{
			if (D!=3)	return in;
			// assumes unit length normal
			return in - normal * (2.0 * DotProduct(in, normal));
		}
		//Products
		friend T dot( Vector<T,D> const &vec1,  Vector<T,D> const &vec2)	{
			T result = (T) 0.0;
			for (int i=0;i<D;i++)
				result += vec1.data[i]*vec2.data[i];
			return result;
		}
		friend T DotProduct( Vector<T,D> const &vec1,  Vector<T,D> const &vec2)	{
			T result = (T) 0.0;
			for (int i=0;i<D;i++)
				result += vec1.data[i]*vec2.data[i];
			return result;
		}
		friend Vector<T,D> cross( Vector<T,D> const &vec1, Vector<T,D> const &vec2)	{
			if (D==3)	{
				return (Vector<T,D>((vec1.data[1]*vec2.data[2]) - (vec1.data[2]*vec2.data[1]),
						    (vec1.data[2]*vec2.data[0]) - (vec1.data[0]*vec2.data[2]),
						    (vec1.data[0]*vec2.data[1]) - (vec1.data[1]*vec2.data[0])));
			}
			else	{
				return Vector<T,D>(T(0),T(0),T(0));
			}
		}
		friend Vector<T,D> CrossProduct( Vector<T,D> const &vec1, Vector<T,D> const &vec2)	{
			if (D==3)	{
				return (Vector<T,D>((vec1.data[1]*vec2.data[2]) - (vec1.data[2]*vec2.data[1]),
						    (vec1.data[2]*vec2.data[0]) - (vec1.data[0]*vec2.data[2]),
						    (vec1.data[0]*vec2.data[1]) - (vec1.data[1]*vec2.data[0])));
			}
			else	{
				return Vector<T,D>(T(0),T(0),T(0));
			}
		}
		//Linear Interpolation
		friend Vector<T,D> LinearInterpolation( Vector<T,D> const &vec1, Vector<T,D> const &vec2,double fraction)	{
			Vector<T,D> result;
			for (int i=0;i<D;i++)
				result[i] = vec1.data[i] + (vec2.data[i] - vec1.data[i])*fraction;
			return result;
		}

		template<class new_T> friend Vector<new_T,D> convert(Vector<T,D> const &vec)	{
			Vector<new_T,D> result;
			for (unsigned int i=0;i<D;i++)	{
				result[i] = new_T(vec.data[i]);
			}
			return result;
		}

	private:
		///the actual data
		boost::array<T,D> data;
};

///Instantiate the class
typedef	Vector<unsigned int,2>	Vector2ui;
typedef	Vector<unsigned int,3>	Vector3ui;
typedef	Vector<unsigned int,4>	Vector4ui;
typedef	Vector<unsigned int,5>	Vector5ui;

typedef	Vector<int,2> Vector2i;
typedef	Vector<int,3> Vector3i;
typedef	Vector<int,4> Vector4i;
typedef	Vector<int,5> Vector5i;

typedef	Vector<float,2> Vector2f;
typedef	Vector<float,3> Vector3f;
typedef	Vector<float,4> Vector4f;
typedef	Vector<float,5> Vector5f;
typedef	Vector<float,6> Vector6f;
typedef	Vector<float,7> Vector7f;

typedef	Vector<double,2> Vector2d;
typedef	Vector<double,3> Vector3d;
typedef	Vector<double,4> Vector4d;
typedef	Vector<double,5> Vector5d;


#endif

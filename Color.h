////////////////////////////////////////////////////////////////////////////////////
// Copyright © Charalambos "Charis" Poullis, charalambos@poullis.org    	  //
// This work can only be used under an exclusive license of the author.           //
////////////////////////////////////////////////////////////////////////////////////
#ifndef __COLOR_H__
#define __COLOR_H__

#include <math.h>
#include "Vector.h"

#define COLOR_EPSILON 1e-03

class Color	{
	public:
		///Constructors
		Color();
		Color(Color const &_color);
		Color(float _grayscale);
		Color(float _r, float _g, float _b);
		Color(float _r, float _g, float _b, float _a);
		///Destructor
		~Color();

		///Returns the color and opacity
		float r() const;
		float &r();
		float g() const;
		float &g();
		float b() const;
		float &b();
		float a() const;
		float &a();

		///Get a specific element
		float& operator() (int elementNumber);
		float operator() (int elementNumber) const;

		///Mathematical functions
		friend Color operator* ( Color const &c1,  Color const &c2);
		friend Color operator* ( Color const &c1,  float scalar);
		friend Color operator* ( float scalar,  Color const &c1);
		friend Color operator+ ( Color const &c1,  Color const &c2);
		friend Color operator- ( Color const &c1,  Color const &c2);
		friend Color operator/ ( Color const &c1,  Color const &c2);
		friend Color operator/ ( Color const &c1,  float scalar);
		friend bool operator== ( Color const &c1,  Color const &c2);
		friend bool operator!= ( Color const &c1,  Color const &c2);

		Color &operator+= (Color const &vec);
		Color &operator*= (float scalar);
		Color &operator/= (float scalar);

		///Conversion functions
		template<class T>
		friend Vector<T,3> color2vector3(Color const &c)	{
			return Vector<T,3>((T) c.r(), (T) c.g(), (T) c.b());
		}

		template<typename T>
		friend Vector<T,4> color2vector4(Color const &c)	{
			return Vector<T,4>((T) c.r(), (T) c.g(), (T) c.b(), (T) c.a());
		}

		template<typename T>
		friend Color vector2color3(Vector<T,3> const &v)	{
			return Color(float(v(0)), float(v(1)), float(v(2)), 1.0f);
		}

		template<typename T>
		friend Color vector2color4(Vector<T,4> const &v)	{
			return Color(float(v(0)), float(v(1)), float(v(2)), float(v(3)));
		}

		///Print functions
		friend std::ostream &operator<<(std::ostream &os, Color const &c) {
			os << "red: " << c.r() << " green: " << c.g() << " blue: " << c.b() << " alpha: " << c.a() << std::endl;
			return os;
		}

		friend std::istream &operator>>(std::istream& is, Color &c)	{
			is >> c.data[0];
			is >> c.data[1];
			is >> c.data[2];
			is >> c.data[3];
			return is;
		}

	private:
		Vector4f data;
};

#endif

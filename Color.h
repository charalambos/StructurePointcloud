////////////////////////////////////////////////////////////////////////////////////
// Copyright © Charalambos "Charis" Poullis, charalambos@poullis.org    	  //
// This work can only be used under an exclusive license of the author.           //
////////////////////////////////////////////////////////////////////////////////////
#ifndef __COLOR_H__
#define __COLOR_H__

#include <math.h>
#include <Eigen/Eigen>
using namespace Eigen;

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
		friend Color vector2color3(Vector3i const &v)	{
			return Color(float(v(0)), float(v(1)), float(v(2)), 1.0f);
		}

		friend Color vector2color3(Vector3f const &v)	{
			return Color(v(0), v(1), v(2), 1.0f);
		}

		friend Color vector2color3(Vector3d const &v)	{
			return Color(float(v(0)), float(v(1)), float(v(2)), 1.0f);
		}

		friend Color vector2color4(Vector4i const &v)	{
			return Color(float(v(0)), float(v(1)), float(v(2)), float(v(3)));
		}

		friend Color vector2color4(Vector4f const &v)	{
			return Color(float(v(0)), float(v(1)), float(v(2)), float(v(3)));
		}

		friend Color vector2color4(Vector4d const &v)	{
			return Color(float(v(0)), float(v(1)), float(v(2)), float(v(3)));
		}

		friend Vector3f color2vector3(Color const &c)	{
			return Vector3f(c.r(), c.g(), c.b());
		}

		friend Vector4f color2vector4(Color const &c)	{
			return Vector4f(c.r(), c.g(), c.b(), c.a());
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

Color vector2color3(Vector3i const &v);

Color vector2color3(Vector3f const &v);

Color vector2color3(Vector3d const &v);

Color vector2color4(Vector4i const &v);

Color vector2color4(Vector4f const &v);

Color vector2color4(Vector4d const &v);

Vector3f color2vector3(Color const &c);

Vector4f color2vector4(Color const &c);

#endif

#include<iostream>
using namespace std;

#ifndef STRUCT_H
#define STRUCT_H

class point
{
public:
	float x, y, z;
	point(float x = 0.0, float y = 0.0, float z = 0.0) { this->x = x; this->y = y; this->z = z; }

	void set(float x, float y, float z) { this->x = x; this->y = y; this->z = z; }

	point getnormal(point p1, point p2)
	{//normal for the tower
		double x01, y01, z01, x12, y12, z12, x20, y20, z20;
		x01 = p1.x - x;
		y01 = p1.y - y;
		z01 = p1.z - z;
		x12 = p2.x - p1.x;
		y12 = p2.y - p1.y;
		z12 = p2.z - p1.z;
		x20 = x - p2.x;
		y20 = y - p2.y;
		z20 = z - p2.z;
		float nx = y01*(-z20) - (-y20)*z01;
		float ny = z01*(-x20) - (-z20)*x01;
		float nz = x01*(-y20) - (-x20)*y01;
		float den = sqrt(nx*nx + ny*ny + nz*nz);
		if (den > 0.0)
		{
			nx /= den;
			ny /= den;
			nz /= den;
		}
		point result(nx, ny, nz);
		return result;
	}

	point operator+(const point& a)
	{
		point p;
		p.x = this->x + a.x;
		p.y = this->y + a.y;
		p.z = this->z + a.z;
		return p;
	}

	point operator-(const point& a)
	{
		point p;
		p.x = this->x - a.x;
		p.y = this->y - a.y;
		p.z = this->z - a.z;
		return p;
	}

	point operator/(int a)
	{
		point p;
		p.x = this->x / a;
		p.y = this->y / a;
		p.z = this->z / a;
		return p;
	}

	point operator*(float a)
	{
		point p;
		p.x = this->x * a;
		p.y = this->y * a;
		p.z = this->z * a;
		return p;
	}

	point matmult(float a[3][3])
	{
		point result;
		result.x = a[0][0] * x + a[0][1] * y + a[0][2] * z;
		result.y = a[1][0] * x + a[1][1] * y + a[1][2] * z;
		result.z = a[2][0] * x + a[2][1] * y + a[2][2] * z;
		return result;
	}

	void print()
	{
		cout << x << ' ' << y << ' ' << z << endl;
	}

	void norm()
	{
		float dist = sqrt(x*x + y*y + z*z);
		x /= dist;
		y /= dist;
		z /= dist;
	}

	point cross(point p)
	{
		point result;
		result.x = y*p.z - p.y*z;
		result.y = z*p.x - x*p.z;
		result.z = x*p.y - y*p.x;
		return result;
	}

	float length()
	{
		return sqrt(x*x + y*y + z*z);
	}
	~point() {};
};

#endif
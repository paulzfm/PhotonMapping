#include "Vector.h"
#include "common.h"

#include <math.h>

// This defines the location of x, y and z axis.
const Vector Vector::TOP = Vector(0, 1, 0);
const Vector Vector::RIGHT = Vector(1, 0, 0);

Vector::Vector(double x, double y, double z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}

double Vector::length() const
{
	return sqrt(x * x + y * y + z * z);
}

double Vector::square() const
{
	return x * x + y * y + z * z;
}

Vector Vector::normalize() const
{
	double inv = 1.0 / length();
	return Vector(inv * x, inv * y, inv * z);
}

double Vector::dot(const Vector& that) const
{
	return x * that.x + y * that.y + z * that.z;
}

Vector Vector::cross(const Vector& that) const
{
	return Vector(y * that.z - z * that.y,
				  z * that.x - x * that.z,
				  x * that.y - y * that.x);
}

Vector Vector::operator - () const
{
	return Vector(-x, -y, -z);
}

Vector Vector::operator + (const Vector& that) const
{
	return Vector(x + that.x, y + that.y, z + that.z);
}

Vector Vector::operator - (const Vector& that) const
{
	return Vector(x - that.x, y - that.y, z - that.z);
}

Vector Vector::operator * (const Vector& that) const
{
	return Vector(x * that.x, y * that.y, z * that.z);
}

Vector Vector::operator * (double t) const
{
	return Vector(x * t, y * t, z * t);
}

Vector Vector::operator / (double t) const
{
	return Vector(x / t, y / t, z / t);
}

void Vector::operator += (const Vector& that)
{
	x += that.x;
	y += that.y;
	z += that.z;
}

void Vector::operator -= (const Vector& that)
{
	x -= that.x;
	y -= that.y;
	z -= that.z;
}

void Vector::operator *= (const Vector& that)
{
	x *= that.x;
	y *= that.y;
	z *= that.z;
}

void Vector::operator *= (double t)
{
	x *= t;
	y *= t;
	z *= t;
}

void Vector::operator /= (double t)
{
	x /= t;
	y /= t;
	z /= t;
}

bool operator == (const Vector& v1, const Vector& v2)
{
	return ABS(v1.x - v2.x) < EPS && ABS(v1.y - v2.y) < EPS &&
		ABS(v1.z - v2.z); 
}

bool operator != (const Vector& v1, const Vector& v2)
{
	return !(v1 == v2);
}

Vector operator * (double t, const Vector& v)
{
	return Vector(t * v.x, t * v.y, t * v.z);
}

std::istream& operator >> (std::istream& is, Vector& v)
{
	double tmp;
	is >> tmp;
	v.x = tmp;
	is >> tmp;
	v.y = tmp;
	is >> tmp;
	v.z = tmp;
	return is;
}

std::ostream& operator << (std::ostream& os, const Vector &v)
{
	os << "(" << v.x << ", " << v.y << ", " << v.z << ")";
	return os;
}

double Vector::operator [] (int index) const
{
	switch (index) {
	case 0:
		return x;
	case 1:
		return y;
	case 2:
		return z;
	}

	return 0;
}

Vector Vector::noise(double delta) const
{
    double radius = drand48() * length() * delta;
    double theta = drand48() * PI * 2;

	double a = radius * cos(theta);
	double b = radius * sin(theta); 

	Vector u = cross(TOP);
	if (u.square() != 1)
		u = cross(RIGHT);

	Vector v = cross(u);

	return (*this * (length() - radius) + u * a + v * b).normalize();
}

double Vector::distance(const Vector& v1, const Vector& v2)
{
	return sqrt( (v1.x - v2.x) * (v1.x - v2.x) + (v1.y - v2.y) * (v1.y - v2.y)
		+ (v1.z - v2.z) * (v1.z - v2.z) );
}

double Vector::angleCos(const Vector& v1, const Vector& v2)
{
	return v1.dot(v2) / (v1.length() * v2.length());
}
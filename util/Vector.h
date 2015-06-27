#ifndef VECTOR_H_
#define VECTOR_H_

#include <iostream>

class Vector
{
public:
	Vector(double x = 0, double y = 0, double z = 0)
		: x(x), y(y), z(z) {}
	
	double length() const;     // Euclid norm: |x^2 + y^2 + z^2|
	double square() const;     // Square of Euclid norm
	Vector normalize() const;  // v / |v|
	double dot(const Vector& that) const;   // dot product: v1 . v2
	Vector cross(const Vector& that) const; // cross product: v1 X v2
	
	Vector operator - () const;					   // -v
	Vector operator + (const Vector& that) const;  // v1 + v2
	Vector operator - (const Vector& that) const;  // v1 - v2
	Vector operator * (const Vector& that) const;  // (v1.x * v2.x, v1.y * v2.y, v1.z * v2.z)
	Vector operator * (double t) const;            // v * t
	Vector operator / (double t) const;            // v / t
	void operator += (const Vector& that);   // v1 = v1 + v2
	void operator -= (const Vector& that);   // v1 = v1 - v2
	void operator *= (const Vector& that);	 // v = v1 * v2
	void operator *= (double t);			 // v = v * t
	void operator /= (double t);			 // v = v / t
	
	friend bool operator == (const Vector& v1, const Vector& v2);
	friend bool operator != (const Vector& v1, const Vector& v2);

	friend Vector operator * (double t, const Vector& v); // t * v

	friend std::istream& operator >> (std::istream& is, Vector& v);
	friend std::ostream& operator << (std::ostream& os, const Vector &v);

	double operator [] (int index) const;

	Vector noise(double delta) const;
	
	static double distance(const Vector& v1, const Vector& v2);
	static double angleCos(const Vector& v1, const Vector& v2); // cos <v1, v2>

	const static Vector TOP;
	const static Vector RIGHT;
	
	double x;
	double y;
	double z;
};

#endif // VECTOR_H_
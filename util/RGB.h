#ifndef RGB_H_
#define RGB_H_

#include <iostream>

class RGB
{
public:
	RGB() : r(0.0), g(0.0), b(0.0), a(1.0) {}
	RGB(double red, double green, double blue, double alpha = 1.0)
		: r(red), g(green), b(blue), a(alpha) {}
	
	bool isBlack();
	
	void scale();
	void min(const RGB& that);

	// calculations
	RGB operator + (const RGB& that) const;
	RGB operator + (double t) const;
	RGB operator * (double t) const;
	RGB operator * (const RGB& that) const;
	void operator += (const RGB& that);
	void operator += (double t);
	void operator *= (double t);
	
	friend bool operator == (const RGB& v1, const RGB& v2);
	friend bool operator != (const RGB& v1, const RGB& v2);

	friend RGB operator * (double t, const RGB& v); // t * v

	friend std::istream& operator >> (std::istream& is, RGB& v);
	friend std::ostream& operator << (std::ostream& os, const RGB &v);

	static RGB filter(const RGB& in, const RGB& ft);

	double r;
	double g;
	double b;
	double a;
};

#endif // RGB_H_
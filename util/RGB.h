#ifndef RGB_H_
#define RGB_H_

#include <iostream>

class RGB
{
public:
	RGB(double red = 0, double green = 0, double blue = 0)
		: r(red), g(green), b(blue) {}
	
	bool isBlack();
	
	void scale();
	void min(const RGB& that);

	// calculations
	RGB operator + (const RGB& that) const;
	RGB operator + (double t) const;
	RGB operator * (double t) const;
	RGB operator * (const RGB& that) const;
	RGB operator / (double t) const;
	void operator += (const RGB& that);
	void operator += (double t);
	void operator *= (double t);
	void operator /= (double t);
	
	friend bool operator == (const RGB& v1, const RGB& v2);
	friend bool operator != (const RGB& v1, const RGB& v2);

	friend RGB operator * (double t, const RGB& v); // t * v

	friend std::istream& operator >> (std::istream& is, RGB& v);
	friend std::ostream& operator << (std::ostream& os, const RGB &v);

	static RGB filter(const RGB& in, const RGB& ft);

	double r; // red
	double g; // green
	double b; // blue
};

#endif // RGB_H_
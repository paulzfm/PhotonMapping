#include "RGB.h"
#include "common.h"

bool RGB::isBlack()
{
	return (r == 0 && g == 0 && b == 0);
}

void RGB::scale()
{
	r = MIN(r, 1.0);
	g = MIN(g, 1.0);
	b = MIN(b, 1.0);
}

void RGB::min(const RGB& that)
{
	r = MIN(r, that.r);
	g = MIN(g, that.g);
	b = MIN(b, that.b);
}

RGB RGB::operator + (const RGB& that) const
{
	return RGB(r + that.r, g + that.g, b + that.b);
}

RGB RGB::operator + (double t) const
{
	return RGB(r + t, g + t, b + t);
}

RGB RGB::operator * (double t) const
{
	return RGB(r * t, g * t, b * t);
}

RGB RGB::operator * (const RGB& that) const
{
	return RGB(r * that.r, g * that.g, b * that.b);
}

RGB RGB::operator / (double t) const
{
	return RGB(r / t, g / t, b / t);
}

void RGB::operator += (const RGB& that)
{
	r += that.r;
	g += that.g;
	b += that.b;
}

void RGB::operator += (double t)
{
	r += t;
	g += t;
	b += t;
}

void RGB::operator *= (double t)
{
	r *= t;
	g *= t;
	b *= t;
}

void RGB::operator /= (double t)
{
	r /= t;
	g /= t;
	b /= t;
}

bool operator == (const RGB& v1, const RGB& v2)
{
	return ABS(v1.r - v2.r) < EPS && ABS(v1.g - v2.g) < EPS &&
		ABS(v1.b - v2.b); 
}

bool operator != (const RGB& v1, const RGB& v2)
{
	return !(v1 == v2);
}

RGB operator * (double t, const RGB& v)
{
	return RGB(t * v.r, t * v.g, t * v.b);
}

std::istream& operator >> (std::istream& is, RGB& v)
{
	double tmp;
	is >> tmp;
	v.r = tmp;
	is >> tmp;
	v.g = tmp;
	is >> tmp;
	v.b = tmp;
	return is;
}

std::ostream& operator << (std::ostream& os, const RGB &v)
{
	os << "(" << v.r << ", " << v.g << ", " << v.b << ")";
	return os;
}

RGB RGB::filter(const RGB& in, const RGB& ft)
{
	RGB out = ft;
	if (ft.r + ft.g + ft.b > 0) {
		out.r = (in.r + in.g + in.b) * out.r / (out.r + out.g + out.b);
		out.g = (in.r + in.g + in.b) * out.g / (out.r + out.g + out.b);
		out.b = (in.r + in.g + in.b) * out.b / (out.r + out.g + out.b);
	} else {
		out.r = MIN(in.r, out.r);
		out.g = MIN(in.g, out.g);
		out.b = MIN(in.b, out.b);
	}

	return out;
}
#include "RGB.h"
#include "common.h"

void RGB::scale()
{
	double min = MIN(MIN(0.0, r), MIN(g, b));
	double max = 1.0;
	if (min < 0) {
		(*this) += -min;
		max -= min;
	}

	max = MAX(MAX(max, r), MAX(g, b));
	(*this) *= 1.0 / max;
}

RGB RGB::operator + (const RGB& that) const
{
	return RGB(r + that.r, g + that.g, b + that.b, MIN(a, that.a));
}

RGB RGB::operator + (double t) const
{
	return RGB(r + t, g + t, b + t, a);
}

RGB RGB::operator * (double t) const
{
	return RGB(r * t, g * t, b * t, a);
}

void RGB::operator += (const RGB& that)
{
	r += that.r;
	g += that.g;
	b += that.b;
	a = MIN(a, that.a);
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
	return RGB(t * v.r, t * v.g, t * v.b, v.a);
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
	os << "(" << v.r << ", " << v.g << ", " << v.b << ", " << v.a << ")";
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
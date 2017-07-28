#pragma once
#include <Windows.h>
#include <math.h>


const float kPI = 3.141592654f;

struct t3Point
{
	union
	{
		float			v[3];
		struct
		{
			float		x, y, z;
		};
	};

	t3Point() { }
	t3Point(float ix, float iy, float iz) { x = ix; y = iy; z = iz; }

	int operator != (const t3Point&)  const;
	int operator == (const t3Point&)  const;
	t3Point& operator += (const t3Point&);
	t3Point& operator *= (const t3Point&);
	t3Point operator-(t3Point);
	t3Point operator+(t3Point);
	t3Point operator* (t3Point);
	t3Point operator* (float);
	void operator=(t3Point);
};



inline int t3Point::operator != (const t3Point& point) const
{
	return point.x != x || point.y != y || point.z != z;
}



inline int t3Point::operator == (const t3Point& point) const
{
	return point.x == x && point.y == y && point.z == z;
}



inline t3Point& t3Point::operator += (const t3Point& point)
{
	x += point.x;
	y += point.y;
	z += point.z;
	return *this;
}


inline t3Point& t3Point::operator *= (const t3Point& point)
{
	x *= point.x;
	y *= point.y;
	z *= point.z;
	return *this;
}


inline t3Point t3Point::operator - (t3Point p2)
{
	t3Point r;
	r.x = x - p2.x;
	r.y = y - p2.y;
	r.z = z - p2.z;
	return (r);
}


inline t3Point t3Point::operator + (t3Point p2)
{
	t3Point r;
	r.x = x + p2.x;
	r.y = y + p2.y;
	r.z = z + p2.z;
	return (r);
}



inline t3Point t3Point::operator * (t3Point p2)
{
	t3Point r;
	r.x = x * p2.x;
	r.y = y * p2.y;
	r.z = z * p2.z;
	return (r);
}



inline t3Point t3Point::operator* (float s)
{
	t3Point r;
	r.x = x * s;
	r.y = y * s;
	r.z = z * s;
	return (r);
}




inline void t3Point::operator=(t3Point p2)
{
	this->x = p2.x;
	this->y = p2.y;
	this->z = p2.z;
}





inline void Normalize(t3Point& p)
{
	float mag;

	mag = sqrtf((p.x * p.x) + (p.y * p.y) + (p.z * p.z));

	if (mag != 0)
	{
		p.x /= mag;
		p.y /= mag;
		p.z /= mag;
	}
}


inline t3Point Cross(t3Point *p1, t3Point *p2)
{
	t3Point	r;

	r.x = p1->y * p2->z - p2->y * p1->z;
	r.y = -(p1->x * p2->z - p2->x * p1->z);
	r.z = p1->x * p2->y - p2->x * p1->y;

	return(r);
}





struct tEulerAngle
{
	union
	{
		float			e[3];
		struct
		{
			float		a, b, g;
		};
	};

	int operator != (const tEulerAngle&)  const;
	int operator == (const tEulerAngle&)  const;
};



inline int tEulerAngle::operator != (const tEulerAngle& angle) const
{
	return angle.a != a || angle.b != b || angle.g != g;
}

inline int tEulerAngle::operator == (const tEulerAngle& angle) const
{
	return angle.a == a && angle.b == b && angle.g == g;
}






struct tRotMatrix
{
	union
	{
		float			e[3][3];
		struct
		{
			float		r1[3];
			float		r2[3];
			float		r3[3];
		};
		struct
		{
			float		e11, e12, e13;
			float		e21, e22, e23;
			float		e31, e32, e33;
		};
	};
};






//### Bunch of inline functions for basic 3D tasks...
inline float distance(const t3Point& a, const t3Point& b)
{
	return sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y) + (a.z - b.z) * (a.z - b.z));
}

//### Bunch of inline functions for basic 3D tasks...
inline float dotproduct(const t3Point& a, const t3Point& b)
{
	return 	(a.x * b.x + a.y * b.y + a.z * b.z);
}


//### Multiply two rotation matrices together.
inline void RotationMult(tRotMatrix& d, const tRotMatrix& a, const tRotMatrix& b)
{
	for (int i = 0; i < 3; i++) for (int j = 0; j < 3; j++)
	{
		d.e[i][j] = a.e[i][0] * b.e[0][j] + a.e[i][1] * b.e[1][j] + a.e[i][2] * b.e[2][j];
	}
}

//### Transpose (ie invert) a rotation matrix.
inline void TransposeRotation(tRotMatrix& d, const tRotMatrix& s)
{
	for (int i = 0; i < 2; i++) for (int j = i + 1; j < 3; j++)
	{
		d.e[i][j] = s.e[j][i];
		d.e[j][i] = s.e[i][j];
	}
	for (int i = 0; i < 3; i++)
	{
		d.e[i][i] = s.e[i][i];
	}
}

//### Build a rotation matrix from the angles (x, y, z).
inline void BuildRotation(tRotMatrix& d, const tEulerAngle& r)
{
	float sa, ca, sb, cb, sg, cg;
	sa = sin(r.a); ca = cos(r.a);
	sb = sin(r.b); cb = cos(r.b);
	sg = sin(r.g); cg = cos(r.g);

	d.e11 = cb * cg;
	d.e12 = sa * sb * cg - ca * sg;
	d.e13 = ca * sb * cg + sa * sg;
	d.e21 = cb * sg;
	d.e22 = sa * sb * sg + ca * cg;
	d.e23 = ca * sb * sg - sa * cg;
	d.e31 = -sb;
	d.e32 = sa * cb;
	d.e33 = ca * cb;
}

//### Make a unit matrix
inline void BuildUnitRotation(tRotMatrix& d)
{
	d.e11 = 1;
	d.e12 = 0;
	d.e13 = 0;
	d.e21 = 0;
	d.e22 = 1;
	d.e23 = 0;
	d.e31 = 0;
	d.e32 = 0;
	d.e33 = 1;
}

//### Safe for divide by zero...
inline float safeatan(float a, float b)
{
	if (fabs(b) > 1e-20 * fabs(a))
	{
		return atan(a / b);
	}
	else
	{
		return a < 0 ? -kPI / 2 : kPI / 2;
	}
}

//### Map and angle into [-pi .. pi)
inline void FixAngle(float& angle)
{
	if (angle >= kPI)
	{
		angle -= 2 * kPI * int((kPI + angle) / 2 / kPI);
	}
	else if (angle < -kPI)
	{
		angle += 2 * kPI * int((kPI - angle) / 2 / kPI);
	}
}

//### Take a rotation matrix and figure out the angles (x, y, z).
inline void UnwindRotation(tEulerAngle& r, const tRotMatrix& m)
{
	float sa, ca, sb, cb, sg, cg;

	r.a = safeatan(m.e32, m.e33);
	sa = sin(r.a); ca = cos(r.a);

	r.b = safeatan(-sa * m.e31, m.e32);
	sb = sin(r.b); cb = cos(r.b);

	r.g = safeatan(m.e21, m.e11);
	sg = sin(r.g); cg = cos(r.g);

	if ((sb > 0) != (-m.e31 > 0))
	{
		r.b += kPI;
		if ((cb * cg > 0) == (m.e11 > 0))
		{
			r.g += kPI;
		}
	}
	else if ((cb * cg > 0) != (m.e11 > 0))
	{
		r.g += kPI;
	}

	FixAngle(r.b);

	//### Use r(a, b, c) == r(a + pi, -b + pi, c + pi) to map b into [-pi/2 .. pi/2)
	if (r.b >= kPI / 2)
	{
		r.a += kPI;
		r.b = kPI - r.b;
		r.g += kPI;
	}
	else if (r.b < -kPI / 2)
	{
		r.a += kPI;
		r.b = -r.b - kPI;
		r.g += kPI;
	}

	FixAngle(r.a);
	FixAngle(r.g);
}

inline int AngToExp(float d, int bitDepth)
{
	return int(floor(d * (1 << bitDepth) / 2.0f / kPI + 0.5f));
}

inline float ExpToAng(int d, int bitDepth)
{
	return d * 2.0f * kPI / (1 << bitDepth);
}

//### Build a rotation matrix from the angles (y, x, z).
inline void BuildRotationYXZ(tRotMatrix& d, const tEulerAngle& r)
{
	float sa, ca, sb, cb, sg, cg;

	sa = sin(r.a); ca = cos(r.a);
	sb = sin(r.b); cb = cos(r.b);
	sg = sin(r.g); cg = cos(r.g);

	d.e11 = +ca * cg + sa * sb * sg;
	d.e12 = +sa * cg - ca * sb * sg;
	d.e13 = +cb * sg;
	d.e21 = -sa * cb;
	d.e22 = +ca * cb;
	d.e23 = +sb;
	d.e31 = -ca * sg + sa * sb * cg;
	d.e32 = -sa * sg - ca * sb * cg;
	d.e33 = +cb * cg;
}

//### Take a rotation matrix and figure out the angles (y, x, z).
inline void UnwindRotationYXZ(tEulerAngle& r, const tRotMatrix& m)
{
	float sa, ca, sb, cb, sg, cg;

	r.a = safeatan(-m.e21, m.e22);
	sa = sin(r.a); ca = cos(r.a);

	r.b = safeatan(sa * m.e23, -m.e21);
	sb = sin(r.b); cb = cos(r.b);

	r.g = safeatan(m.e13, m.e33);
	sg = sin(r.g); cg = cos(r.g);

	if ((sb > 0) != (m.e23 > 0))
	{
		r.b += kPI;
		if ((cb * cg > 0) == (m.e33 > 0))
		{
			r.g += kPI;
		}
	}
	else if ((cb * cg > 0) != (m.e33 > 0))
	{
		r.g += kPI;
	}

	FixAngle(r.b);

	//### Use r(a, b, c) == r(a + pi, -b + pi, c + pi) to map b into [-pi/2 .. pi/2)
	if (r.b >= kPI / 2)
	{
		r.a += kPI;
		r.b = kPI - r.b;
		r.g += kPI;
	}
	else if (r.b < -kPI / 2)
	{
		r.a += kPI;
		r.b = -r.b - kPI;
		r.g += kPI;
	}

	FixAngle(r.a);
	FixAngle(r.g);

#if 1
	tRotMatrix test;
	BuildRotationYXZ(test, r);
	float sums = 0;

	for (int i = 0; i < 3; i++) for (int j = 0; j < 3; j++)
	{
		sums += (test.e[i][j] - m.e[i][j]) * (test.e[i][j] - m.e[i][j]);
	}
//	ASSERT(sums < 1e-10);
#endif
}

//### Append an angle rotation to a matrix
inline void AppendRotation(tRotMatrix& d, const tRotMatrix& s, const tEulerAngle& r)
{
	tRotMatrix t;
	BuildRotation(t, r);
	RotationMult(d, s, t);
}

//### Append an angle rotation to a matrix
inline void AppendRotationYXZ(tRotMatrix& d, const tRotMatrix& s, const tEulerAngle& r)
{
	tRotMatrix t;
	BuildRotationYXZ(t, r);
	RotationMult(d, s, t);
}

//### Multiply a point by a rotation matrix
inline void RotatePoint(t3Point& d, const t3Point& s, const tRotMatrix& m)
{
	for (int i = 0; i < 3; i++)
	{
		d.v[i] = m.e[i][0] * s.v[0] + m.e[i][1] * s.v[1] + m.e[i][2] * s.v[2];
	}
}

//### Offset a point
inline void OffsetPoint(t3Point& d, const t3Point& o)
{
	d.v[0] += o.v[0];
	d.v[1] += o.v[1];
	d.v[2] += o.v[2];
}

//### Transform a bunch of points...
inline void TransformPoints(t3Point* d, const t3Point* s, const tRotMatrix& r, const t3Point& t, int n)
{
	for (int i = 0; i < n; i++)
	{
		RotatePoint(d[i], s[i], r);
		OffsetPoint(d[i], t);
	}
}




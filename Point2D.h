// ----------------------------------------------------
// Point class    -----------
// ----------------------------------------------------

#ifndef __Point2D_H__
#define __Point2D_H__

#include <math.h>

template<class TYPE>
class Point2D
{
public:

	TYPE x, y;

	Point2D()
	{}

	Point2D(const Point2D& p)
	{
		x = p.x;
		y = p.y;
	}

	// Math ------------------------------------------------
	Point2D operator -(const Point2D &v) const
	{
		p2Vector2 r;

		r.x = x - v.x;
		r.y = y - v.y;

		return(r);
	}

	Point2D operator + (const Point2D &v) const
	{
		p2Vector2 r;

		r.x = x + v.x;
		r.y = y + v.y;

		return(r);
	}

	const Point2D& operator -=(const Point2D &v)
	{
		x -= v.x;
		y -= v.y;

		return(*this);
	}

	const Point2D& operator +=(const Point2D &v)
	{
		x += v.x;
		y += v.y;

		return(*this);
	}

	bool operator ==(const Point2D& v) const
	{
		return (x == v.x && y == v.y);
	}

	bool operator !=(const Point2D& v) const
	{
		return (x != v.x || y != v.y);
	}

	// Utils ------------------------------------------------
	bool IsZero() const
	{
		return (x == 0 && y == 0);
	}

	Point2D& SetToZero()
	{
		x = y = 0;
		return(*this);
	}

	Point2D& Negate()
	{
		x = -x;
		y = -y;

		return(*this);
	}

	// Distances ---------------------------------------------
	TYPE DistanceTo(const Point2D& v) const
	{
		TYPE fx = x - v.x;
		TYPE fy = y - v.y;

		return (TYPE) sqrt((float) (fx*fx) + (fy*fy));
	}
};

#endif // __Point2D_H__
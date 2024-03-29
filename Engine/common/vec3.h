#ifndef __VEC3_H__
#define __VEC3_H__

#include "Math.h"

//! 3d vector template class with lots of operators and methods.
class vec3
{
public:

	vec3() : x(0), y(0), z(0) {};
	vec3(float nx, float ny, float nz) : x(nx), y(ny), z(nz) {};
	vec3(const vec3& other) :x(other.x), y(other.y), z(other.z) {};

	// operators

	vec3& operator=(const vec3& other)	{ x = other.x; y = other.y; z = other.z; return *this; }

	vec3 operator+(const vec3& other) const { return vec3(x + other.x, y + other.y, z + other.z); }
	vec3& operator+=(const vec3& other)	{ x += other.x; y += other.y; z += other.z; return *this; }

	vec3 operator-(const vec3& other) const { return vec3(x - other.x, y - other.y, z - other.z); }
	vec3& operator-=(const vec3& other)	{ x -= other.x; y -= other.y; z -= other.z; return *this; }

	vec3 operator*(const vec3& other) const { return vec3(x * other.x, y * other.y, z * other.z); }
	vec3& operator*=(const vec3& other)	{ x *= other.x; y *= other.y; z *= other.z; return *this; }
	vec3 operator*(const float v) const { return vec3(x * v, y * v, z * v); }
	vec3& operator*=(const float v) { x *= v; y *= v; z *= v; return *this; }

	vec3 operator/(const vec3& other) const { return vec3(x / other.x, y / other.y, z / other.z); }
	vec3& operator/=(const vec3& other)	{ x /= other.x; y /= other.y; z /= other.z; return *this; }
	vec3 operator/(const float v) const { float i = (float)1.0 / v; return vec3(x * i, y * i, z * i); }
	vec3& operator/=(const float v) { float i = (float)1.0 / v; x *= i; y *= i; z *= i; return *this; }

	bool operator<=(const vec3&other) const { return x <= other.x && y <= other.y && z <= other.z; };
	bool operator>=(const vec3&other) const { return x >= other.x && y >= other.y && z >= other.z; };

	bool operator==(const vec3& other) const { return other.x == x && other.y == y && other.z == z; }
	bool operator!=(const vec3& other) const { return other.x != x || other.y != y || other.z != z; }

	// functions

	void set(const float nx, const float ny, const float nz) { x = nx; y = ny; z = nz; }
	void set(const vec3& p) { x = p.x; y = p.y; z = p.z; }

	//! Returns length of the vector.
	float getLength() const { return sqrt(x*x + y*y + z*z); }

	//! Returns the dot product with another vector.
	float dot(const vec3& other) const
	{
		return x*other.x + y*other.y + z*other.z;
	}

	//! Returns distance from an other point. Here, the vector is interpreted as
	//! point in 3 dimensional space.
	float getDistanceFrom(const vec3& other)
	{
		float vx = x - other.x; float vy = y - other.y; float vz = z - other.z;
		return sqrt(vx*vx + vy*vy + vz*vz);
	}

	vec3 cross(const vec3& p) const
	{
		return vec3(y * p.z - z * p.y, z * p.x - x * p.z, x * p.y - y * p.x);
	}

	bool isBetweenPoints(const vec3& begin, const vec3& end) const
	{
		// this is very slow, i'll have to write a faster one later.

		vec3 lv = end - begin;
		vec3 pv = *this - begin;

		float l1 = lv.x*lv.x + lv.y*lv.y + lv.z*lv.z;
		float l2 = pv.x*pv.x + pv.y*pv.y + pv.z*pv.z;

		return (l2 < l1);
	}

	//! Normalizes the vector.
	vec3 normalize()
	{
		float inv = (float)1.0 / (float)getLength();
		x *= inv;
		y *= inv;
		z *= inv;
		return *this;
	}

	//! Inverts the vector.
	void invert()
	{
		x *= -1.0f;
		y *= -1.0f;
		z *= -1.0f;
	}

	void rotatexzBy(float degrees, const vec3& center)
	{
		degrees = DEG2RAD(degrees);
		float cs = (float)cos(degrees);
		float sn = (float)sin(degrees);
		x -= center.x;
		z -= center.z;
		set(x*cs - z*sn, y, x*sn + z*cs);
		x += center.x;
		z += center.z;
	}

	void rotatexyBy(float degrees, const vec3& center)
	{
		degrees = DEG2RAD(degrees);
		float cs = (float)cos(degrees);
		float sn = (float)sin(degrees);
		x -= center.x;
		y -= center.y;
		set(x*cs - y*sn, x*sn + y*cs, z);
		x += center.x;
		y += center.y;
	}

	void rotateyzBy(float degrees, const vec3& center)
	{
		degrees = DEG2RAD(degrees);
		float cs = (float)cos(degrees);
		float sn = (float)sin(degrees);
		z -= center.z;
		y -= center.y;
		set(x, y*cs - z*sn, y*sn + z*cs);
		z += center.z;
		y += center.y;
	}

	void Zero() {
		x = y = z = 0.0f;
	}

	inline float operator[]( const int index ) const {
		return ( &x )[ index ];
	}

	inline float& operator[]( const int index ) {
		return ( &x )[ index ];
	}
	// member variables

	inline bool FixDegenerateNormal( void );

	float x, y, z;
};

inline bool vec3::FixDegenerateNormal( void ) {
	if ( x == 0.0f ) {
		if ( y == 0.0f ) {
			if ( z > 0.0f ) {
				if ( z != 1.0f ) {
					z = 1.0f;
					return true;
				}
			} else {
				if ( z != -1.0f ) {
					z = -1.0f;
					return true;
				}
			}
			return false;
		} else if ( z == 0.0f ) {
			if ( y > 0.0f ) {
				if ( y != 1.0f ) {
					y = 1.0f;
					return true;
				}
			} else {
				if ( y != -1.0f ) {
					y = -1.0f;
					return true;
				}
			}
			return false;
		}
	} else if ( y == 0.0f ) {
		if ( z == 0.0f ) {
			if ( x > 0.0f ) {
				if ( x != 1.0f ) {
					x = 1.0f;
					return true;
				}
			} else {
				if ( x != -1.0f ) {
					x = -1.0f;
					return true;
				}
			}
			return false;
		}
	}
	if ( idMath::Fabs( x ) == 1.0f ) {
		if ( y != 0.0f || z != 0.0f ) {
			y = z = 0.0f;
			return true;
		}
		return false;
	} else if ( idMath::Fabs( y ) == 1.0f ) {
		if ( x != 0.0f || z != 0.0f ) {
			x = z = 0.0f;
			return true;
		}
		return false;
	} else if ( idMath::Fabs( z ) == 1.0f ) {
		if ( x != 0.0f || y != 0.0f ) {
			x = y = 0.0f;
			return true;
		}
		return false;
	}
	return false;
}


#endif
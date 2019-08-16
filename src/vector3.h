// simple 3-component vector class
// this class is assumed to be exactly 3 floats in size!
// absolutely avoid virtual function!

#pragma once

#include <math.h>

class Vector3
{
	#define EPS 0.0001f

public:
   float x,y,z;

   Vector3() = default;
   Vector3(float x, float y, float z) : x(x), y(y), z(z) { };

   // create normalized vector
   inline static Vector3 normalize(const Vector3& v);

   inline operator const float* () const;                 // cast to (float*)
   inline bool    operator ==(const Vector3& v) const;    // equal
   inline bool    operator !=(const Vector3& v) const;    // not equal
   inline Vector3 operator + (const Vector3& v) const;    // add two vectors, return new vector
   inline void    operator +=(const Vector3& v);          // add another vector
   inline Vector3 operator - (const Vector3& v) const;    // subtract two vectors, return new vector
   inline Vector3 operator - () const;                    // negate
   inline void    operator -=(const Vector3& v);          // subtract another vector
   inline Vector3 operator * (const float f) const;       // multiply by scalar
   inline float   operator * (const Vector3& v) const;    // dot product
   inline void    operator *=(const float f);             // multiply by scalar
   inline Vector3 operator % (const Vector3& v) const;    // cross product
   inline void    normalize(float t= 1.0f);               // scale to given length
   inline float   length() const;                         // get length
   inline float   length2() const;                        // get length^2 (avoids sqrt)
   inline float*  data() const;                           // get pointer to components
};


// cast vector to float*
Vector3::operator const float* () const
{
   return &x;
}

// return scaled vector
Vector3 Vector3::operator * (const float f) const
{
   Vector3 n;
   n.x= x*f;
   n.y= y*f;
   n.z= z*f;
   return n;
}

// scale vector
void Vector3::operator *= (const float f)
{
   x*= f;
   y*= f;
   z*= f;
}

// dot product
float Vector3::operator * (const Vector3& v) const
{
   return x*v.x + y*v.y + z*v.z;
}

// return sum of two vectors
Vector3 Vector3::operator + (const Vector3& v) const
{
   Vector3 n;
   n.x= x + v.x;
   n.y= y + v.y;
   n.z= z + v.z;
   return n;
}

// add vector
void Vector3::operator += (const Vector3& v)
{
   x+=v.x;
   y+=v.y;
   z+=v.z;
}

// subtract
Vector3 Vector3::operator - (const Vector3& v) const
{
   Vector3 n;
   n.x= x - v.x;
   n.y= y - v.y;
   n.z= z - v.z;
   return n;
}

// negate
Vector3 Vector3::operator - () const
{
   Vector3 n;
   n.x= -x;
   n.y= -y;
   n.z= -z;
   return n;
}

// sub from vector
void Vector3::operator -= (const Vector3& v)
{
   x-=v.x;
   y-=v.y;
   z-=v.z;
}

// cross product
Vector3 Vector3::operator % (const Vector3& v) const
{
   Vector3 n;
   n.x = y*v.z - z*v.y;
   n.y = z*v.x - x*v.z;
   n.z = x*v.y - y*v.x;
   return n;
}

// compare vectors
bool Vector3::operator == (const Vector3& v1) const
{
   Vector3 diff= *this - v1;
   float dist= diff * diff;
   // no sqrt! compare with EPS^2 instead
   return (dist<EPS*EPS);
}

// not equal
bool Vector3::operator != (const Vector3& v1) const
{
   return !(*this == v1);
}

void Vector3::normalize(float t)
{
   t /= length();
   x*=t;
   y*=t;
   z*=t;
}

float Vector3::length2() const
{
   return x*x+y*y+z*z;
}

float Vector3::length() const
{
   float t= length2();
   return sqrtf(t);
}

// create normalized vector
Vector3 Vector3::normalize(const Vector3& v)
{
   return v * (1.0f / v.length());
}

float* Vector3::data() const
{
   return (float*)&x;
}

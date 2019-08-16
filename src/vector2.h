// simple 2-component vector class
// this class is assumed to be exactly 2 floats (8 bytes) in size!
// absolutely avoid virtual function!

#pragma once

#include <math.h>

class Vector2
{
   #define EPS 0.0001f

public:
   float x,y;

   Vector2() = default;
   Vector2(float x, float y) : x(x), y(y) { };

   // create normalized vector
   inline static Vector2 normalize(const Vector2& v);

   inline operator const float* () const;                 // cast to (float*)
   inline bool    operator ==(const Vector2& v) const;    // equal
   inline bool    operator !=(const Vector2& v) const;    // not equal
   inline Vector2 operator + (const Vector2& v) const;    // add two vectors, return new vector
   inline void    operator +=(const Vector2& v);          // add another vector
   inline Vector2 operator - (const Vector2& v) const;    // subtract two vectors, return new vector
   inline Vector2 operator - () const;                    // negate
   inline void    operator -=(const Vector2& v);          // subtract another vector
   inline Vector2 operator * (const float f) const;       // multiply by scalar
   inline float   operator * (const Vector2& v) const;    // dot product
   inline void    operator *=(const float f);             // multiply by scalar
   inline float   operator % (const Vector2& v) const;    // cross product
   inline void    normalize(float t= 1.0f);               // scale to given length
   inline float   length() const;                         // get length
   inline float   length2() const;                        // get length^2 (avoids sqrt)
   inline float*  data() const;                           // get pointer to components
};


// cast vector to float*
Vector2::operator const float* () const
{
   return &x;
}

// return scaled vector
Vector2 Vector2::operator * (const float f) const
{
   Vector2 n;
   n.x= x*f;
   n.y= y*f;
   return n;
}

// scale vector
void Vector2::operator *= (const float f)
{
   x*= f;
   y*= f;
}

// dot product
float Vector2::operator * (const Vector2& v) const
{
   return x*v.x + y*v.y;
}

// return sum of two vectors
Vector2 Vector2::operator + (const Vector2& v) const
{
   Vector2 n;
   n.x= x + v.x;
   n.y= y + v.y;
   return n;
}

// add vector
void Vector2::operator += (const Vector2& v)
{
   x+=v.x;
   y+=v.y;
}

// subtract
Vector2 Vector2::operator - (const Vector2& v) const
{
   Vector2 n;
   n.x= x - v.x;
   n.y= y - v.y;
   return n;
}

// negate
Vector2 Vector2::operator - () const
{
   Vector2 n;
   n.x= -x;
   n.y= -y;
   return n;
}

// sub from vector
void Vector2::operator -= (const Vector2& v)
{
   x-=v.x;
   y-=v.y;
}

// cross product (assuming z=0)
float Vector2::operator % (const Vector2& v) const
{
   return x*v.y - y*v.x;
}

// compare vectors
bool Vector2::operator == (const Vector2& v1) const
{
   Vector2 diff= *this - v1;
   float dist= diff * diff;
   // no sqrt! compare with EPS^2 instead
   return (dist<EPS*EPS);
}

// not equal
bool Vector2::operator != (const Vector2& v1) const
{
   return !(*this == v1);
}

void Vector2::normalize(float t)
{
   t /= length();
   x*=t;
   y*=t;
}

float Vector2::length2() const
{
   return x*x + y*y;
}

float Vector2::length() const
{
   float t= length2();
   return sqrtf(t);
}

// create normalized vector
Vector2 Vector2::normalize(const Vector2& v)
{
   return v * (1.0f / v.length());
}

float* Vector2::data() const
{
   return (float*)&x;
}

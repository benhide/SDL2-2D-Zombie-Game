#ifndef _Vector2_H
#define _Vector2_H

//C++ Headers
#include <iostream>
#include <math.h>

//Vector 2 class
class Vector2
{
public:
	//Default constructor
	Vector2();

	//Overloaded constructor
	Vector2(float x, float y);

	//Vector length function
	float length();

	//Vector normalize function
	void normalize();
	
	//Vector lerp function
	Vector2 lerp(Vector2 startV, Vector2 destV, float percent);
	
	//Vector 2 addition operators
	Vector2 operator+(const Vector2& v2) const;
	friend Vector2& operator+=(Vector2& v1, const Vector2& v2);

	//Vector 2 subtraction operators
	Vector2 operator-(const Vector2& v2) const;
	friend Vector2& operator-=(Vector2& v1, const Vector2& v2);
	
	//Vector 2 multiplication operators
	Vector2 operator*(float scalar);
	Vector2& operator*=(float scalar);
	
	//Vector 2 division operators
	Vector2 operator/(float scalar);
	Vector2& operator/=(float scalar);
	
	//X and Y points of the vector
	float X;
	float Y;
};

#endif
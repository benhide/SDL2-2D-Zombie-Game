#include "Vector2.h"

//Default constructor
Vector2::Vector2() : X(0), Y(0) 
{}

//Overloaded constructor
Vector2::Vector2(float x, float y) : X(x), Y(y) 
{}

//Vector length function
float Vector2::length()
{
	return sqrt((X * X) + (Y * Y));
}

//Vector normalize function
void Vector2::normalize()
{
	float l = length();
	if (l > 0)
		(*this) *= (1 / l);
}

//Vector lerp function
Vector2 Vector2::lerp(Vector2 startV, Vector2 destV, float percent)
{
	return (startV + (destV - startV) * percent);
}

//Vector 2 addition operators
Vector2 Vector2::operator+(const Vector2& v2) const
{
	return Vector2(X + v2.X, Y + v2.Y);
}

//Vector 2 addition operators
Vector2& operator+=(Vector2& v1, const Vector2& v2)
{
	v1.X += v2.X; v1.Y += v2.Y;	return v1;
}

//Vector 2 subtraction operators
Vector2 Vector2::operator-(const Vector2& v2) const
{
	return Vector2(X - v2.X, Y - v2.Y);
}

//Vector 2 subtraction operators
Vector2& operator-=(Vector2& v1, const Vector2& v2)
{
	v1.X -= v2.X; v1.Y -= v2.Y;	return v1;
}

//Vector 2 multiplication operators
Vector2 Vector2::operator*(float scalar)
{
	return Vector2(X * scalar, Y * scalar);
}

//Vector 2 multiplication operators
Vector2& Vector2::operator*=(float scalar)
{
	X *= scalar; Y *= scalar; return *this;
}

//Vector 2 division operators
Vector2 Vector2::operator/(float scalar)
{
	return Vector2(X / scalar, Y / scalar);
}

//Vector 2 division operators
Vector2& Vector2::operator/=(float scalar)
{
	X /= scalar; Y /= scalar; return *this;
}
#ifndef VECTOR2D_H
#define VECTOR2D_H
#include <ostream>
#include <math.h>

class Vector2
{
public:
	float x, y;

	//Construction
	Vector2() : x(0.0f), y(0.0f) { }
	Vector2(const float& _x, const float& _y) : x(_x), y(_y) { }
	Vector2(const Vector2& other) : x(other.x), y(other.y) { }

	//Logic Operators
		//Assignment Op is called to set existing Vector2 to other
		//(?) Do we even need the +(float) operator? the +=(Vec&&) op?
	operator=(const Vector2& other) {
		this->x = other.x;
		this->y = other.y;
	}
	bool operator==(const Vector2& other) const {
		if (x == other.x && y == other.y)
			return true;
		return false;
	}
	bool operator!=(const Vector2& other) const {
		return !(*this == other);
	}
	Vector2 operator+(const Vector2& other) const {
		return Add(other);
	}
	Vector2& operator+(const float& num) {
		this->x += num;
		this->y += num;
		return *this;
	}
	Vector2& operator+=(Vector2& other) {
		x += other.x;
		y += other.y;
		return other;
	}
	Vector2& operator+=(Vector2&& rhs) {
		x += rhs.x;
		y += rhs.y;
		return *this;
	}
	Vector2 operator-(const Vector2& other) const {
		return Subtract(other);
	}
	Vector2& operator-=(Vector2& other) {
		x -= other.x;
		y -= other.y;
		return other;
	}

	//Methods
	Vector2 Add(const Vector2& other) const {
		return Vector2(x + other.x, y + other.y);
	}
	Vector2 Subtract(const Vector2& other) const {
		return Vector2(x - other.x, y - other.y);
	}
	Vector2& ScaleMult(const float& value) {
		this->x *= value;
		this->y *= value;
		return *this;
	}
	Vector2& ScaleDiv(const float& value) {
		if (value == 0) {
			return *this;
		}
		this->x /= value;
		this->y /= value;
		return *this;
	}
	void Set(const float& _x, const float& _y) {
		x = _x;
		y = _y;
	}
	void Reset() {
		x = 0.0;
		y = 0.0;
	}
	void Normalize() {
		float mag = Magnitude();
		if (mag == 0)
			return;
		x /= mag;
		y /= mag;
	}
	Vector2 Normalized() {
		//In situations where you just want the normal of another Vec2.
		//As a new vec2.
		Vector2 norm = *this;
		norm.Normalize();
		return norm;
	}
	float Magnitude() {
		float magnitude = 0.0;
		if (abs(x + y) != 0)
			magnitude = sqrt((x*x) + (y*y));
		return magnitude;
	}
	float Length() {
		return Magnitude();
	}
	float Dot(const Vector2& other) {
		float dot = (x * other.x) + (y * other.y);
		return dot;
	}
	float ScProjectThisOnto(Vector2& other) {
		//Projects this onto other
		Vector2 a = *this;
		float projection = a.Dot(other);
		return projection / other.Magnitude();
	}
	float ScProjectOntoThis(Vector2& other) {
		//Projects other onto this
		Vector2 b = other.Normalized();
		return b.Dot(other);
	}
	
	//Vector Projection

	friend std::ostream& operator<<(std::ostream& stream, const Vector2& vec) {
		stream << "(" << vec.x << "," << vec.y << ")";
		return stream;
	}
};
#endif
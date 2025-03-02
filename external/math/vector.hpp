#pragma once

#include <corecrt_math.h>

class Vector3
{

public:
	Vector3() : x(0.f), y(0.f), z(0.f) {}
	Vector3(double _x, double _y, double _z) : x(_x), y(_y), z(_z) {}
	~Vector3() {}
	double x;
	double y;
	double z;
	inline double Dot(Vector3 v)
	{
		return x * v.x + y * v.y + z * v.z;
	}
	inline double GetDistance(Vector3 v)
	{
		return double(sqrtf(powf(v.x - x, 2.0) + powf(v.y - y, 2.0) + powf(v.z - z, 2.0)));
	}
	Vector3 operator-(Vector3 v)
	{
		return Vector3(x - v.x, y - v.y, z - v.z);
	}
};

struct FQuat
{

	double x;
	double y;
	double z;
	double w;
};
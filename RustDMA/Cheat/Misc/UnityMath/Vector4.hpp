#ifndef UnityVector4_HPP
#define UnityVector4_HPP

#include "Vector3.hpp"

#include <algorithm>
#include <limits>
#include <stdexcept>
#include <cmath>

class UnityVector4
{
public:
	float x, y, z, w;

	UnityVector4();
	UnityVector4(float x, float y, float z, float w);
	explicit UnityVector4(const float* array);

	void Set(float x, float y, float z, float w);
	void Set(const float* array);

	void Scale(UnityVector4 scale);
	UnityVector4 Normalized();
	float Magnitude();
	float SqrMagnitude();

	std::string ToString();
	void DebugPrint();

	float* GetPtr();
	const float* GetPtr() const;

	float& operator[](int i);
	const float& operator[](int i)const;

	bool operator == (const UnityVector4& v) const;
	bool operator != (const UnityVector4& v) const;
	bool operator == (const float v[4]) const;
	bool operator != (const float v[4]) const;

	UnityVector4& operator += (const UnityVector4& input_vector);
	UnityVector4& operator += (float scale);

	UnityVector4& operator -= (const UnityVector4& input_vector);
	UnityVector4& operator -= (float scale);

	UnityVector4& operator *= (const UnityVector4& input_vector);
	UnityVector4& operator *= (float scale);

	UnityVector4& operator /= (const UnityVector4& input_vector);
	UnityVector4& operator /= (float scale);

	UnityVector4 operator - () const;

	friend UnityVector4 operator + (const UnityVector4& lhs, const UnityVector4& rhs);
	friend UnityVector4 operator + (const UnityVector4& lhs, float scale);
	friend UnityVector4 operator + (float scale, const UnityVector4& rhs);

	friend UnityVector4 operator - (const UnityVector4& lhs, const UnityVector4& rhs);
	friend UnityVector4 operator - (const UnityVector4& lhs, float scale);
	friend UnityVector4 operator - (float scale, const UnityVector4& rhs);

	friend UnityVector4 operator * (const UnityVector4& lhs, const UnityVector4& rhs);
	friend UnityVector4 operator * (const UnityVector4& lhs, float scale);
	friend UnityVector4 operator * (float scale, const UnityVector4& rhs);

	friend UnityVector4 operator / (const UnityVector4& lhs, const UnityVector4& rhs);
	friend UnityVector4 operator / (const UnityVector4& lhs, float scale);
	friend UnityVector4 operator / (float scale, const UnityVector4& rhs);

	static UnityVector4 Scale(UnityVector4 vector_a, UnityVector4 vector_b);
	static float Dot(UnityVector4 vector_a, UnityVector4 vector_b);
	static UnityVector4 Normalize(UnityVector4 vector_a);
	static float Distance(UnityVector4 vector_a, UnityVector4 vector_b);
	static float Magnitude(UnityVector4 vector_a);
	static float SqrMagnitude(UnityVector4 vector_a);
	static UnityVector4 Min(UnityVector4 lhs, UnityVector4 rhs);
	static UnityVector4 Max(UnityVector4 lhs, UnityVector4 rhs);
	static UnityVector4 Project(UnityVector4 vector_a, UnityVector4 b);
	static UnityVector4 Lerp(UnityVector4 vector_a, UnityVector4 vector_b, float time);
	static UnityVector4 LerpUnclamped(UnityVector4 vector_a, UnityVector4 vector_b, float time);
	static UnityVector4 MoveTowards(UnityVector4 current, UnityVector4 target, float max_distance_delta);

	static UnityVector4 Zero();
	static UnityVector4 One();
	static UnityVector4 PositiveInfinity();
	static UnityVector4 NegativeInfinity();

	static const float epsilon;
	static const float infinity;
	static const UnityVector4 zero;
	static const UnityVector4 one;
	static const UnityVector4 positive_infinity;
	static const UnityVector4 negative_infinity;
};

#endif
#ifndef UnityVector3_HPP
#define UnityVector3_HPP

#include <algorithm>
#include <string>
#include "FloatConversion.hpp"

class UnityVector3
{
public:
	float x, y, z;

	UnityVector3();
	UnityVector3(float x, float y);
	UnityVector3(float x, float y, float z);
	explicit UnityVector3(const float* array);

	void Set(float x, float y, float z);
	void Set(const float* array);
	void Set(const UnityVector3& vector);

	void Scale(const UnityVector3& scale);
	UnityVector3 Normalized();
	float Magnitude();
	float SqrMagnitude();

	std::string ToString();
	void DebugPrint();

	float* GetPtr();
	const float* GetPtr()const;
	float& operator[](int i);
	const float& operator[](int i)const;

	bool operator == (const UnityVector3& input_vector);
	bool operator != (const UnityVector3& input_vector);

	UnityVector3& operator += (const UnityVector3& input_vector);
	UnityVector3& operator += (float scale);

	UnityVector3& operator -= (const UnityVector3& input_vector);
	UnityVector3& operator -= (float scale);

	UnityVector3& operator *= (const UnityVector3& input_vector);
	UnityVector3& operator *= (float scale);

	UnityVector3& operator /= (const UnityVector3& input_vector);
	UnityVector3& operator /= (float scale);

	UnityVector3 operator - ()const;

	friend UnityVector3 operator + (const UnityVector3& lhs, const UnityVector3& rhs);
	friend UnityVector3 operator + (const UnityVector3& lhs, float scale);
	friend UnityVector3 operator + (float scale, const UnityVector3& rhs);
 
	friend UnityVector3 operator - (const UnityVector3& lhs, const UnityVector3& rhs);
	friend UnityVector3 operator - (const UnityVector3& lhs, float scale);
	friend UnityVector3 operator - (float scale, const UnityVector3& rhs);

	friend UnityVector3 operator * (const UnityVector3& lhs, const UnityVector3& rhs);
	friend UnityVector3 operator * (const UnityVector3& lhs, float scale);
	friend UnityVector3 operator * (float scale, const UnityVector3& rhs);

	friend UnityVector3 operator / (const UnityVector3& lhs, const UnityVector3& rhs);
	friend UnityVector3 operator / (const UnityVector3& lhs, float scale);
	friend UnityVector3 operator / (float scale, const UnityVector3& rhs);

	static UnityVector3 Scale(UnityVector3 vector_a, UnityVector3 vector_b);
	static UnityVector3 Cross(UnityVector3 lhs, UnityVector3 rhs);
	static float Dot(UnityVector3 lhs, UnityVector3 rhs);
	static UnityVector3 Normalize(UnityVector3 vector);
	static void OrthoNormalize(UnityVector3* inU, UnityVector3* inV, UnityVector3* inW);
	static UnityVector3 OrthoNormalVectorFast(const UnityVector3& n);
	static float Distance(UnityVector3 vector_a, UnityVector3 vector_b);
	static UnityVector3 ClampMagnitude(UnityVector3 vector, float max_length);
	static float Magnitude(UnityVector3 vector);
	static float SqrMagnitude(UnityVector3 vector);
	static UnityVector3 Min(UnityVector3 lhs, UnityVector3 rhs);
	static UnityVector3 Max(UnityVector3 lhs, UnityVector3 rhs);
	static UnityVector3 Reflect(UnityVector3 input_direction, UnityVector3 input_normal);
	static float Angle(UnityVector3 from, UnityVector3 to);
	static float AngleBetween(UnityVector3 from, UnityVector3 to);
	static float SignedAngle(UnityVector3 from, UnityVector3 to, UnityVector3 axis);
	static UnityVector3 CalculateAngle(UnityVector3 src, UnityVector3 dst);
	static UnityVector3 InverseSafe(UnityVector3 v);
	static UnityVector3 Project(UnityVector3 vector, UnityVector3 on_normal);
	static UnityVector3 ProjectOnPlane(UnityVector3 vector, UnityVector3 plane_normal);
	static UnityVector3 Exclude(UnityVector3 exclude_this, UnityVector3 from_that);
	static UnityVector3 Lerp(UnityVector3 vector_a, UnityVector3 vector_b, float time);
	static UnityVector3 LerpUnclamped(UnityVector3 vector_a, UnityVector3 vector_b, float time);
	static UnityVector3 MoveTowards(UnityVector3 current, UnityVector3 target, float max_distance_delta);
	static UnityVector3 SmoothDamp(UnityVector3 current, UnityVector3 target, UnityVector3 current_velocity, float smooth_time, float max_speed);
	static UnityVector3 SmoothDamp(UnityVector3 current, UnityVector3 target, UnityVector3 current_velocity, float smooth_time);
	static UnityVector3 SmoothDamp(UnityVector3 current, UnityVector3 target, UnityVector3 current_velocity, float smooth_time, float max_speed = std::numeric_limits<float>::infinity(), float delta_time = 0.03125f);

	static UnityVector3 Zero();
	static UnityVector3 One();
	static UnityVector3 Up();
	static UnityVector3 Down();
	static UnityVector3 Left();
	static UnityVector3 Right();
	static UnityVector3 Forward();
	static UnityVector3 Back();
	static UnityVector3 PositiveInfinity();
	static UnityVector3 NegativeInfinity();

	static const float epsilon;
	static const float infinity;
	static const UnityVector3 zero;
	static const UnityVector3 one;
	static const UnityVector3 up;
	static const UnityVector3 down;
	static const UnityVector3 left;
	static const UnityVector3 right;
	static const UnityVector3 forward;
	static const UnityVector3 back;
	static const UnityVector3 positive_infinity;
	static const UnityVector3 negative_infinity;
};

#endif
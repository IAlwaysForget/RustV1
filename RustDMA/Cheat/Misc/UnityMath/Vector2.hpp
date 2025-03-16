#ifndef UnityVector2_HPP
#define UnityVector2_HPP

#include <string>
#include <algorithm>
#include "FloatConversion.hpp"
#include "imgui.h"

class UnityVector2
{
public:
	float x, y;

	UnityVector2();
	UnityVector2(float x, float y);
	explicit UnityVector2(const float* array);

	void Set(float x, float y);
	void Set(const float* array);
	void Set(const UnityVector2& vector);

	void Scale(const UnityVector2& scale);
	UnityVector2 Normalized();
	float Magnitude();
	float SqrMagnitude();

	std::string ToString();
	void DebugPrint();

	float* GetPtr();
	const float* GetPtr()const;
	float& operator[](int i);
	const float& operator[](int i)const;

	bool operator == (const UnityVector2& input_vector);
	bool operator != (const UnityVector2& input_vector);

	UnityVector2& operator += (const UnityVector2& input_vector);
	UnityVector2& operator += (float scale);

	UnityVector2& operator -= (const UnityVector2& input_vector);
	UnityVector2& operator -= (float scale);

	UnityVector2& operator *= (const UnityVector2& input_vector);
	UnityVector2& operator *= (float scale);

	UnityVector2& operator /= (const UnityVector2& input_vector);
	UnityVector2& operator /= (float scale);

	UnityVector2 operator - ()const;

	operator ImVec2() const { return ImVec2{ x, y }; }



	friend UnityVector2 operator + (const UnityVector2& lhs, const UnityVector2& rhs);
	friend UnityVector2 operator + (const UnityVector2& lhs, float scale);
	friend UnityVector2 operator + (float scale, const UnityVector2& rhs);

	friend UnityVector2 operator - (const UnityVector2& lhs, const UnityVector2& rhs);
	friend UnityVector2 operator - (const UnityVector2& lhs, float scale);
	friend UnityVector2 operator - (float scale, const UnityVector2& rhs);

	friend UnityVector2 operator * (const UnityVector2& lhs, const UnityVector2& rhs);
	friend UnityVector2 operator * (const UnityVector2& lhs, float scale);
	friend UnityVector2 operator * (float scale, const UnityVector2& rhs);

	friend UnityVector2 operator / (const UnityVector2& lhs, const UnityVector2& rhs);
	friend UnityVector2 operator / (const UnityVector2& lhs, float scale);
	friend UnityVector2 operator / (float scale, const UnityVector2& rhs);

	static UnityVector2 Scale(UnityVector2 vector_a, UnityVector2 vector_b);
	static float Dot(UnityVector2 lhs, UnityVector2 rhs);
	static UnityVector2 Normalize(UnityVector2 value);
	static float Distance(UnityVector2 vector_a, UnityVector2 vector_b);
	static UnityVector2 ClampMagnitude(UnityVector2 vector, float max_length);
	static float Magnitude(UnityVector2 vector_a);
	static float SqrMagnitude(UnityVector2 vector_a);
	static UnityVector2 Min(UnityVector2 lhs, UnityVector2 rhs);
	static UnityVector2 Max(UnityVector2 lhs, UnityVector2 rhs);
	static UnityVector2 Reflect(UnityVector2 in_direction, UnityVector2 in_normal);
	static UnityVector2 Perpendicular(UnityVector2 in_direction);

	static float Angle(UnityVector2 from, UnityVector2 to);
	static float SignedAngle(UnityVector2 from, UnityVector2 to);
	static UnityVector2 Lerp(UnityVector2 vector_a, UnityVector2 vector_b, float time);
	static UnityVector2 LerpUnclamped(UnityVector2 vector_a, UnityVector2 vector_b, float time);
	static UnityVector2 MoveTowards(UnityVector2 current, UnityVector2 target, float max_distance_delta);
	static UnityVector2 SmoothDamp(UnityVector2 current, UnityVector2 target, UnityVector2& current_velocity, float smooth_time, float max_speed);
	static UnityVector2 SmoothDamp(UnityVector2 current, UnityVector2 target, UnityVector2& current_velocity, float smooth_time);
	static UnityVector2 SmoothDamp(UnityVector2 current, UnityVector2 target, UnityVector2& current_velocity, float smooth_time, float max_speed = infinity, float delta_time = 0.03125f);

	static UnityVector2 Zero();
	static UnityVector2 One();
	static UnityVector2 Up();
	static UnityVector2 Down();
	static UnityVector2 Left();
	static UnityVector2 Right();
	static UnityVector2 PositiveInfinity();
	static UnityVector2 NegativeInfinity();

	static const float epsilon;
	static const float infinity;
	static const UnityVector2 zero;
	static const UnityVector2 one;
	static const UnityVector2 up;
	static const UnityVector2 down;
	static const UnityVector2 left;
	static const UnityVector2 right;
	static const UnityVector2 positive_infinity;
	static const UnityVector2 negative_infinity;
};

#endif
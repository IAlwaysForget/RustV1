#include "Vector2.hpp"
#include <stdexcept>
#include <limits>

UnityVector2::UnityVector2()
{
	this->x = 0.f;
	this->y = 0.f;
}

UnityVector2::UnityVector2(float x, float y)
{
	this->x = x;
	this->y = y;
}

UnityVector2::UnityVector2(const float* array)
{
	this->x = array[0];
	this->y = array[1];
}

void UnityVector2::Set(float x, float y)
{
	this->x = x;
	this->y = y;
}

void UnityVector2::Set(const float* array)
{
	this->x = array[0];
	this->y = array[1];
}

void UnityVector2::Set(const UnityVector2& vector)
{
	this->x = vector.x;
	this->y = vector.y;
}

void UnityVector2::Scale(const UnityVector2& scale)
{
	this->x *= scale.x;
	this->y *= scale.y;
}

UnityVector2 UnityVector2::Normalized()
{
	return Normalize(UnityVector2(this->x, this->y));
}

float UnityVector2::Magnitude()
{
	return sqrt(powf(this->x, 2) + powf(this->y, 2));
}

float UnityVector2::SqrMagnitude()
{
	return powf(this->x, 2) + powf(this->y, 2);
}

std::string UnityVector2::ToString()
{
	char buffer[64];
	sprintf(buffer, "X:%f, Y:%f", this->x, this->y);
	return buffer;
}

void UnityVector2::DebugPrint()
{
	printf("X:%f, Y:%f\n", this->x, this->y);
}

float* UnityVector2::GetPtr()
{
	return &x;
}

const float* UnityVector2::GetPtr() const
{
	return &x;
}

float& UnityVector2::operator[] (int i)
{
	if (i < 0 || i > 1)
	{
		throw std::out_of_range("Vector3 index out of range");
	}
	return (&x)[i];
}

const float& UnityVector2::operator[] (int i)const
{
	if (i < 0 || i > 1)
	{
		throw std::out_of_range("Vector3 index out of range");
	}
	return (&x)[i];
}

bool UnityVector2::operator == (const UnityVector2& input_vector)
{
	return this->x == input_vector.x && this->y == input_vector.y;
}

bool UnityVector2::operator != (const UnityVector2& input_vector)
{
	return this->x != input_vector.x || this->y != input_vector.y;
}

UnityVector2& UnityVector2::operator += (const UnityVector2& input_vector)
{
	this->x += input_vector.x;
	this->y += input_vector.y;
	return *this;
}

UnityVector2& UnityVector2::operator += (float scale)
{
	this->x += scale;
	this->y += scale;
	return *this;
}

UnityVector2& UnityVector2::operator -= (const UnityVector2& input_vector)
{
	this->x -= input_vector.x;
	this->y -= input_vector.y;
	return *this;
}

UnityVector2& UnityVector2::operator -= (float scale)
{
	this->x -= scale;
	this->y -= scale;
	return *this;
}

UnityVector2& UnityVector2::operator *= (const UnityVector2& input_vector)
{
	this->x *= input_vector.x;
	this->y *= input_vector.y;
	return *this;
}

UnityVector2& UnityVector2::operator *= (float scale)
{
	this->x *= scale;
	this->y *= scale;
	return *this;
}

UnityVector2& UnityVector2::operator /= (const UnityVector2& input_vector)
{
	this->x /= input_vector.x;
	this->y /= input_vector.y;
	return *this;
}

UnityVector2& UnityVector2::operator /= (float scale)
{
	this->x /= scale;
	this->y /= scale;
	return *this;
}

UnityVector2 UnityVector2::operator - ()const
{
	return UnityVector2(-x, -y);
}

UnityVector2 operator + (const UnityVector2& lhs, const UnityVector2& rhs)
{
	return UnityVector2(lhs.x + rhs.x, lhs.y + rhs.y);
}

UnityVector2 operator + (const UnityVector2& lhs, float scale)
{
	return UnityVector2(lhs.x + scale, lhs.y + scale);
}

UnityVector2 operator + (float scale, const UnityVector2& rhs)
{
	return UnityVector2(scale + rhs.x, scale + rhs.y);
}

UnityVector2 operator - (const UnityVector2& lhs, const UnityVector2& rhs)
{
	return UnityVector2(lhs.x - rhs.x, lhs.y - rhs.y);
}

UnityVector2 operator - (const UnityVector2& lhs, float scale)
{
	return UnityVector2(lhs.x - scale, lhs.y - scale);
}

UnityVector2 operator - (float scale, const UnityVector2& rhs)
{
	return UnityVector2(scale - rhs.x, scale - rhs.y);
}

UnityVector2 operator * (const UnityVector2& lhs, const UnityVector2& rhs)
{
	return UnityVector2(lhs.x * rhs.x, lhs.y * rhs.y);
}

UnityVector2 operator * (const UnityVector2& lhs, float scale)
{
	return UnityVector2(lhs.x * scale, lhs.y * scale);
}

UnityVector2 operator * (float scale, const UnityVector2& rhs)
{
	return UnityVector2(scale * rhs.x, scale * rhs.y);
}

UnityVector2 operator / (const UnityVector2& lhs, const UnityVector2& rhs)
{
	if (rhs.x == 0.0f || rhs.y == 0.0f)
	{
		throw std::runtime_error("Division by zero is not allowed.");
	}
	return UnityVector2(lhs.x / rhs.x, lhs.y / rhs.y);
}

UnityVector2 operator / (const UnityVector2& lhs, float scale)
{
	if (scale == 0.0f)
	{
		throw std::runtime_error("Division by zero is not allowed.");
	}
	return UnityVector2(lhs.x / scale, lhs.y / scale);
}

UnityVector2 operator / (float scale, const UnityVector2& rhs)
{
	if (rhs.x == 0.0f || rhs.y == 0.0f)
	{
		throw std::runtime_error("Division by zero is not allowed.");
	}
	return UnityVector2(scale / rhs.x, scale / rhs.y);
}

UnityVector2 UnityVector2::Scale(UnityVector2 vector_a, UnityVector2 vector_b)
{
	return UnityVector2(vector_a.x * vector_b.x, vector_a.y * vector_b.y);
}

float UnityVector2::Dot(UnityVector2 lhs, UnityVector2 rhs)
{
	return lhs.x * rhs.x + lhs.y * rhs.y;
}

UnityVector2 UnityVector2::Normalize(UnityVector2 value)
{
	UnityVector2 result;
	float num = Magnitude(value);
	if (num > 1E-05f)
	{
		result = value / num;
	}
	else
	{
		result = UnityVector2::Zero();
	}
	return result;
}

float UnityVector2::Distance(UnityVector2 vector_a, UnityVector2 vector_b)
{
	float num = vector_a.x - vector_b.x;
	float num2 = vector_a.y - vector_b.y;
	return sqrt(num * num + num2 * num2);
}

UnityVector2 UnityVector2::ClampMagnitude(UnityVector2 vector, float max_length)
{
	float sqr_magnitude = vector.SqrMagnitude();
	bool flag = sqr_magnitude > max_length * max_length;
	UnityVector2 result;
	if (flag)
	{
		float num = sqrt(sqr_magnitude);
		float num2 = vector.x / num;
		float num3 = vector.y / num;
		result = UnityVector2(num2 * max_length, num3 * max_length);
	}
	else
	{
		result = vector;
	}
	return result;
}

float UnityVector2::Magnitude(UnityVector2 vector_a)
{
	return sqrt(vector_a.x * vector_a.x + vector_a.y * vector_a.y);
}

float UnityVector2::SqrMagnitude(UnityVector2 vector_a)
{
	return vector_a.x * vector_a.x + vector_a.y * vector_a.y;
}

UnityVector2 UnityVector2::Min(UnityVector2 lhs, UnityVector2 rhs)
{
	return UnityVector2(std::min(lhs.x, rhs.x), std::min(lhs.y, rhs.y));
}

UnityVector2 UnityVector2::Max(UnityVector2 lhs, UnityVector2 rhs)
{
	return UnityVector2(std::max(lhs.x, rhs.x), std::max(lhs.y, rhs.y));
}

UnityVector2 UnityVector2::Reflect(UnityVector2 in_direction, UnityVector2 in_normal)
{
	float num = -2.f * Dot(in_normal, in_direction);
	return UnityVector2(num * in_normal.x + in_direction.x, num * in_normal.y + in_direction.y);
}

UnityVector2 UnityVector2::Perpendicular(UnityVector2 in_direction)
{
	return UnityVector2(-in_direction.y, in_direction.x);
}


float UnityVector2::Angle(UnityVector2 from, UnityVector2 to)
{
	float num = sqrt(from.SqrMagnitude() * to.SqrMagnitude());
	bool flag = num < 1E-15f;
	float result;
	if (flag)
	{
		result = 0.f;
	}
	else
	{
		float num2 = (Dot(from, to) / num);
		bool flag = num2 < -1.f;
		if (flag)
		{
			num2 = -1.f;
		}
		else
		{
			bool flag2 = num2 > 1.f;
			if (flag2)
			{
				num2 = 1.f;
			}
		}

		result = acos(num2) * 57.29578f;
	}
	return result;
}

float UnityVector2::SignedAngle(UnityVector2 from, UnityVector2 to)
{
	float num = Angle(from, to);
	float num2 = (from.x * to.y - from.y * to.x >= 0.f) ? 1.f : -1.f;
	return num * num2;
}

UnityVector2 UnityVector2::Lerp(UnityVector2 vector_a, UnityVector2 vector_b, float time)
{
	bool flag = time < 0.f;
	float result;
	if (flag)
	{
		result = 0.f;
	}
	else
	{
		bool flag2 = time > 1.f;
		if (flag2)
		{
			result = 1.f;
		}
		else
		{
			result = time;
		}
	}
	time = result;
	return UnityVector2(vector_a.x + (vector_b.x - vector_a.x) * time, vector_a.y + (vector_b.y - vector_a.y) * time);
}

UnityVector2 UnityVector2::LerpUnclamped(UnityVector2 vector_a, UnityVector2 vector_b, float time)
{
	return UnityVector2(vector_a.x + (vector_b.x - vector_a.x) * time, vector_a.y + (vector_b.y - vector_a.y) * time);
}

UnityVector2 UnityVector2::MoveTowards(UnityVector2 current, UnityVector2 target, float max_distance_delta)
{
	float num = target.x - current.x;
	float num2 = target.y - current.y;
	float num3 = num * num + num2 * num2;
	bool flag = num3 == 0.f || (max_distance_delta >= 0.f && num3 <= max_distance_delta * max_distance_delta);
	UnityVector2 result;
	if (flag)
	{
		result = target;
	}
	else
	{
		float num4 = sqrt(num3);
		result = UnityVector2(current.x + num / num4 * max_distance_delta, current.y + num2 / num4 * max_distance_delta);
	}
	return result;
}

UnityVector2 UnityVector2::SmoothDamp(UnityVector2 current, UnityVector2 target, UnityVector2& current_velocity, float smooth_time, float max_speed)
{
	float deltaTime = 0.03125f;//Time.deltaTime;
	return SmoothDamp(current, target, current_velocity, smooth_time, max_speed, deltaTime);
}

UnityVector2 UnityVector2::SmoothDamp(UnityVector2 current, UnityVector2 target, UnityVector2& current_velocity, float smooth_time)
{
	float delta_time = 0.03125f;// Time.deltaTime;
	return SmoothDamp(current, target, current_velocity, smooth_time, infinity, delta_time);
}

UnityVector2 UnityVector2::SmoothDamp(UnityVector2 current, UnityVector2 target, UnityVector2& current_velocity, float smooth_time, float max_speed, float delta_time)
{
	smooth_time = std::max(0.0001f, smooth_time);
	float num = 2.f / smooth_time;
	float num2 = num * delta_time;
	float num3 = 1.f / (1.f + num2 + 0.48f * num2 * num2 + 0.235f * num2 * num2 * num2);
	float num4 = current.x - target.x;
	float num5 = current.y - target.y;
	UnityVector2 vector = target;
	float num6 = max_speed * smooth_time;
	float num7 = num6 * num6;
	float num8 = num4 * num4 + num5 * num5;
	bool flag = num8 > num7;
	if (flag)
	{
		float num9 = sqrt(num8);
		num4 = num4 / num9 * num6;
		num5 = num5 / num9 * num6;
	}
	target.x = current.x - num4;
	target.y = current.y - num5;
	float num10 = (current_velocity.x + num * num4) * delta_time;
	float num11 = (current_velocity.y + num * num5) * delta_time;
	current_velocity.x = (current_velocity.x - num * num10) * num3;
	current_velocity.y = (current_velocity.y - num * num11) * num3;
	float num12 = target.x + (num4 + num10) * num3;
	float num13 = target.y + (num5 + num11) * num3;
	float num14 = vector.x - current.x;
	float num15 = vector.y - current.y;
	float num16 = num12 - vector.x;
	float num17 = num13 - vector.y;
	bool flag2 = num14 * num16 + num15 * num17 > 0.f;
	if (flag2)
	{
		num12 = vector.x;
		num13 = vector.y;
		current_velocity.x = (num12 - vector.x) / delta_time;
		current_velocity.y = (num13 - vector.y) / delta_time;
	}
	return UnityVector2(num12, num13);
}

UnityVector2 UnityVector2::Zero()
{
	return zero;
}

UnityVector2 UnityVector2::One()
{
	return one;
}

UnityVector2 UnityVector2::Up()
{
	return up;
}

UnityVector2 UnityVector2::Down()
{
	return down;
}

UnityVector2 UnityVector2::Left()
{
	return left;
}

UnityVector2 UnityVector2::Right()
{
	return right;
}

UnityVector2 UnityVector2::PositiveInfinity()
{
	return positive_infinity;
}

UnityVector2 UnityVector2::NegativeInfinity()
{
	return negative_infinity;
}

const float UnityVector2::epsilon = 0.00001f;
const float UnityVector2::infinity = std::numeric_limits<float>::infinity();
const UnityVector2 UnityVector2::zero = UnityVector2(0, 0);
const UnityVector2 UnityVector2::one = UnityVector2(1, 1);
const UnityVector2 UnityVector2::up = UnityVector2(0.f, 1.f);
const UnityVector2 UnityVector2::down = UnityVector2(0.f, -1.f);
const UnityVector2 UnityVector2::left = UnityVector2(-1.f, 0.f);
const UnityVector2 UnityVector2::right = UnityVector2(1.f, 0.f);
const UnityVector2 UnityVector2::positive_infinity = UnityVector2(infinity, infinity);
const UnityVector2 UnityVector2::negative_infinity = UnityVector2(-infinity, -infinity);
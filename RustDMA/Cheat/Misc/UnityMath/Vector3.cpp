#include "Vector3.hpp"
#include "Matrix3x3.hpp"
#include <limits>
#include <stdexcept>

UnityVector3::UnityVector3()
{
	this->x = 0.f;
	this->y = 0.f;
	this->z = 0.f;
}

UnityVector3::UnityVector3(float x, float y)
{
	this->x = x;
	this->y = y;
	this->z = 0.f;
}

UnityVector3::UnityVector3(float x, float y, float z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}

UnityVector3::UnityVector3(const float* array)
{
	this->x = array[0];
	this->y = array[1];
	this->z = array[2];
}

void UnityVector3::Set(float x, float y, float z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}

void UnityVector3::Set(const float* array)
{
	this->x = array[0];
	this->y = array[1];
	this->z = array[2];
}

void UnityVector3::Set(const UnityVector3& vector)
{
	this->x = vector.x;
	this->y = vector.y;
	this->z = vector.z;
}

void UnityVector3::Scale(const UnityVector3& scale)
{
	this->x *= scale.x;
	this->y *= scale.y;
	this->z *= scale.z;
}

UnityVector3 UnityVector3::Normalized()
{
	return Normalize(UnityVector3(this->x, this->y, this->z));
}

float UnityVector3::Magnitude()
{
	return sqrt(powf(this->x, 2) + powf(this->y, 2) + powf(this->z, 2));
}

float UnityVector3::SqrMagnitude()
{
	return powf(this->x, 2) + powf(this->y, 2) + powf(this->z, 2);
}

std::string UnityVector3::ToString()
{
	char buffer[64];
	sprintf(buffer, "X:%f, Y:%f  Z:%f", this->x, this->y, this->z);
	return buffer;
}

void UnityVector3::DebugPrint()
{
	printf("X:%f, Y:%f  Z:%f\n", this->x, this->y, this->z);
}

float* UnityVector3::GetPtr()
{
	return &x;
}

const float* UnityVector3::GetPtr() const
{
	return &x;
}

float& UnityVector3::operator[] (int i)
{
	if (i < 0 || i > 2)
	{
		throw std::out_of_range("UnityVector3 index out of range");
	}
	return (&x)[i];
}

const float& UnityVector3::operator[] (int i)const 
{ 
	if (i < 0 || i > 2)
	{
		throw std::out_of_range("UnityVector3 index out of range");
	}
	return (&x)[i];
}

bool UnityVector3::operator == (const UnityVector3& input_vector)
{
	return this->x == input_vector.x && this->y == input_vector.y && this->z == input_vector.z;
}

bool UnityVector3::operator != (const UnityVector3& input_vector)
{ 
	return this->x != input_vector.x || this->y != input_vector.y || this->z != input_vector.z;
}

UnityVector3& UnityVector3::operator += (const UnityVector3& input_vector)
{
	this->x += input_vector.x;
	this->y += input_vector.y;
	this->z += input_vector.z;
	return *this;
}

UnityVector3& UnityVector3::operator += (float scale)
{
	this->x += scale;
	this->y += scale;
	this->z += scale;
	return *this;
}

UnityVector3& UnityVector3::operator -= (const UnityVector3& input_vector)
{
	this->x -= input_vector.x;
	this->y -= input_vector.y;
	this->z -= input_vector.z;
	return *this;
}

UnityVector3& UnityVector3::operator -= (float scale)
{
	this->x -= scale;
	this->y -= scale;
	this->z -= scale;
	return *this;
}

UnityVector3& UnityVector3::operator *= (const UnityVector3& input_vector)
{
	this->x *= input_vector.x;
	this->y *= input_vector.y;
	this->z *= input_vector.z;
	return *this;
}

UnityVector3& UnityVector3::operator *= (float scale)
{
	this->x *= scale;
	this->y *= scale;
	this->z *= scale;
	return *this;
}

UnityVector3& UnityVector3::operator /= (const UnityVector3& input_vector)
{
	this->x /= input_vector.x;
	this->y /= input_vector.y;
	this->z /= input_vector.z;
	return *this;
}

UnityVector3& UnityVector3::operator /= (float scale)
{
	this->x /= scale;
	this->y /= scale;
	this->z /= scale;
	return *this;
}

UnityVector3 UnityVector3::operator - ()const
{ 
	return UnityVector3(-x, -y, -z);
}

UnityVector3 operator + (const UnityVector3& lhs, const UnityVector3& rhs)
{
	return UnityVector3(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z);
}

UnityVector3 operator + (const UnityVector3& lhs, float scale)
{
	return UnityVector3(lhs.x + scale, lhs.y + scale, lhs.z + scale);
}

UnityVector3 operator + (float scale, const UnityVector3& rhs)
{
	return UnityVector3(scale + rhs.x, scale + rhs.y, scale + rhs.z);
}

UnityVector3 operator - (const UnityVector3& lhs, const UnityVector3& rhs)
{
	return UnityVector3(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z);
}

UnityVector3 operator - (const UnityVector3& lhs, float scale)
{
	return UnityVector3(lhs.x - scale, lhs.y - scale, lhs.z - scale);
}

UnityVector3 operator - (float scale, const UnityVector3& rhs)
{
	return UnityVector3(scale - rhs.x, scale - rhs.y, scale - rhs.z);
}

UnityVector3 operator * (const UnityVector3& lhs, const UnityVector3& rhs)
{
	return UnityVector3(lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z);
}

UnityVector3 operator * (const UnityVector3& lhs, float scale)
{
	return UnityVector3(lhs.x * scale, lhs.y * scale, lhs.z * scale);
}

UnityVector3 operator * (float scale, const UnityVector3& rhs)
{
	return UnityVector3(scale * rhs.x, scale * rhs.y, scale * rhs.z);
}

UnityVector3 operator / (const UnityVector3& lhs, const UnityVector3& rhs)
{
	if (rhs.x == 0.0f || rhs.y == 0.0f || rhs.z == 0.0f)
	{
		throw std::runtime_error("Division by zero is not allowed.");
	}
	return UnityVector3(lhs.x / rhs.x, lhs.y / rhs.y, lhs.z / rhs.z);
}

UnityVector3 operator / (const UnityVector3& lhs, float scale)
{
	if (scale == 0.0f)
	{
		throw std::runtime_error("Division by zero is not allowed.");
	}
	return UnityVector3(lhs.x / scale, lhs.y / scale, lhs.z / scale);
}

UnityVector3 operator / (float scale, const UnityVector3& rhs)
{
	if (rhs.x == 0.0f || rhs.y == 0.0f || rhs.z == 0.0f)
	{
		throw std::runtime_error("Division by zero is not allowed.");
	}
	return UnityVector3(scale / rhs.x, scale / rhs.y, scale / rhs.z);
}

UnityVector3 UnityVector3::Scale(UnityVector3 vector_a, UnityVector3 vector_b)
{
	return UnityVector3(vector_a.x * vector_b.x, vector_a.y * vector_b.y, vector_a.z * vector_b.z);
}

UnityVector3 UnityVector3::Cross(UnityVector3 lhs, UnityVector3 rhs)
{
	return UnityVector3(lhs.y * rhs.z - lhs.z * rhs.y, lhs.z * rhs.x - lhs.x * rhs.z, lhs.x * rhs.y - lhs.y * rhs.x);
}

float UnityVector3::Dot(UnityVector3 lhs, UnityVector3 rhs)
{
	return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z;
}

UnityVector3 UnityVector3::Normalize(UnityVector3 vector)
{
	UnityVector3 result;
	float num = Magnitude(vector);
	(num > epsilon) ? result = vector / num : result = UnityVector3::Zero();
	return result;
}

void UnityVector3::OrthoNormalize(UnityVector3* inU, UnityVector3* inV, UnityVector3* inW)
{
	// compute u0
	float mag = Magnitude(*inU);
	if (mag > UnityVector3::epsilon)
		*inU /= mag;
	else
		*inU = UnityVector3(1.0F, 0.0F, 0.0F);

	// compute u1
	float dot0 = Dot(*inU, *inV);
	*inV -= dot0 * *inU;
	mag = Magnitude(*inV);
	if (mag > UnityVector3::epsilon)
		*inV /= mag;
	else
		*inV = OrthoNormalVectorFast(*inU);

	// compute u2
	float dot1 = Dot(*inV, *inW);
	dot0 = Dot(*inU, *inW);
	*inW -= dot0 * *inU + dot1 * *inV;
	mag = Magnitude(*inW);
	if (mag > UnityVector3::epsilon)
		*inW /= mag;
	else
		*inW = Cross(*inU, *inV);
}

UnityVector3 UnityVector3::OrthoNormalVectorFast(const UnityVector3& n)
{
	UnityVector3 res;
	if (abs(n.z) > 0.7071067811865475244008443621048490)
	{
		// choose p in y-z plane
		float a = n.y * n.y + n.z * n.z;
		float k = 1.0F / sqrt(a);
		res.x = 0;
		res.y = -n.z * k;
		res.z = n.y * k;
	}
	else
	{
		// choose p in x-y plane
		float a = n.x * n.x + n.y * n.y;
		float k = 1.0F / sqrt(a);
		res.x = -n.y * k;
		res.y = n.x * k;
		res.z = 0;
	}
	return res;
}

float UnityVector3::Distance(UnityVector3 vector_a, UnityVector3 vector_b)
{
	return sqrt(pow(vector_a.x - vector_b.x, 2) + pow(vector_a.y - vector_b.y, 2) + pow(vector_a.z - vector_b.z, 2));
}

UnityVector3 UnityVector3::ClampMagnitude(UnityVector3 vector, float max_length)
{
	UnityVector3 result = vector;
	float sqr_magnitude = vector.SqrMagnitude();
	if (sqr_magnitude > pow(max_length, 2))
	{
		float num = sqrt(sqr_magnitude);
		float num2 = vector.x / num;
		float num3 = vector.y / num;
		float num4 = vector.z / num;
		result = UnityVector3(num2 * max_length, num3 * max_length, num4 * max_length);
	}
	return result;
}

float UnityVector3::Magnitude(UnityVector3 vector)
{
	return sqrt(powf(vector.x, 2) + powf(vector.y, 2) + powf(vector.z, 2));
}

float UnityVector3::SqrMagnitude(UnityVector3 vector)
{
	return vector.x * vector.x + vector.y * vector.y + vector.z * vector.z;
}

UnityVector3 UnityVector3::Min(UnityVector3 lhs, UnityVector3 rhs)
{
	return UnityVector3(std::min(lhs.x, rhs.x), std::min(lhs.y, rhs.y), std::min(lhs.z, rhs.z));
}

UnityVector3 UnityVector3::Max(UnityVector3 lhs, UnityVector3 rhs)
{
	return UnityVector3(std::max(lhs.x, rhs.x), std::max(lhs.y, rhs.y), std::max(lhs.z, rhs.z));
}

UnityVector3 UnityVector3::Reflect(UnityVector3 input_direction, UnityVector3 input_normal)
{
	float num = -2.f * Dot(input_normal, input_direction);
	return UnityVector3(num * input_normal.x + input_direction.x, num * input_normal.y + input_direction.y, num * input_normal.z + input_direction.z);
}

float UnityVector3::Angle(UnityVector3 from, UnityVector3 to)
{
	float result;
	float num = sqrt((from.SqrMagnitude() * to.SqrMagnitude()));
	if (num > 1E-15f)
	{
		float dot = Dot(from, to) / num;
		if (dot < -1.f)
		{
			dot = -1.f;
		}
		else
		{
			if (dot > 1.f)
			{
				dot = 1.f;
			}
		}
		result = acos(dot) * 57.29578f;
	}
	else
	{
		result = 0.f;
	}
	return result;
}

float UnityVector3::AngleBetween(UnityVector3 from, UnityVector3 to)
{
	float dot = Dot(from.Normalized(), to.Normalized());
	if (dot < -1.f)
	{
		dot = -1.f;
	}
	else
	{
		if (dot > 1.f)
		{
			dot = 1.f;
		}
	}
	return acos(dot);
}

float UnityVector3::SignedAngle(UnityVector3 from, UnityVector3 to, UnityVector3 axis)
{
	float num = Angle(from, to);
	float num2 = from.y * to.z - from.z * to.y;
	float num3 = from.z * to.x - from.x * to.z;
	float num4 = from.x * to.y - from.y * to.x;
	float num5 = ((axis.x * num2 + axis.y * num3 + axis.z * num4) >= 0.f) ? 1.f : -1.f;
	return num * num5;
}

UnityVector3 UnityVector3::CalculateAngle(UnityVector3 src, UnityVector3 dst)
{
	UnityVector3 direction = UnityVector3(src.x - dst.x, src.y - dst.y, src.z - dst.z);
	return UnityVector3((asin(direction.y / direction.Magnitude())) * (180 / 3.1415926f), (-atan2(direction.x, -direction.z)) * (180 / 3.1415926f), 0.f);
}

UnityVector3 UnityVector3::InverseSafe(UnityVector3 v)
{
	return UnityVector3(FloatConversion::InverseSafe(v.x), FloatConversion::InverseSafe(v.y), FloatConversion::InverseSafe(v.z));
}

UnityVector3 UnityVector3::Project(UnityVector3 vector, UnityVector3 on_normal)
{
	UnityVector3 result;
	float num = Dot(on_normal, on_normal);
	if (num < epsilon)
	{
		result = UnityVector3::Zero();
	}
	else
	{
		float num2 = Dot(vector, on_normal);
		result = UnityVector3(on_normal.x * num2 / num, on_normal.y * num2 / num, on_normal.z * num2 / num);
	}
	return result;
}

UnityVector3 UnityVector3::ProjectOnPlane(UnityVector3 vector, UnityVector3 plane_normal)
{
	UnityVector3 result;
	float num = Dot(plane_normal, plane_normal);
	if (num < epsilon)
	{
		result = vector;
	}
	else
	{
		float num2 = Dot(vector, plane_normal);
		result = UnityVector3(vector.x - plane_normal.x * num2 / num, vector.y - plane_normal.y * num2 / num, vector.z - plane_normal.z * num2 / num);
	}
	return result;
}

UnityVector3 UnityVector3::Exclude(UnityVector3 exclude_this, UnityVector3 from_that)
{
	return ProjectOnPlane(from_that, exclude_this);
}

UnityVector3 UnityVector3::Lerp(UnityVector3 vector_a, UnityVector3 vector_b, float time)
{
	float result;
	if (time < 0.f)
	{
		result = 0.f;
	}
	else
	{
		if (time > 1.f)
		{
			result = 1.f;
		}
		else
		{
			result = time;
		}
	}

	time = result;
	return UnityVector3(vector_a.x + (vector_b.x - vector_a.x) * time, vector_a.y + (vector_b.y - vector_a.y) * time, vector_a.z + (vector_b.z - vector_a.z) * time);
}

UnityVector3 UnityVector3::LerpUnclamped(UnityVector3 vector_a, UnityVector3 vector_b, float time)
{
	return UnityVector3(vector_a.x + (vector_b.x - vector_a.x) * time, vector_a.y + (vector_b.y - vector_a.y) * time, vector_a.z + (vector_b.z - vector_a.z) * time);
}

UnityVector3 UnityVector3::MoveTowards(UnityVector3 current, UnityVector3 target, float max_distance_delta)
{
	float num = target.x - current.x;
	float num2 = target.y - current.y;
	float num3 = target.z - current.z;
	float num4 = num * num + num2 * num2 + num3 * num3;
	bool flag = num4 == 0.f || (max_distance_delta >= 0.f  && num4 <= max_distance_delta * max_distance_delta);
	UnityVector3 result;
	if (flag)
	{
		result = target;
	}
	else
	{
		float num5 = sqrt(num4);
		result = UnityVector3(current.x + num / num5 * max_distance_delta, current.y + num2 / num5 * max_distance_delta, current.z + num3 / num5 * max_distance_delta);
	}
	return result;
}

UnityVector3 UnityVector3::SmoothDamp(UnityVector3 current, UnityVector3 target, UnityVector3 current_velocity, float smooth_time, float max_speed)
{
	float delta_time = 0.03125f;//Time.deltaTime;
	return SmoothDamp(current, target, current_velocity, smooth_time, max_speed, delta_time);
}

UnityVector3 UnityVector3::SmoothDamp(UnityVector3 current, UnityVector3 target, UnityVector3 current_velocity, float smooth_time)
{
	float delta_time = 0.03125f;//Time.deltaTime;
	return SmoothDamp(current, target, current_velocity, smooth_time, infinity, delta_time);
}

UnityVector3 UnityVector3::SmoothDamp(UnityVector3 current, UnityVector3 target, UnityVector3 current_velocity, float smooth_time, float max_speed, float delta_time)
{
	smooth_time = std::max(0.0001f, smooth_time);
	float num = 2.f / smooth_time;
	float num2 = num * delta_time;
	float num3 = 1.f / (1.f + num2 + 0.48f * num2 * num2 + 0.235f * num2 * num2 * num2);
	float num4 = current.x - target.x;
	float num5 = current.y - target.y;
	float num6 = current.z - target.z;
	UnityVector3 vector = target;
	float num7 = max_speed * smooth_time;
	float num8 = num7 * num7;
	float num9 = num4 * num4 + num5 * num5 + num6 * num6;
	bool flag = num9 > num8;
	if (flag)
	{
		float num10 = sqrt(num9);
		num4 = num4 / num10 * num7;
		num5 = num5 / num10 * num7;
		num6 = num6 / num10 * num7;
	}
	target.x = current.x - num4;
	target.y = current.y - num5;
	target.z = current.z - num6;
	float num11 = (current_velocity.x + num * num4) * delta_time;
	float num12 = (current_velocity.y + num * num5) * delta_time;
	float num13 = (current_velocity.z + num * num6) * delta_time;
	current_velocity.x = (current_velocity.x - num * num11) * num3;
	current_velocity.y = (current_velocity.y - num * num12) * num3;
	current_velocity.z = (current_velocity.z - num * num13) * num3;
	float num14 = target.x + (num4 + num11) * num3;
	float num15 = target.y + (num5 + num12) * num3;
	float num16 = target.z + (num6 + num13) * num3;
	float num17 = vector.x - current.x;
	float num18 = vector.y - current.y;
	float num19 = vector.z - current.z;
	float num20 = num14 - vector.x;
	float num21 = num15 - vector.y;
	float num22 = num16 - vector.z;
	bool flag2 = num17 * num20 + num18 * num21 + num19 * num22 > 0.f;
	if (flag2)
	{
		num14 = vector.x;
		num15 = vector.y;
		num16 = vector.z;
		current_velocity.x = (num14 - vector.x) / delta_time;
		current_velocity.y = (num15 - vector.y) / delta_time;
		current_velocity.z = (num16 - vector.z) / delta_time;
	}
	return UnityVector3(num14, num15, num16);
}

UnityVector3 UnityVector3::Zero()
{
	return zero;
}

UnityVector3 UnityVector3::One()
{
	return one;
}

UnityVector3 UnityVector3::Up()
{
	return up;
}

UnityVector3 UnityVector3::Down()
{
	return down;
}

UnityVector3 UnityVector3::Left()
{
	return left;
}

UnityVector3 UnityVector3::Right()
{
	return right;
}

UnityVector3 UnityVector3::Forward()
{
	return forward;
}

UnityVector3 UnityVector3::Back()
{
	return back;
}

UnityVector3 UnityVector3::PositiveInfinity()
{
	return positive_infinity;
}

UnityVector3 UnityVector3::NegativeInfinity()
{
	return negative_infinity;
}

const float UnityVector3::epsilon = 0.00001f;
const float UnityVector3::infinity = std::numeric_limits<float>::infinity();
const UnityVector3 UnityVector3::zero = UnityVector3(0.f, 0.f, 0.f);
const UnityVector3 UnityVector3::one = UnityVector3(1.f, 1.f, 1.f);
const UnityVector3 UnityVector3::up = UnityVector3(0.f, 1.f, 0.f);
const UnityVector3 UnityVector3::down = UnityVector3(0.f, -1.f, 0.f);
const UnityVector3 UnityVector3::left = UnityVector3(-1.f, 0.f, 0.f);
const UnityVector3 UnityVector3::right = UnityVector3(1.f, 0.f, 0.f);
const UnityVector3 UnityVector3::forward = UnityVector3(0.f, 0.f, 1.f);
const UnityVector3 UnityVector3::back = UnityVector3(0.f, 0.f, -1.f);
const UnityVector3 UnityVector3::positive_infinity = UnityVector3(infinity, infinity, infinity);
const UnityVector3 UnityVector3::negative_infinity = UnityVector3(-infinity, -infinity, -infinity);
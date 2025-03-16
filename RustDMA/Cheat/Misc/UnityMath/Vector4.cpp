#include "Vector4.hpp"

UnityVector4::UnityVector4()
{
	this->x = 0.f;
	this->y = 0.f;
	this->z = 0.f;
	this->w = 0.f;
}

UnityVector4::UnityVector4(float x, float y, float z, float w)
{
	this->x = x;
	this->y = y;
	this->z = z;
	this->w = w;
}

UnityVector4::UnityVector4(const float* array)
{
	this->x = array[0];
	this->y = array[1];
	this->z = array[2];
	this->w = array[3];
}

void UnityVector4::Set(float x, float y, float z, float w)
{
	this->x = x;
	this->y = y;
	this->z = z;
	this->w = w;
}

void UnityVector4::Set(const float* array)
{
	this->x = array[0];
	this->y = array[1];
	this->z = array[2];
	this->w = array[3];
}

void UnityVector4::Scale(UnityVector4 scale)
{
	this->x *= scale.x;
	this->y *= scale.y;
	this->z *= scale.z;
	this->w *= scale.w;
}

UnityVector4 UnityVector4::Normalized()
{
	return Normalize(UnityVector4(this->x, this->y, this->z, this->w));
}

float UnityVector4::Magnitude()
{
	return sqrt(Dot(UnityVector4(this->x, this->y, this->z, this->w), UnityVector4(this->x, this->y, this->z, this->w)));
}

float UnityVector4::SqrMagnitude()
{
	return Dot(UnityVector4(this->x, this->y, this->z, this->w), UnityVector4(this->x, this->y, this->z, this->w));
}

std::string UnityVector4::ToString()
{
	char buffer[64];
	sprintf(buffer, "X:%f, Y:%f Z:%f W:%f", x, y, z, w);
	return buffer;
}

void UnityVector4::DebugPrint()
{
	printf("X:%f, Y:%f Z:%f W:%f\n", x, y, z, w);
}

float* UnityVector4::GetPtr()
{
	return &x;
}

const float* UnityVector4::GetPtr() const
{
	return &x;
}

float& UnityVector4::operator[] (int i)
{
	if (i < 0 || i > 3)
	{
		throw std::out_of_range("Vector3 index out of range");
	}
	return (&x)[i];
}

const float& UnityVector4::operator[] (int i)const
{
	if (i < 0 || i > 3)
	{
		throw std::out_of_range("Vector3 index out of range");
	}
	return (&x)[i];
}

bool UnityVector4::operator == (const UnityVector4& input_vector) const
{
	return this->x == input_vector.x && this->y == input_vector.y && this->z == input_vector.z && this->w == input_vector.w;
}

bool UnityVector4::operator != (const UnityVector4& input_vector) const
{
	return this->x != input_vector.x || this->y != input_vector.y || this->z != input_vector.z || this->w != input_vector.w;
}

bool UnityVector4::operator == (const float array[4]) const
{
	return this->x == array[0] && this->y == array[1] && this->z == array[2] && this->w == array[3];
}

bool UnityVector4::operator != (const float array[4]) const
{
	return this->x != array[0] || this->y != array[1] || this->z != array[2] || this->w != array[3];
}

UnityVector4& UnityVector4::operator += (const UnityVector4& input_vector)
{
	this->x += input_vector.x;
	this->y += input_vector.y;
	this->z += input_vector.z;
	this->w += input_vector.w;
	return *this;
}

UnityVector4& UnityVector4::operator += (float scale)
{
	this->x += scale;
	this->y += scale;
	this->z += scale;
	this->w += scale;
	return *this;
}

UnityVector4& UnityVector4::operator -= (const UnityVector4& input_vector)
{
	this->x -= input_vector.x;
	this->y -= input_vector.y;
	this->z -= input_vector.z;
	this->w -= input_vector.w;
	return *this;
}

UnityVector4& UnityVector4::operator -= (float scale)
{
	this->x -= scale;
	this->y -= scale;
	this->z -= scale;
	this->w -= scale;
	return *this;
}

UnityVector4& UnityVector4::operator *= (const UnityVector4& input_vector)
{
	this->x *= input_vector.x;
	this->y *= input_vector.y;
	this->z *= input_vector.z;
	this->w *= input_vector.w;
	return *this;
}

UnityVector4& UnityVector4::operator *= (float scale)
{
	this->x *= scale;
	this->y *= scale;
	this->z *= scale;
	this->w *= scale;
	return *this;
}

UnityVector4& UnityVector4::operator /= (const UnityVector4& input_vector)
{
	this->x /= input_vector.x;
	this->y /= input_vector.y;
	this->z /= input_vector.z;
	this->w /= input_vector.w;
	return *this;
}

UnityVector4& UnityVector4::operator /= (float scale)
{
	this->x /= scale;
	this->y /= scale;
	this->z /= scale;
	this->w /= scale;
	return *this;
}

UnityVector4 UnityVector4::operator- () const
{
	return UnityVector4(-this->x, -this->y, -this->z, -this->w);
}

UnityVector4 operator + (const UnityVector4& lhs, const UnityVector4& rhs)
{
	return UnityVector4(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z, lhs.w + rhs.w);
}

UnityVector4 operator + (const UnityVector4& lhs, float scale)
{
	return UnityVector4(lhs.x + scale, lhs.y + scale, lhs.z + scale, lhs.w + scale);
}

UnityVector4 operator + (float scale, const UnityVector4& rhs)
{
	return UnityVector4(scale + rhs.x, scale + rhs.y, scale + rhs.z, scale + rhs.w);
}

UnityVector4 operator - (const UnityVector4& lhs, const UnityVector4& rhs)
{
	return UnityVector4(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z, lhs.w - rhs.w);
}

UnityVector4 operator - (const UnityVector4& lhs, float scale)
{
	return UnityVector4(lhs.x - scale, lhs.y - scale, lhs.z - scale, lhs.w - scale);
}

UnityVector4 operator - (float scale, const UnityVector4& rhs)
{
	return UnityVector4(scale - rhs.x, scale - rhs.y, scale - rhs.z, scale - rhs.w);
}

UnityVector4 operator * (const UnityVector4& lhs, const UnityVector4& rhs)
{
	return UnityVector4(lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z, lhs.w * rhs.w);
}

UnityVector4 operator * (const UnityVector4& lhs, float scale)
{
	return UnityVector4(lhs.x * scale, lhs.y * scale, lhs.z * scale, lhs.w * scale);
}

UnityVector4 operator * (float scale, const UnityVector4& rhs)
{
	return UnityVector4(scale * rhs.x, scale * rhs.y, scale * rhs.z, scale * rhs.w);
}

UnityVector4 operator / (const UnityVector4& lhs, const UnityVector4& rhs)
{
	if (rhs.x == 0.0f || rhs.y == 0.0f || rhs.z == 0.0f || rhs.w == 0.0f)
	{
		throw std::runtime_error("Division by zero is not allowed.");
	}
	return UnityVector4(lhs.x / rhs.x, lhs.y / rhs.y, lhs.z / rhs.z, lhs.w / rhs.w);
}

UnityVector4 operator / (const UnityVector4& lhs, float scale)
{
	if (scale == 0.0f)
	{
		throw std::runtime_error("Division by zero is not allowed.");
	}
	return UnityVector4(lhs.x / scale, lhs.y / scale, lhs.z / scale, lhs.z / scale);
}

UnityVector4 operator / (float scale, const UnityVector4& rhs)
{
	if (rhs.x == 0.0f || rhs.y == 0.0f || rhs.z == 0.0f)
	{
		throw std::runtime_error("Division by zero is not allowed.");
	}
	return UnityVector4(scale / rhs.x, scale / rhs.y, scale / rhs.z, scale / rhs.w);
}

UnityVector4 UnityVector4::Scale(UnityVector4 vector_a, UnityVector4 vector_b)
{
	return UnityVector4(vector_a.x * vector_b.x, vector_a.y * vector_b.y, vector_a.z * vector_b.z, vector_a.w * vector_b.w);
}

float UnityVector4::Dot(UnityVector4 vector_a, UnityVector4 vector_b)
{
	return vector_a.x * vector_b.x + vector_a.y * vector_b.y + vector_a.z * vector_b.z + vector_a.w * vector_b.w;
}

UnityVector4 UnityVector4::Normalize(UnityVector4 vector_a)
{
	UnityVector4 result;
	float num = Magnitude(vector_a);
	if (num > 1E-05f)
	{
		result = vector_a / num;
	}
	else
	{
		result = UnityVector4(0, 0, 0, 0);
	}
	return result;
}

float UnityVector4::Distance(UnityVector4 vector_a, UnityVector4 vector_b)
{
	return Magnitude(vector_a - vector_b);
}

float UnityVector4::Magnitude(UnityVector4 vector_a)
{
	return sqrt(Dot(vector_a, vector_a));
}

float UnityVector4::SqrMagnitude(UnityVector4 vector_a)
{
	return Dot(vector_a, vector_a);
}

UnityVector4 UnityVector4::Min(UnityVector4 lhs, UnityVector4 rhs)
{
	return UnityVector4(std::min(lhs.x, rhs.x), std::min(lhs.y, rhs.y), std::min(lhs.z, rhs.z), std::min(lhs.w, rhs.w));
}

UnityVector4 UnityVector4::Max(UnityVector4 lhs, UnityVector4 rhs)
{
	return UnityVector4(std::max(lhs.x, rhs.x), std::max(lhs.y, rhs.y), std::max(lhs.z, rhs.z), std::max(lhs.w, rhs.w));
}

UnityVector4 UnityVector4::Project(UnityVector4 vector_a, UnityVector4 vector_b)
{
	return vector_b * (Dot(vector_a, vector_b) / Dot(vector_b, vector_b));
}

UnityVector4 UnityVector4::Lerp(UnityVector4 vector_a, UnityVector4 vector_b, float time)
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
	return UnityVector4(vector_a.x + (vector_b.x - vector_a.x) * time, vector_a.y + (vector_b.y - vector_a.y) * time, vector_a.z + (vector_b.z - vector_a.z) * time, vector_a.w + (vector_b.w - vector_a.w) * time);
}

UnityVector4 UnityVector4::LerpUnclamped(UnityVector4 vector_a, UnityVector4 vector_b, float time)
{
	return UnityVector4(vector_a.x + (vector_b.x - vector_a.x) * time, vector_a.y + (vector_b.y - vector_a.y) * time, vector_a.z + (vector_b.z - vector_a.z) * time, vector_a.w + (vector_b.w - vector_a.w) * time);
}

UnityVector4 UnityVector4::MoveTowards(UnityVector4 current, UnityVector4 target, float max_distance_delta)
{
	float num = target.x - current.x;
	float num2 = target.y - current.y;
	float num3 = target.z - current.z;
	float num4 = target.w - current.w;
	float num5 = num * num + num2 * num2 + num3 * num3 + num4 * num4;
	bool flag = num5 == 0.f || (max_distance_delta >= 0.f && num5 <= max_distance_delta * max_distance_delta);
	UnityVector4 result;
	if (flag)
	{
		result = target;
	}
	else
	{
		float num6 = sqrt(num5);
		result = UnityVector4(current.x + num / num6 * max_distance_delta, current.y + num2 / num6 * max_distance_delta, current.z + num3 / num6 * max_distance_delta, current.w + num4 / num6 * max_distance_delta);
	}
	return result;
}

UnityVector4 UnityVector4::Zero()
{
	return zero;
}

UnityVector4 UnityVector4::One()
{
	return one;
}

UnityVector4 UnityVector4::PositiveInfinity()
{
	return positive_infinity;
}

UnityVector4 UnityVector4::NegativeInfinity()
{
	return negative_infinity;
}

const float UnityVector4::epsilon = 0.00001f;
const float UnityVector4::infinity = std::numeric_limits<float>::infinity();
const UnityVector4 UnityVector4::zero = UnityVector4(0, 0, 0, 0);
const UnityVector4 UnityVector4::one = UnityVector4(1, 1, 1, 1);
const UnityVector4 UnityVector4::positive_infinity = UnityVector4(infinity, infinity, infinity, infinity);
const UnityVector4 UnityVector4::negative_infinity = UnityVector4(-infinity, -infinity, -infinity, -infinity);
#include "FloatConversion.hpp"

float FloatConversion::FloatMin(float a, float b)
{
	return std::min(a, b);
}

float FloatConversion::FloatMax(float a, float b)
{
	return std::max(a, b);
}

float FloatConversion::Abs(float v)
{
	return v < 0.0F ? -v : v;
}

double FloatConversion::Abs(double v)
{
	return v < 0.0 ? -v : v;
}

int FloatConversion::Abs(int v)
{
	return v < 0 ? -v : v;
}


int FloatConversion::FloorfToInt(float f)
{
	return f >= 0 ? (int)f : (int)(f - BiggestFloatSmallerThanOne);
}

unsigned long FloatConversion::FloorfToIntPos(float f)
{
	return (unsigned long)f;
}

float FloatConversion::Floorf(float f)
{
	return floor(f);
}

double FloatConversion::Floord(double f)
{
	return floor(f);
}


int FloatConversion::CeilfToInt(float f)
{
	return f >= 0 ? (int)(f + BiggestFloatSmallerThanOne) : (int)(f);
}

unsigned long FloatConversion::CeilfToIntPos(float f)
{
	return (unsigned long)(f + BiggestFloatSmallerThanOne);
}

float FloatConversion::Ceilf(float f)
{
	return ceil(f);
}

double FloatConversion::Ceild(double f)
{
	return ceil(f);
}


int FloatConversion::RoundfToInt(float f)
{
	return FloorfToInt(f + 0.5F);
}

unsigned long FloatConversion::RoundfToIntPos(float f)
{
	return FloorfToIntPos(f + 0.5F);
}

float FloatConversion::Roundf(float f)
{
	return Floorf(f + 0.5F);
}

double FloatConversion::Roundf(double f)
{
	return Floord(f + 0.5);
}

int FloatConversion::NormalizedToWord(float f)
{
	f = FloatMax(f, 0.0F);
	f = FloatMin(f, 1.0F);
	return RoundfToIntPos(f * 65535.0f);
}

float FloatConversion::WordToNormalized(int p)
{
	return (float)p / 65535.0F;
}

int FloatConversion::NormalizedToByte(float f)
{
	f = FloatMax(f, 0.0F);
	f = FloatMin(f, 1.0F);
	return RoundfToIntPos(f * 255.0f);
}

float FloatConversion::ByteToNormalized(int p)
{
	return (float)p / 255.0F;
}

float FloatConversion::Repeat(float t, float length)
{
	return t - Floorf(t / length) * length;
}

double FloatConversion::RepeatD(double t, double length)
{
	return t - floor(t / length) * length;
}

float FloatConversion::DeltaAngleRad(float current, float target)
{
	float delta = Repeat((target - current), 2 * PI);
	if (delta > PI)
		delta -= 2 * PI;
	return delta;
}

bool FloatConversion::CompareApproximately(float f0, float f1, float epsilon)
{
	float dist = (f0 - f1);
	dist = Abs(dist);
	return dist < epsilon;
}

float FloatConversion::CopySignf(float x, float y)
{
	union
	{
		float f;
		unsigned long i;
	} u, u0, u1;
	u0.f = x; u1.f = y;
	unsigned long a = u0.i;
	unsigned long b = u1.i;
	signed long mask = 1 << 31;
	unsigned long sign = b & mask;
	a &= ~mask;
	a |= sign;

	u.i = a;
	return u.f;
}

int FloatConversion::CompareFloatRobustSignUtility(float A)
{
	union
	{
		float f;
		int i;
	} u;
	u.f = A;
	return (u.i) & 0x80000000;
}

bool FloatConversion::CompareFloatRobust(float f0, float f1, int maxUlps)
{
	if (CompareFloatRobustSignUtility(f0) != CompareFloatRobustSignUtility(f1))
	{
		return f0 == f1;
	}

	union
	{
		float f;
		int i;
	} u0, u1;
	u0.f = f0;
	u1.f = f1;
	int aInt = u0.i;

	if (aInt < 0)
	{
		aInt = 0x80000000 - aInt;
	}

	int bInt = u1.i;
	if (bInt < 0)
	{
		bInt = 0x80000000 - bInt;
	}

	int intDiff = Abs(aInt - bInt);
	if (intDiff <= maxUlps)
	{
		return true;
	}
	return false;
}

float FloatConversion::InverseSafe(float f)
{
	if (Abs(f) > 0.00001F)
		return 1.0F / f;
	else
		return 0.0F;
}

template<class T>
T FloatConversion::Sqr(const T& t)
{
	return t * t;
}

float FloatConversion::Deg2Rad(float deg)
{
	return deg / 360.0F * 2.0F * PI;
}

float FloatConversion::Rad2Deg(float rad)
{
	return rad / 2.0F / PI * 360.0F;
}

float FloatConversion::Lerp(float from, float to, float t)
{
	return to * t + from * (1.0F - t);
}

bool FloatConversion::IsNAN(float value)
{
	return _isnan(value) != 0;
}

bool FloatConversion::IsNAN(double value)
{
	return _isnan(value) != 0;
}

bool FloatConversion::IsPlusInf(float value)
{
	return value == std::numeric_limits<float>::infinity();
}

bool FloatConversion::IsMinusInf(float value)
{
	return value == -std::numeric_limits<float>::infinity();
}

bool FloatConversion::IsFinite(const float& value)
{
	unsigned long intval = *reinterpret_cast<const unsigned long*>(&value);
	return (intval & 0x7f800000) != 0x7f800000;
}

bool FloatConversion::IsFinite(const double& value)
{
	unsigned long long intval = *reinterpret_cast<const unsigned long long*>(&value);
	return (intval & 0x7ff0000000000000LL) != 0x7ff0000000000000LL;
}

float FloatConversion::InvSqrt(float p)
{
	return 1.0F / sqrt(p);
}

float FloatConversion::Sqrt(float p)
{
	return sqrt(p);
}

float FloatConversion::FastSqrt(float value)
{
	return sqrtf(value);
}

float FloatConversion::FastInvSqrt(float f)
{
	if (fabs(f) == 0.0F)
		return f;
	return 1.0F / sqrtf(f);
}

float FloatConversion::FastestInvSqrt(float f)
{
	union
	{
		float f;
		int i;
	} u;
	float fhalf = 0.5f * f;
	u.f = f;
	int i = u.i;
	i = 0x5f3759df - (i >> 1);
	u.i = i;
	f = u.f;
	f = f * (1.5f - fhalf * f * f);
	return f;
}

float FloatConversion::SqrtImpl(float f)
{
	return sqrt(f);
}
float FloatConversion::Sin(float f)
{
	return sinf(f);
}

float FloatConversion::Pow(float f, float f2)
{
	return powf(f, f2);
}

float FloatConversion::Cos(float f)
{
	return cosf(f);
}

float FloatConversion::Sign(float f)
{
	if (f < 0.0F)
		return -1.0F;
	else
		return 1.0;
}

const float FloatConversion::BiggestFloatSmallerThanOne = 0.99999994f;
const double FloatConversion::BiggestDoubleSmallerThanOne = 0.99999999999999989;
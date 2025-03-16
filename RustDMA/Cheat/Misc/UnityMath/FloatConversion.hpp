#ifndef FLOATCONVERSION_HPP
#define FLOATCONVERSION_HPP

#include <algorithm>
#include <cmath>
#include <limits>
#include <math.h>

class FloatConversion
{
public:
	static float FloatMin(float a, float b);
	static float FloatMax(float a, float b);
	static float Abs(float v);
	static double Abs(double v);
	static int Abs(int v);
	static int FloorfToInt(float f);
	static unsigned long FloorfToIntPos(float f);
	static float Floorf(float f);
	static double Floord(double f);
	static int CeilfToInt(float f);
	static unsigned long CeilfToIntPos(float f);
	static float Ceilf(float f);
	static double Ceild(double f);
	static int RoundfToInt(float f);
	static unsigned long RoundfToIntPos(float f);
	static float Roundf(float f);
	static double Roundf(double f);
	static int NormalizedToWord(float f);
	static float WordToNormalized(int p);
	static int NormalizedToByte(float f);
	static float ByteToNormalized(int p);
	static float Repeat(float t, float length);
	static double RepeatD(double t, double length);
	static float DeltaAngleRad(float current, float target);
	static bool CompareApproximately(float f0, float f1, float epsilon = 0.000001F);
	static float CopySignf(float x, float y);
	static int CompareFloatRobustSignUtility(float A);
	static bool CompareFloatRobust(float f0, float f1, int maxUlps = 10);
	static float InverseSafe(float f);

	template<class T>
	static T Sqr(const T& t);

	static float Deg2Rad(float deg);
	static float Rad2Deg(float rad);
	static float Lerp(float from, float to, float t);
	static bool IsNAN(float value);
	static bool IsNAN(double value);
	static bool IsPlusInf(float value);
	static bool IsMinusInf(float value);
	static bool IsFinite(const float& value);
	static bool IsFinite(const double& value);
	static float InvSqrt(float p);
	static float Sqrt(float p);
	static float FastSqrt(float value);
	static float FastInvSqrt(float f);
	static float FastestInvSqrt(float f);
	static float SqrtImpl(float f);
	static float Sin(float f);
	static float Pow(float f, float f2);
	static float Cos(float f);
	static float Sign(float f);

#define PI 3.14159265358979323846264338327950288419716939937510F

	static const float BiggestFloatSmallerThanOne;
	static const double BiggestDoubleSmallerThanOne;

};

#endif
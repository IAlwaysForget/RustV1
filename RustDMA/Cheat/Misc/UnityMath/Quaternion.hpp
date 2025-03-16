#ifndef QUATERNION_HPP
#define QUATERNION_HPP

#include "Matrix3x3.hpp"
#include "Matrix4x4.hpp"
#include "Vector3.hpp"
#include "FloatConversion.hpp"

#include <algorithm>
#include <vector>

class Quaternion
{
public:
	float x, y, z, w;

	Quaternion();
	Quaternion(float x, float y, float z, float w);
	explicit Quaternion(const float* array);

	const float* GetPtr()const;
	float* GetPtr();

	const float& operator [] (int i)const;
	float& operator [] (int i);

	void Set(float x, float y, float z, float w);
	void Set(const Quaternion& quat);
	void Set(const float* array);

	bool operator == (const Quaternion& quat)const;
	bool operator != (const Quaternion& quat)const;

	Quaternion& operator += (const Quaternion& quat);
	Quaternion& operator -= (const Quaternion& quat);
	Quaternion& operator *= (const float scalar);
	Quaternion& operator *= (const Quaternion& quat);
	Quaternion	operator * (const float scale) const;
	Quaternion& operator /= (const float scalar);

	Quaternion operator - () const;

	friend Quaternion operator + (const Quaternion& lhs, const Quaternion& rhs);
	friend Quaternion operator - (const Quaternion& lhs, const Quaternion& rhs);
	friend Quaternion operator * (const Quaternion& lhs, const Quaternion& rhs);
	friend Quaternion operator * (const float s, const Quaternion& q);
	friend UnityVector3 operator * (Quaternion rotation, UnityVector3 point);
	friend UnityVector3 operator * (UnityVector3 point, Quaternion rotation);
	friend Quaternion operator / (const Quaternion& quat, const float scale);

	static Quaternion Normalize(const Quaternion& quat);
	static Quaternion NormalizeSafe(const Quaternion& quat);
	static Quaternion Conjugate(const Quaternion& quat);
	static Quaternion Inverse(const Quaternion& quat);
	static float SqrMagnitude(const Quaternion& quat);
	static float Magnitude(const Quaternion& quat);
	static bool CompareApproximately(const Quaternion& q1, const Quaternion& q2, float epsilon = UnityVector3::epsilon);
	static Quaternion Lerp(const Quaternion& q1, const Quaternion& q2, float t);
	static Quaternion Slerp(const Quaternion& q1, const Quaternion& q2, float t);
	static float Dot(const Quaternion& q1, const Quaternion& q2);
	static UnityVector3 QuaternionToEuler(const Quaternion& quat);
	static std::vector<UnityVector3> GetEquivalentEulerAngles(const Quaternion& quat);
	static Quaternion EulerToQuaternion(const UnityVector3& euler);
	static void MatrixToQuaternion(const Matrix3x3& m, Quaternion& q);
	static void MatrixToQuaternion(const Matrix4x4& m, Quaternion& q);
	static void QuaternionToMatrix(const Quaternion& q, Matrix3x3& m);
	static void QuaternionToMatrix(const Quaternion& q, Matrix4x4& m);
	static void QuaternionToAxisAngle(const Quaternion& q, UnityVector3* axis, float* targetAngle);
	static Quaternion AxisAngleToQuaternion(const UnityVector3& axis, float angle);
	static bool LookRotationToQuaternion(const UnityVector3& viewVec, const UnityVector3& upVec, Quaternion* res);
	static UnityVector3 RotateVectorByQuat(const Quaternion& lhs, const UnityVector3& rhs);
	static float AngularDistance(const Quaternion& lhs, const Quaternion& rhs);
	static Quaternion AngularVelocityToQuaternion(const UnityVector3& axis, float deltaTime);
	static Quaternion AxisAngleToQuaternionSafe(const UnityVector3& axis, float angle);
	static Quaternion FromToQuaternionSafe(const UnityVector3& lhs, const UnityVector3& rhs);
	static Quaternion FromToQuaternion(const UnityVector3& from, const UnityVector3& to);
	static Quaternion NormalizeFastEpsilonZero(const Quaternion& q);
	static bool IsFinite(const Quaternion& f);

	static Quaternion Identity();

	static const Quaternion identity;
};

#endif
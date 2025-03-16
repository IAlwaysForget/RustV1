#include "Quaternion.hpp"
#include <limits>

Quaternion::Quaternion()
{
	this->x = 0.f;
	this->y = 0.f;
	this->z = 0.f;
	this->w = 0.f;
}

Quaternion::Quaternion(float x, float y, float z, float w)
{
	this->x = x;
	this->y = y;
	this->z = z;
	this->w = w;
}

Quaternion::Quaternion(const float* array)
{
	this->x = array[0]; 
	this->y = array[1]; 
	this->z = array[2]; 
	this->w = array[3]; 
}

const float* Quaternion::GetPtr()const
{
	return &x;
}

float* Quaternion::GetPtr()
{
	return &x;
}

const float& Quaternion::operator [] (int i)const
{
	return GetPtr()[i];
}

float& Quaternion::operator [] (int i)
{
	return GetPtr()[i];
}

void Quaternion::Set(float inX, float inY, float inZ, float inW)
{
	this->x = inX;
	this->y = inY;
	this->z = inZ;
	this->w = inW;
}

void Quaternion::Set(const Quaternion& aQuat)
{
	this->x = aQuat.x;
	this->y = aQuat.y;
	this->z = aQuat.z;
	this->w = aQuat.w;
}

void Quaternion::Set(const float* array) 
{
	this->x = array[0];
	this->y = array[1];
	this->z = array[2];
	this->w = array[3];
}

bool Quaternion::operator == (const Quaternion& q)const
{ 
	return x == q.x && y == q.y && z == q.z && w == q.w;
}

bool Quaternion::operator != (const Quaternion& q)const
{
	return x != q.x || y != q.y || z != q.z || w != q.w;
}

Quaternion& Quaternion::operator+= (const Quaternion& aQuat)
{
	x += aQuat.x;
	y += aQuat.y;
	z += aQuat.z;
	w += aQuat.w;
	return *this;
}

Quaternion& Quaternion::operator-= (const Quaternion& aQuat)
{
	x -= aQuat.x;
	y -= aQuat.y;
	z -= aQuat.z;
	w -= aQuat.w;
	return *this;
}

Quaternion& Quaternion::operator *= (float aScalar)
{
	x *= aScalar;
	y *= aScalar;
	z *= aScalar;
	w *= aScalar;
	return *this;
}

Quaternion& Quaternion::operator *= (const Quaternion& rhs)
{
	float tempx = w * rhs.x + x * rhs.w + y * rhs.z - z * rhs.y;
	float tempy = w * rhs.y + y * rhs.w + z * rhs.x - x * rhs.z;
	float tempz = w * rhs.z + z * rhs.w + x * rhs.y - y * rhs.x;
	float tempw = w * rhs.w - x * rhs.x - y * rhs.y - z * rhs.z;
	x = tempx; y = tempy; z = tempz; w = tempw;
	return *this;
}

Quaternion Quaternion::operator * (const float scale) const
{
	return Quaternion(x * scale, y * scale, z * scale, w * scale);
}

Quaternion& Quaternion::operator /= (const float aScalar)
{
	x /= aScalar;
	y /= aScalar;
	z /= aScalar;
	w /= aScalar;
	return *this;
}

Quaternion Quaternion::operator - () const
{
	return Quaternion(-x, -y, -z, -w);
}

Quaternion operator + (const Quaternion& lhs, const Quaternion& rhs)
{
	Quaternion q(lhs);
	return q += rhs;
}

Quaternion operator -(const Quaternion& lhs, const Quaternion& rhs)
{
	Quaternion t(lhs);
	return t -= rhs;
}

Quaternion operator * (const Quaternion& lhs, const Quaternion& rhs)
{
	return Quaternion(lhs.w * rhs.x + lhs.x * rhs.w + lhs.y * rhs.z - lhs.z * rhs.y, lhs.w * rhs.y + lhs.y * rhs.w + lhs.z * rhs.x - lhs.x * rhs.z, lhs.w * rhs.z + lhs.z * rhs.w + lhs.x * rhs.y - lhs.y * rhs.x, lhs.w * rhs.w - lhs.x * rhs.x - lhs.y * rhs.y - lhs.z * rhs.z);
}

Quaternion operator * (const float s, const Quaternion& q)
{
	Quaternion t(q);
	return t *= s;
}

UnityVector3 operator * (Quaternion rotation, UnityVector3 point)
{
	float num = rotation.x * 2.f;
	float num2 = rotation.y * 2.f;
	float num3 = rotation.z * 2.f;
	float num4 = rotation.x * num;
	float num5 = rotation.y * num2;
	float num6 = rotation.z * num3;
	float num7 = rotation.x * num2;
	float num8 = rotation.x * num3;
	float num9 = rotation.y * num3;
	float num10 = rotation.w * num;
	float num11 = rotation.w * num2;
	float num12 = rotation.w * num3;
	UnityVector3 result;
	result.x = (1.f - (num5 + num6)) * point.x + (num7 - num12) * point.y + (num8 + num11) * point.z;
	result.y = (num7 + num12) * point.x + (1.f - (num4 + num6)) * point.y + (num9 - num10) * point.z;
	result.z = (num8 - num11) * point.x + (num9 + num10) * point.y + (1.f - (num4 + num5)) * point.z;
	return result;
}

UnityVector3 operator * (UnityVector3 point, Quaternion rotation)
{
	float num = rotation.x * 2.f;
	float num2 = rotation.y * 2.f;
	float num3 = rotation.z * 2.f;
	float num4 = rotation.x * num;
	float num5 = rotation.y * num2;
	float num6 = rotation.z * num3;
	float num7 = rotation.x * num2;
	float num8 = rotation.x * num3;
	float num9 = rotation.y * num3;
	float num10 = rotation.w * num;
	float num11 = rotation.w * num2;
	float num12 = rotation.w * num3;
	UnityVector3 result;
	result.x = (1.f - (num5 + num6)) * point.x + (num7 - num12) * point.y + (num8 + num11) * point.z;
	result.y = (num7 + num12) * point.x + (1.f - (num4 + num6)) * point.y + (num9 - num10) * point.z;
	result.z = (num8 - num11) * point.x + (num9 + num10) * point.y + (1.f - (num4 + num5)) * point.z;
	return result;
}

Quaternion operator / (const Quaternion& q, const float s)
{
	Quaternion t(q);
	return t /= s;
}

Quaternion Quaternion::Normalize(const Quaternion& q)
{
	return q / Magnitude(q);
}

Quaternion Quaternion::NormalizeSafe(const Quaternion& q)
{
	float mag = Magnitude(q);
	if (mag < UnityVector3::epsilon)
		return Quaternion::Identity();
	else
		return q / mag;
}

Quaternion Quaternion::Conjugate(const Quaternion& q)
{
	return Quaternion(-q.x, -q.y, -q.z, q.w);
}

Quaternion Quaternion::Inverse(const Quaternion& q)
{
	Quaternion res = Conjugate(q);
	return res;
}

float Quaternion::SqrMagnitude(const Quaternion& q)
{
	return Dot(q, q);
}

float Quaternion::Magnitude(const Quaternion& q)
{
	return FloatConversion::SqrtImpl(SqrMagnitude(q));
}

bool Quaternion::CompareApproximately(const Quaternion& q1, const Quaternion& q2, float epsilon)
{
	return (SqrMagnitude(q1 - q2) < epsilon * epsilon) || (SqrMagnitude(q1 + q2) < epsilon * epsilon);
}

Quaternion Quaternion::Lerp(const Quaternion& q1, const Quaternion& q2, float t)
{
	Quaternion tmpQuat;
	if (Dot(q1, q2) < 0.0F)
	{
		tmpQuat.Set(q1.x + t * (-q2.x - q1.x),
			q1.y + t * (-q2.y - q1.y),
			q1.z + t * (-q2.z - q1.z),
			q1.w + t * (-q2.w - q1.w));
	}
	else
	{
		tmpQuat.Set(q1.x + t * (q2.x - q1.x),
			q1.y + t * (q2.y - q1.y),
			q1.z + t * (q2.z - q1.z),
			q1.w + t * (q2.w - q1.w));
	}
	return Normalize(tmpQuat);
}

Quaternion Quaternion::Slerp(const Quaternion& q1, const Quaternion& q2, float t)
{
	float dot = Dot(q1, q2);
	Quaternion tmpQuat;
	if (dot < 0.0f)
	{
		dot = -dot;
		tmpQuat.Set(-q2.x, -q2.y, -q2.z, -q2.w);
	}
	else
	{
		tmpQuat = q2;
	}

	if (dot < 0.95f)
	{
		float angle = acos(dot);
		float sinadiv, sinat, sinaomt;
		sinadiv = 1.0f / sin(angle);
		sinat = sin(angle * t);
		sinaomt = sin(angle * (1.0f - t));
		tmpQuat.Set((q1.x * sinaomt + tmpQuat.x * sinat) * sinadiv, (q1.y * sinaomt + tmpQuat.y * sinat) * sinadiv, (q1.z * sinaomt + tmpQuat.z * sinat) * sinadiv, (q1.w * sinaomt + tmpQuat.w * sinat) * sinadiv);
		return tmpQuat;

	}
	else
	{
		return Lerp(q1, tmpQuat, t);
	}
}

float Quaternion::Dot(const Quaternion& q1, const Quaternion& q2)
{
	return (q1.x * q2.x + q1.y * q2.y + q1.z * q2.z + q1.w * q2.w);
}

UnityVector3 Quaternion::QuaternionToEuler(const Quaternion& quat)
{
	Matrix3x3 m;
	UnityVector3 rot;
	QuaternionToMatrix(quat, m);
	Matrix3x3::MatrixToEuler(m, rot);
	return rot;
}

std::vector<UnityVector3> Quaternion::GetEquivalentEulerAngles(const Quaternion& quat)
{
	Matrix3x3 m;
	UnityVector3 rot;

	std::vector<UnityVector3> euler_triples;

	QuaternionToMatrix(quat, m);
	Matrix3x3::MatrixToEuler(m, rot);

	euler_triples.push_back(rot);

	euler_triples.push_back(UnityVector3(rot.x + 180.0f, -rot.y, rot.z + 180.0f));
	euler_triples.push_back(UnityVector3(rot.x - 180.0f, -rot.y, rot.z - 180.0f));
	euler_triples.push_back(UnityVector3(-rot.x, rot.y + 180.0f, -rot.z));
	euler_triples.push_back(UnityVector3(-rot.x, rot.y - 180.0f, -rot.z));

	return euler_triples;
}

Quaternion Quaternion::EulerToQuaternion(const UnityVector3& someEulerAngles)
{
	float cX(cos(someEulerAngles.x / 2.0f));
	float sX(sin(someEulerAngles.x / 2.0f));

	float cY(cos(someEulerAngles.y / 2.0f));
	float sY(sin(someEulerAngles.y / 2.0f));

	float cZ(cos(someEulerAngles.z / 2.0f));
	float sZ(sin(someEulerAngles.z / 2.0f));

	Quaternion qX(sX, 0.0F, 0.0F, cX);
	Quaternion qY(0.0F, sY, 0.0F, cY);
	Quaternion qZ(0.0F, 0.0F, sZ, cZ);

	Quaternion q = (qY * qX) * qZ;
	return q;
}

void Quaternion::MatrixToQuaternion(const Matrix4x4& m, Quaternion& q)
{
	Matrix3x3 mat(m.Get(0, 0), m.Get(0, 1), m.Get(0, 2), m.Get(1, 0), m.Get(1, 1), m.Get(1, 2), m.Get(2, 0), m.Get(2, 1), m.Get(2, 2));
	MatrixToQuaternion(mat, q);
}

void Quaternion::MatrixToQuaternion(const Matrix3x3& kRot, Quaternion& q)
{
	float fTrace = kRot.Get(0, 0) + kRot.Get(1, 1) + kRot.Get(2, 2);
	float fRoot;

	if (fTrace > 0.0f)
	{
		fRoot = sqrt(fTrace + 1.0f);
		q.w = 0.5f * fRoot;
		fRoot = 0.5f / fRoot;
		q.x = (kRot.Get(2, 1) - kRot.Get(1, 2)) * fRoot;
		q.y = (kRot.Get(0, 2) - kRot.Get(2, 0)) * fRoot;
		q.z = (kRot.Get(1, 0) - kRot.Get(0, 1)) * fRoot;
	}
	else
	{
		// |w| <= 1/2
		int s_iNext[3] = { 1, 2, 0 };
		int i = 0;
		if (kRot.Get(1, 1) > kRot.Get(0, 0))
			i = 1;
		if (kRot.Get(2, 2) > kRot.Get(i, i))
			i = 2;
		int j = s_iNext[i];
		int k = s_iNext[j];

		fRoot = sqrt(kRot.Get(i, i) - kRot.Get(j, j) - kRot.Get(k, k) + 1.0f);
		float* apkQuat[3] = { &q.x, &q.y, &q.z };
		*apkQuat[i] = 0.5f * fRoot;
		fRoot = 0.5f / fRoot;
		q.w = (kRot.Get(k, j) - kRot.Get(j, k)) * fRoot;
		*apkQuat[j] = (kRot.Get(j, i) + kRot.Get(i, j)) * fRoot;
		*apkQuat[k] = (kRot.Get(k, i) + kRot.Get(i, k)) * fRoot;
	}
	q = Normalize(q);
}

void Quaternion::QuaternionToMatrix(const Quaternion& q, Matrix3x3& m)
{
	float x = q.x * 2.0F;
	float y = q.y * 2.0F;
	float z = q.z * 2.0F;
	float xx = q.x * x;
	float yy = q.y * y;
	float zz = q.z * z;
	float xy = q.x * y;
	float xz = q.x * z;
	float yz = q.y * z;
	float wx = q.w * x;
	float wy = q.w * y;
	float wz = q.w * z;

	// Calculate 3x3 matrix from orthonormal basis
	m.m_Data[0] = 1.0f - (yy + zz);
	m.m_Data[1] = xy + wz;
	m.m_Data[2] = xz - wy;

	m.m_Data[3] = xy - wz;
	m.m_Data[4] = 1.0f - (xx + zz);
	m.m_Data[5] = yz + wx;

	m.m_Data[6] = xz + wy;
	m.m_Data[7] = yz - wx;
	m.m_Data[8] = 1.0f - (xx + yy);
}


void Quaternion::QuaternionToMatrix(const Quaternion& q, Matrix4x4& m)
{
	float x = q.x * 2.0F;
	float y = q.y * 2.0F;
	float z = q.z * 2.0F;
	float xx = q.x * x;
	float yy = q.y * y;
	float zz = q.z * z;
	float xy = q.x * y;
	float xz = q.x * z;
	float yz = q.y * z;
	float wx = q.w * x;
	float wy = q.w * y;
	float wz = q.w * z;

	// Calculate 3x3 matrix from orthonormal basis
	m.m_Data[0] = 1.0f - (yy + zz);
	m.m_Data[1] = xy + wz;
	m.m_Data[2] = xz - wy;
	m.m_Data[3] = 0.0F;

	m.m_Data[4] = xy - wz;
	m.m_Data[5] = 1.0f - (xx + zz);
	m.m_Data[6] = yz + wx;
	m.m_Data[7] = 0.0F;

	m.m_Data[8] = xz + wy;
	m.m_Data[9] = yz - wx;
	m.m_Data[10] = 1.0f - (xx + yy);
	m.m_Data[11] = 0.0F;

	m.m_Data[12] = 0.0F;
	m.m_Data[13] = 0.0F;
	m.m_Data[14] = 0.0F;
	m.m_Data[15] = 1.0F;
}

void Quaternion::QuaternionToAxisAngle(const Quaternion& q, UnityVector3* axis, float* targetAngle)
{
	*targetAngle = 2.0f * acos(q.w);
	float div = 1.0f / sqrt(1.0f - (q.w * q.w));
	axis->Set(q.x * div, q.y * div, q.z * div);
}

Quaternion Quaternion::AxisAngleToQuaternion(const UnityVector3& axis, float angle)
{
	Quaternion q;
	float halfAngle = angle * 0.5F;
	float s = sin(halfAngle);

	q.w = cos(halfAngle);
	q.x = s * axis.x;
	q.y = s * axis.y;
	q.z = s * axis.z;
	return q;
}

bool Quaternion::LookRotationToQuaternion(const UnityVector3& viewVec, const UnityVector3& upVec, Quaternion* res)
{
	Matrix3x3 m;
	if (!Matrix3x3::LookRotationToMatrix(viewVec, upVec, &m))
		return false;
	MatrixToQuaternion(m, *res);
	return true;
}

UnityVector3 Quaternion::RotateVectorByQuat(const Quaternion& lhs, const UnityVector3& rhs)
{
	float x = lhs.x * 2.0F;
	float y = lhs.y * 2.0F;
	float z = lhs.z * 2.0F;
	float xx = lhs.x * x;
	float yy = lhs.y * y;
	float zz = lhs.z * z;
	float xy = lhs.x * y;
	float xz = lhs.x * z;
	float yz = lhs.y * z;
	float wx = lhs.w * x;
	float wy = lhs.w * y;
	float wz = lhs.w * z;

	UnityVector3 res;
	res.x = (1.0f - (yy + zz)) * rhs.x + (xy - wz) * rhs.y + (xz + wy) * rhs.z;
	res.y = (xy + wz) * rhs.x + (1.0f - (xx + zz)) * rhs.y + (yz - wx) * rhs.z;
	res.z = (xz - wy) * rhs.x + (yz + wx) * rhs.y + (1.0f - (xx + yy)) * rhs.z;

	return res;
}

float Quaternion::AngularDistance(const Quaternion& lhs, const Quaternion& rhs)
{
	float dot = Dot(lhs, rhs);
	if (dot < 0.0f)
		dot = -dot;
	return acos(std::min(1.0F, dot)) * 2.0F;
}

Quaternion Quaternion::AngularVelocityToQuaternion(const UnityVector3& axis, float deltaTime)
{
	float w = UnityVector3::Magnitude(axis);
	if (w > UnityVector3::epsilon)
	{
		float v = deltaTime * w * 0.5f;
		float q = cos(v);
		float s = sin(v) / w;

		Quaternion integrated;
		integrated.w = q;
		integrated.x = s * axis.x;
		integrated.y = s * axis.y;
		integrated.z = s * axis.z;

		return NormalizeSafe(integrated);
	}
	else
	{
		return Quaternion::Identity();
	}
}

Quaternion Quaternion::AxisAngleToQuaternionSafe(const UnityVector3& axis, float angle)
{
	Quaternion q;
	float mag = UnityVector3::Magnitude(axis);
	if (mag > 0.000001F)
	{
		float halfAngle = angle * 0.5F;

		q.w = cos(halfAngle);

		float s = sin(halfAngle) / mag;
		q.x = s * axis.x;
		q.y = s * axis.y;
		q.z = s * axis.z;
		return q;
	}
	else
	{
		return Quaternion::Identity();
	}
}

Quaternion Quaternion::FromToQuaternionSafe(const UnityVector3& lhs, const UnityVector3& rhs)
{
	float lhsMag = UnityVector3::Magnitude(lhs);
	float rhsMag = UnityVector3::Magnitude(rhs);
	if (lhsMag < UnityVector3::epsilon || rhsMag < UnityVector3::epsilon)
		return Quaternion::Identity();
	else
		return FromToQuaternion(lhs / lhsMag, rhs / rhsMag);
}

Quaternion Quaternion::FromToQuaternion(const UnityVector3& from, const UnityVector3& to)
{
	Matrix3x3 m;
	m.SetFromToRotation(from, to);
	Quaternion q;
	MatrixToQuaternion(m, q);
	return q;
}

Quaternion Quaternion::NormalizeFastEpsilonZero(const Quaternion& q)
{
	float m = SqrMagnitude(q);
	if (m < UnityVector3::epsilon)
		return Quaternion(0.0F, 0.0F, 0.0F, 0.0F);
	else
		return q * FloatConversion::FastInvSqrt(m);
}

bool Quaternion::IsFinite(const Quaternion& f)
{
	return FloatConversion::IsFinite(f.x) & FloatConversion::IsFinite(f.y) & FloatConversion::IsFinite(f.z) & FloatConversion::IsFinite(f.w);
}

Quaternion Quaternion::Identity()
{
	return identity;
}

const Quaternion Quaternion::identity = Quaternion(0.0F, 0.0F, 0.0F, 1.0F);
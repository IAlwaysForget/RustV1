#include "Matrix4x4.hpp"
#include "Matrix3x3.hpp"
#include "Quaternion.hpp"

Matrix4x4::Matrix4x4()
{
	this->m_Data[0] = 0.f;
	this->m_Data[1] = 0.f;
	this->m_Data[2] = 0.f;
	this->m_Data[3] = 0.f;
	this->m_Data[4] = 0.f;
	this->m_Data[5] = 0.f;
	this->m_Data[6] = 0.f;
	this->m_Data[7] = 0.f;
	this->m_Data[8] = 0.f;
	this->m_Data[9] = 0.f;
	this->m_Data[10] = 0.f;
	this->m_Data[11] = 0.f;
	this->m_Data[12] = 0.f;
	this->m_Data[13] = 0.f;
	this->m_Data[14] = 0.f;
}

Matrix4x4::Matrix4x4(const Matrix3x3& other)
{
	this->m_Data[0] = other.m_Data[0];
	this->m_Data[1] = other.m_Data[1];
	this->m_Data[2] = other.m_Data[2];
	this->m_Data[3] = 0.0F;

	this->m_Data[4] = other.m_Data[3];
	this->m_Data[5] = other.m_Data[4];
	this->m_Data[6] = other.m_Data[5];
	this->m_Data[7] = 0.0F;

	this->m_Data[8] = other.m_Data[6];
	this->m_Data[9] = other.m_Data[7];
	this->m_Data[10] = other.m_Data[8];
	this->m_Data[11] = 0.0F;

	this->m_Data[12] = 0.0F;
	this->m_Data[13] = 0.0F;
	this->m_Data[14] = 0.0F;
	this->m_Data[15] = 1.0F;
}

Matrix4x4::Matrix4x4(const float data[16])
{
	for (int i = 0; i < 16; i++)
	{
		m_Data[i] = data[i];
	}
}

float& Matrix4x4::Get(int row, int column) 
{
	return m_Data[row + (column * 4)];
}

const float& Matrix4x4::Get(int row, int column)const 
{ 
	return m_Data[row + (column * 4)]; 
}

float* Matrix4x4::GetPtr() 
{
	return m_Data;
}

const float* Matrix4x4::GetPtr()const 
{ 
	return m_Data;
}

float Matrix4x4::operator [] (int index) const
{
	return m_Data[index];
}

float& Matrix4x4::operator [] (int index) 
{ 
	return m_Data[index];
}

Matrix4x4& Matrix4x4::operator=(const Matrix3x3& other)
{
	this->m_Data[0] = other.m_Data[0];
	this->m_Data[1] = other.m_Data[1];
	this->m_Data[2] = other.m_Data[2];
	this->m_Data[3] = 0.0F;

	this->m_Data[4] = other.m_Data[3];
	this->m_Data[5] = other.m_Data[4];
	this->m_Data[6] = other.m_Data[5];
	this->m_Data[7] = 0.0F;

	this->m_Data[8] = other.m_Data[6];
	this->m_Data[9] = other.m_Data[7];
	this->m_Data[10] = other.m_Data[8];
	this->m_Data[11] = 0.0F;

	this->m_Data[12] = 0.0F;
	this->m_Data[13] = 0.0F;
	this->m_Data[14] = 0.0F;
	this->m_Data[15] = 1.0F;
	return *this;
}

Matrix4x4& Matrix4x4::operator *= (const Matrix4x4& inM1)
{
	Matrix4x4 tmp;
	MultiplyMatrices4x4(this, &inM1, &tmp);
	*this = tmp;
	return *this;
}

bool Matrix4x4::IsIdentity(float threshold) const
{
	if (FloatConversion::CompareApproximately(Get(0, 0), 1.0f, threshold) && FloatConversion::CompareApproximately(Get(0, 1), 0.0f, threshold) && FloatConversion::CompareApproximately(Get(0, 2), 0.0f, threshold) && FloatConversion::CompareApproximately(Get(0, 3), 0.0f, threshold) &&
		FloatConversion::CompareApproximately(Get(1, 0), 0.0f, threshold) && FloatConversion::CompareApproximately(Get(1, 1), 1.0f, threshold) && FloatConversion::CompareApproximately(Get(1, 2), 0.0f, threshold) && FloatConversion::CompareApproximately(Get(1, 3), 0.0f, threshold) &&
		FloatConversion::CompareApproximately(Get(2, 0), 0.0f, threshold) && FloatConversion::CompareApproximately(Get(2, 1), 0.0f, threshold) && FloatConversion::CompareApproximately(Get(2, 2), 1.0f, threshold) && FloatConversion::CompareApproximately(Get(2, 3), 0.0f, threshold) &&
		FloatConversion::CompareApproximately(Get(3, 0), 0.0f, threshold) && FloatConversion::CompareApproximately(Get(3, 1), 0.0f, threshold) && FloatConversion::CompareApproximately(Get(3, 2), 0.0f, threshold) && FloatConversion::CompareApproximately(Get(3, 3), 1.0f, threshold))
		return true;
	return false;
}

double Matrix4x4::GetDeterminant() const
{
	double m00 = Get(0, 0);  double m01 = Get(0, 1);  double m02 = Get(0, 2);  double m03 = Get(0, 3);
	double m10 = Get(1, 0);  double m11 = Get(1, 1);  double m12 = Get(1, 2);  double m13 = Get(1, 3);
	double m20 = Get(2, 0);  double m21 = Get(2, 1);  double m22 = Get(2, 2);  double m23 = Get(2, 3);
	double m30 = Get(3, 0);  double m31 = Get(3, 1);  double m32 = Get(3, 2);  double m33 = Get(3, 3);

	double result =
		m03 * m12 * m21 * m30 - m02 * m13 * m21 * m30 - m03 * m11 * m22 * m30 + m01 * m13 * m22 * m30 +
		m02 * m11 * m23 * m30 - m01 * m12 * m23 * m30 - m03 * m12 * m20 * m31 + m02 * m13 * m20 * m31 +
		m03 * m10 * m22 * m31 - m00 * m13 * m22 * m31 - m02 * m10 * m23 * m31 + m00 * m12 * m23 * m31 +
		m03 * m11 * m20 * m32 - m01 * m13 * m20 * m32 - m03 * m10 * m21 * m32 + m00 * m13 * m21 * m32 +
		m01 * m10 * m23 * m32 - m00 * m11 * m23 * m32 - m02 * m11 * m20 * m33 + m01 * m12 * m20 * m33 +
		m02 * m10 * m21 * m33 - m00 * m12 * m21 * m33 - m01 * m10 * m22 * m33 + m00 * m11 * m22 * m33;
	return result;
}

Matrix4x4& Matrix4x4::Invert_Full()
{
	InvertMatrix4x4_Full(m_Data, m_Data);
	return *this;
}

Matrix4x4& Matrix4x4::Transpose()
{
	std::swap(Get(0, 1), Get(1, 0));
	std::swap(Get(0, 2), Get(2, 0));
	std::swap(Get(0, 3), Get(3, 0));
	std::swap(Get(1, 2), Get(2, 1));
	std::swap(Get(1, 3), Get(3, 1));
	std::swap(Get(2, 3), Get(3, 2));
	return *this;
}

Matrix4x4& Matrix4x4::Copy(const Matrix4x4& inM)
{
	CopyMatrix(inM.m_Data, m_Data);
	return *this;
}

Matrix4x4& Matrix4x4::SetIdentity()
{
	Get(0, 0) = 1.0;	Get(0, 1) = 0.0;	Get(0, 2) = 0.0;	Get(0, 3) = 0.0;
	Get(1, 0) = 0.0;	Get(1, 1) = 1.0;	Get(1, 2) = 0.0;	Get(1, 3) = 0.0;
	Get(2, 0) = 0.0;	Get(2, 1) = 0.0;	Get(2, 2) = 1.0;	Get(2, 3) = 0.0;
	Get(3, 0) = 0.0;	Get(3, 1) = 0.0;	Get(3, 2) = 0.0;	Get(3, 3) = 1.0;
	return *this;
}

Matrix4x4& Matrix4x4::SetPerspective(float fovy, float aspect, float zNear, float zFar)
{
	float cotangent, deltaZ;
	float radians = FloatConversion::Deg2Rad(fovy / 2.0f);
	cotangent = cos(radians) / sin(radians);
	deltaZ = zNear - zFar;

	Get(0, 0) = cotangent / aspect;	Get(0, 1) = 0.0F;      Get(0, 2) = 0.0F;                    Get(0, 3) = 0.0F;
	Get(1, 0) = 0.0F;               Get(1, 1) = cotangent; Get(1, 2) = 0.0F;                    Get(1, 3) = 0.0F;
	Get(2, 0) = 0.0F;               Get(2, 1) = 0.0F;      Get(2, 2) = (zFar + zNear) / deltaZ; Get(2, 3) = 2.0F * zNear * zFar / deltaZ;
	Get(3, 0) = 0.0F;               Get(3, 1) = 0.0F;      Get(3, 2) = -1.0F;                   Get(3, 3) = 0.0F;

	return *this;
}

Matrix4x4& Matrix4x4::SetPerspectiveCotan(float cotangent, float zNear, float zFar)
{
	float deltaZ = zNear - zFar;

	Get(0, 0) = cotangent;			Get(0, 1) = 0.0F;      Get(0, 2) = 0.0F;                    Get(0, 3) = 0.0F;
	Get(1, 0) = 0.0F;               Get(1, 1) = cotangent; Get(1, 2) = 0.0F;                    Get(1, 3) = 0.0F;
	Get(2, 0) = 0.0F;               Get(2, 1) = 0.0F;      Get(2, 2) = (zFar + zNear) / deltaZ; Get(2, 3) = 2.0F * zNear * zFar / deltaZ;
	Get(3, 0) = 0.0F;               Get(3, 1) = 0.0F;      Get(3, 2) = -1.0F;                   Get(3, 3) = 0.0F;

	return *this;
}

Matrix4x4& Matrix4x4::SetOrtho(float left, float right, float bottom, float top, float zNear, float zFar)
{
	SetIdentity();

	float deltax = right - left;
	float deltay = top - bottom;
	float deltaz = zFar - zNear;

	Get(0, 0) = 2.0F / deltax;
	Get(0, 3) = -(right + left) / deltax;
	Get(1, 1) = 2.0F / deltay;
	Get(1, 3) = -(top + bottom) / deltay;
	Get(2, 2) = -2.0F / deltaz;
	Get(2, 3) = -(zFar + zNear) / deltaz;
	return *this;
}

Matrix4x4& Matrix4x4::SetFrustum(float left, float right, float bottom, float top, float nearval, float farval)
{
	float x, y, a, b, c, d, e;

	x = (2.0F * nearval) / (right - left);
	y = (2.0F * nearval) / (top - bottom);
	a = (right + left) / (right - left);
	b = (top + bottom) / (top - bottom);
	c = -(farval + nearval) / (farval - nearval);
	d = -(2.0f * farval * nearval) / (farval - nearval);
	e = -1.0f;

	Get(0, 0) = x;    Get(0, 1) = 0.0;  Get(0, 2) = a;   Get(0, 3) = 0.0;
	Get(1, 0) = 0.0;  Get(1, 1) = y;    Get(1, 2) = b;   Get(1, 3) = 0.0;
	Get(2, 0) = 0.0;  Get(2, 1) = 0.0;  Get(2, 2) = c;   Get(2, 3) = d;
	Get(3, 0) = 0.0;  Get(3, 1) = 0.0;  Get(3, 2) = e;	Get(3, 3) = 0.0;
	return *this;
}

UnityVector3 Matrix4x4::GetAxisX() const 
{
	return UnityVector3(Get(0, 0), Get(1, 0), Get(2, 0));
}

UnityVector3 Matrix4x4::GetAxisY() const 
{
	return UnityVector3(Get(0, 1), Get(1, 1), Get(2, 1));
}

UnityVector3 Matrix4x4::GetAxisZ() const 
{
	return UnityVector3(Get(0, 2), Get(1, 2), Get(2, 2));
}

UnityVector3 Matrix4x4::GetPosition() const 
{
	return UnityVector3(Get(0, 3), Get(1, 3), Get(2, 3));
}

UnityVector4 Matrix4x4::GetRow(int row) const 
{
	return UnityVector4(Get(row, 0), Get(row, 1), Get(row, 2), Get(row, 3));
}

UnityVector4 Matrix4x4::GetColumn(int col) const 
{
	return UnityVector4(Get(0, col), Get(1, col), Get(2, col), Get(3, col));
}

void Matrix4x4::SetAxisX(const UnityVector3& v) 
{
	Get(0, 0) = v.x; Get(1, 0) = v.y; Get(2, 0) = v.z;
}

void Matrix4x4::SetAxisY(const UnityVector3& v) 
{
	Get(0, 1) = v.x; Get(1, 1) = v.y; Get(2, 1) = v.z;
}

void Matrix4x4::SetAxisZ(const UnityVector3& v) 
{
	Get(0, 2) = v.x; Get(1, 2) = v.y; Get(2, 2) = v.z;
}

void Matrix4x4::SetPosition(const UnityVector3& v) 
{
	Get(0, 3) = v.x; Get(1, 3) = v.y; Get(2, 3) = v.z;
}

void Matrix4x4::SetRow(int row, const UnityVector4& v) 
{
	Get(row, 0) = v.x; Get(row, 1) = v.y; Get(row, 2) = v.z; Get(row, 3) = v.w;
}

void Matrix4x4::SetColumn(int col, const UnityVector4& v)
{
	Get(0, col) = v.x; Get(1, col) = v.y; Get(2, col) = v.z; Get(3, col) = v.w;
}

Matrix4x4& Matrix4x4::SetTranslate(const UnityVector3& inTrans)
{
	Get(0, 0) = 1.0;	Get(0, 1) = 0.0;	Get(0, 2) = 0.0;	Get(0, 3) = inTrans[0];
	Get(1, 0) = 0.0;	Get(1, 1) = 1.0;	Get(1, 2) = 0.0;	Get(1, 3) = inTrans[1];
	Get(2, 0) = 0.0;	Get(2, 1) = 0.0;	Get(2, 2) = 1.0;	Get(2, 3) = inTrans[2];
	Get(3, 0) = 0.0;	Get(3, 1) = 0.0;	Get(3, 2) = 0.0;	Get(3, 3) = 1.0;
	return *this;
}

Matrix4x4& Matrix4x4::SetOrthoNormalBasis(const UnityVector3& inX, const UnityVector3& inY, const UnityVector3& inZ)
{
	Get(0, 0) = inX[0];	Get(0, 1) = inY[0];	Get(0, 2) = inZ[0];	Get(0, 3) = 0.0;
	Get(1, 0) = inX[1];	Get(1, 1) = inY[1];	Get(1, 2) = inZ[1];	Get(1, 3) = 0.0;
	Get(2, 0) = inX[2];	Get(2, 1) = inY[2];	Get(2, 2) = inZ[2];	Get(2, 3) = 0.0;
	Get(3, 0) = 0.0;		Get(3, 1) = 0.0;		Get(3, 2) = 0.0;		Get(3, 3) = 1.0;
	return *this;
}

Matrix4x4& Matrix4x4::SetOrthoNormalBasisInverse(const UnityVector3& inX, const UnityVector3& inY, const UnityVector3& inZ)
{
	Get(0, 0) = inX[0];	Get(1, 0) = inY[0];	Get(2, 0) = inZ[0];	Get(3, 0) = 0.0;
	Get(0, 1) = inX[1];	Get(1, 1) = inY[1];	Get(2, 1) = inZ[1];	Get(3, 1) = 0.0;
	Get(0, 2) = inX[2];	Get(1, 2) = inY[2];	Get(2, 2) = inZ[2];	Get(3, 2) = 0.0;
	Get(0, 3) = 0.0;		Get(1, 3) = 0.0;		Get(2, 3) = 0.0;		Get(3, 3) = 1.0;
	return *this;
}

Matrix4x4& Matrix4x4::SetScale(const UnityVector3& inScale)
{
	Get(0, 0) = inScale[0];	Get(0, 1) = 0.0;			Get(0, 2) = 0.0;			Get(0, 3) = 0.0;
	Get(1, 0) = 0.0;			Get(1, 1) = inScale[1];	Get(1, 2) = 0.0;			Get(1, 3) = 0.0;
	Get(2, 0) = 0.0;			Get(2, 1) = 0.0;			Get(2, 2) = inScale[2];	Get(2, 3) = 0.0;
	Get(3, 0) = 0.0;			Get(3, 1) = 0.0;			Get(3, 2) = 0.0;			Get(3, 3) = 1.0;
	return *this;
}

Matrix4x4& Matrix4x4::SetPositionAndOrthoNormalBasis(const UnityVector3& inPosition, const UnityVector3& inX, const UnityVector3& inY, const UnityVector3& inZ)
{
	Get(0, 0) = inX[0];	Get(0, 1) = inY[0];	Get(0, 2) = inZ[0];	Get(0, 3) = inPosition[0];
	Get(1, 0) = inX[1];	Get(1, 1) = inY[1];	Get(1, 2) = inZ[1];	Get(1, 3) = inPosition[1];
	Get(2, 0) = inX[2];	Get(2, 1) = inY[2];	Get(2, 2) = inZ[2];	Get(2, 3) = inPosition[2];
	Get(3, 0) = 0.0;		Get(3, 1) = 0.0;		Get(3, 2) = 0.0;		Get(3, 3) = 1.0;
	return *this;
}

Matrix4x4& Matrix4x4::Translate(const UnityVector3& inTrans)
{
	Get(0, 3) = Get(0, 0) * inTrans[0] + Get(0, 1) * inTrans[1] + Get(0, 2) * inTrans[2] + Get(0, 3);
	Get(1, 3) = Get(1, 0) * inTrans[0] + Get(1, 1) * inTrans[1] + Get(1, 2) * inTrans[2] + Get(1, 3);
	Get(2, 3) = Get(2, 0) * inTrans[0] + Get(2, 1) * inTrans[1] + Get(2, 2) * inTrans[2] + Get(2, 3);
	Get(3, 3) = Get(3, 0) * inTrans[0] + Get(3, 1) * inTrans[1] + Get(3, 2) * inTrans[2] + Get(3, 3);
	return *this;
}

Matrix4x4& Matrix4x4::Scale(const UnityVector3& inScale)
{
	Get(0, 0) *= inScale[0];
	Get(1, 0) *= inScale[0];
	Get(2, 0) *= inScale[0];
	Get(3, 0) *= inScale[0];

	Get(0, 1) *= inScale[1];
	Get(1, 1) *= inScale[1];
	Get(2, 1) *= inScale[1];
	Get(3, 1) *= inScale[1];

	Get(0, 2) *= inScale[2];
	Get(1, 2) *= inScale[2];
	Get(2, 2) *= inScale[2];
	Get(3, 2) *= inScale[2];
	return *this;
}

Matrix4x4& Matrix4x4::SetFromToRotation(const UnityVector3& from, const UnityVector3& to)
{
	float mtx[3][3];
	Matrix3x3::FromToRotation(from.GetPtr(), to.GetPtr(), mtx);
	Get(0, 0) = mtx[0][0];	Get(0, 1) = mtx[0][1];	Get(0, 2) = mtx[0][2];	Get(0, 3) = 0.0;
	Get(1, 0) = mtx[1][0];	Get(1, 1) = mtx[1][1];	Get(1, 2) = mtx[1][2];	Get(1, 3) = 0.0;
	Get(2, 0) = mtx[2][0];	Get(2, 1) = mtx[2][1];	Get(2, 2) = mtx[2][2];	Get(2, 3) = 0.0;
	Get(3, 0) = 0.0;		Get(3, 1) = 0.0;		Get(3, 2) = 0.0;		Get(3, 3) = 1.0;
	return *this;
}

void Matrix4x4::SetTR(const UnityVector3& pos, const Quaternion& q)
{
	Quaternion::QuaternionToMatrix(q, *this);
	m_Data[12] = pos[0];
	m_Data[13] = pos[1];
	m_Data[14] = pos[2];
}

void Matrix4x4::SetTRS(const UnityVector3& pos, const Quaternion& q, const UnityVector3& s)
{
	Quaternion::QuaternionToMatrix(q, *this);

	m_Data[0] *= s[0];
	m_Data[1] *= s[0];
	m_Data[2] *= s[0];

	m_Data[4] *= s[1];
	m_Data[5] *= s[1];
	m_Data[6] *= s[1];

	m_Data[8] *= s[2];
	m_Data[9] *= s[2];
	m_Data[10] *= s[2];

	m_Data[12] = pos[0];
	m_Data[13] = pos[1];
	m_Data[14] = pos[2];
}

void Matrix4x4::SetTRInverse(const UnityVector3& pos, const Quaternion& q)
{
	Quaternion::QuaternionToMatrix(Quaternion::Inverse(q), *this);
	Translate(UnityVector3(-pos[0], -pos[1], -pos[2]));
}

UnityVector3 Matrix4x4::MultiplyPoint3(const UnityVector3& v) const
{
	UnityVector3 res;
	res.x = m_Data[0] * v.x + m_Data[4] * v.y + m_Data[8] * v.z + m_Data[12];
	res.y = m_Data[1] * v.x + m_Data[5] * v.y + m_Data[9] * v.z + m_Data[13];
	res.z = m_Data[2] * v.x + m_Data[6] * v.y + m_Data[10] * v.z + m_Data[14];
	return res;
}

void Matrix4x4::MultiplyPoint3(const UnityVector3& v, UnityVector3& output) const
{
	output.x = m_Data[0] * v.x + m_Data[4] * v.y + m_Data[8] * v.z + m_Data[12];
	output.y = m_Data[1] * v.x + m_Data[5] * v.y + m_Data[9] * v.z + m_Data[13];
	output.z = m_Data[2] * v.x + m_Data[6] * v.y + m_Data[10] * v.z + m_Data[14];
}


UnityVector3 Matrix4x4::MultiplyUnityVector3(const UnityVector3& v) const
{
	UnityVector3 res;
	res.x = m_Data[0] * v.x + m_Data[4] * v.y + m_Data[8] * v.z;
	res.y = m_Data[1] * v.x + m_Data[5] * v.y + m_Data[9] * v.z;
	res.z = m_Data[2] * v.x + m_Data[6] * v.y + m_Data[10] * v.z;
	return res;
}

void Matrix4x4::MultiplyUnityVector3(const UnityVector3& v, UnityVector3& output) const
{
	output.x = m_Data[0] * v.x + m_Data[4] * v.y + m_Data[8] * v.z;
	output.y = m_Data[1] * v.x + m_Data[5] * v.y + m_Data[9] * v.z;
	output.z = m_Data[2] * v.x + m_Data[6] * v.y + m_Data[10] * v.z;
}


inline bool Matrix4x4::PerspectiveMultiplyPoint3(const UnityVector3& v, UnityVector3& output) const
{
	UnityVector3 res;
	float w;
	res.x = Get(0, 0) * v.x + Get(0, 1) * v.y + Get(0, 2) * v.z + Get(0, 3);
	res.y = Get(1, 0) * v.x + Get(1, 1) * v.y + Get(1, 2) * v.z + Get(1, 3);
	res.z = Get(2, 0) * v.x + Get(2, 1) * v.y + Get(2, 2) * v.z + Get(2, 3);
	w = Get(3, 0) * v.x + Get(3, 1) * v.y + Get(3, 2) * v.z + Get(3, 3);
	if (FloatConversion::Abs(w) > 1.0e-7f)
	{
		float invW = 1.0f / w;
		output.x = res.x * invW;
		output.y = res.y * invW;
		output.z = res.z * invW;
		return true;
	}
	else
	{
		output.x = 0.0f;
		output.y = 0.0f;
		output.z = 0.0f;
		return false;
	}
}

inline bool Matrix4x4::PerspectiveMultiplyUnityVector3(const UnityVector3& v, UnityVector3& output) const
{
	UnityVector3 res;
	float w;
	res.x = Get(0, 0) * v.x + Get(0, 1) * v.y + Get(0, 2) * v.z;
	res.y = Get(1, 0) * v.x + Get(1, 1) * v.y + Get(1, 2) * v.z;
	res.z = Get(2, 0) * v.x + Get(2, 1) * v.y + Get(2, 2) * v.z;
	w = Get(3, 0) * v.x + Get(3, 1) * v.y + Get(3, 2) * v.z;
	if (FloatConversion::Abs(w) > 1.0e-7f)
	{
		float invW = 1.0f / w;
		output.x = res.x * invW;
		output.y = res.y * invW;
		output.z = res.z * invW;
		return true;
	}
	else
	{
		output.x = 0.0f;
		output.y = 0.0f;
		output.z = 0.0f;
		return false;
	}
}

inline UnityVector3 Matrix4x4::InverseMultiplyPoint3Affine(const UnityVector3& inV) const
{
	UnityVector3 v(inV.x - Get(0, 3), inV.y - Get(1, 3), inV.z - Get(2, 3));
	UnityVector3 res;
	res.x = Get(0, 0) * v.x + Get(1, 0) * v.y + Get(2, 0) * v.z;
	res.y = Get(0, 1) * v.x + Get(1, 1) * v.y + Get(2, 1) * v.z;
	res.z = Get(0, 2) * v.x + Get(1, 2) * v.y + Get(2, 2) * v.z;
	return res;
}

inline UnityVector3 Matrix4x4::InverseMultiplyUnityVector3Affine(const UnityVector3& v) const
{
	UnityVector3 res;
	res.x = Get(0, 0) * v.x + Get(1, 0) * v.y + Get(2, 0) * v.z;
	res.y = Get(0, 1) * v.x + Get(1, 1) * v.y + Get(2, 1) * v.z;
	res.z = Get(0, 2) * v.x + Get(1, 2) * v.y + Get(2, 2) * v.z;
	return res;
}


bool Matrix4x4::IsFinite(const Matrix4x4& f)
{
	return FloatConversion::IsFinite(f.m_Data[0]) & FloatConversion::IsFinite(f.m_Data[1]) & FloatConversion::IsFinite(f.m_Data[2]) & FloatConversion::IsFinite(f.m_Data[4]) & FloatConversion::IsFinite(f.m_Data[5]) & FloatConversion::IsFinite(f.m_Data[6]) & FloatConversion::IsFinite(f.m_Data[8]) & FloatConversion::IsFinite(f.m_Data[9]) & FloatConversion::IsFinite(f.m_Data[10]) & FloatConversion::IsFinite(f.m_Data[12]) & FloatConversion::IsFinite(f.m_Data[13]) & FloatConversion::IsFinite(f.m_Data[14]) & FloatConversion::IsFinite(f.m_Data[15]);
}

void  Matrix4x4::CopyMatrix(const float* __restrict lhs, float* __restrict res)
{
	::memcpy(res, lhs, sizeof(Matrix4x4));
}

void  Matrix4x4::TransposeMatrix4x4(const Matrix4x4* __restrict lhs, Matrix4x4* __restrict res)
{
	CopyMatrix(lhs->m_Data, res->m_Data);
	std::swap(res->Get(0, 1), res->Get(1, 0));
	std::swap(res->Get(0, 2), res->Get(2, 0));
	std::swap(res->Get(0, 3), res->Get(3, 0));
	std::swap(res->Get(1, 2), res->Get(2, 1));
	std::swap(res->Get(1, 3), res->Get(3, 1));
	std::swap(res->Get(2, 3), res->Get(3, 2));
}

void  Matrix4x4::MultiplyMatrices4x4(const Matrix4x4* __restrict lhs, const Matrix4x4* __restrict rhs, Matrix4x4* __restrict res)
{
	for (int i = 0; i < 4; i++)
	{
		res->m_Data[i] = lhs->m_Data[i] * rhs->m_Data[0] + lhs->m_Data[i + 4] * rhs->m_Data[1] + lhs->m_Data[i + 8] * rhs->m_Data[2] + lhs->m_Data[i + 12] * rhs->m_Data[3];
		res->m_Data[i + 4] = lhs->m_Data[i] * rhs->m_Data[4] + lhs->m_Data[i + 4] * rhs->m_Data[5] + lhs->m_Data[i + 8] * rhs->m_Data[6] + lhs->m_Data[i + 12] * rhs->m_Data[7];
		res->m_Data[i + 8] = lhs->m_Data[i] * rhs->m_Data[8] + lhs->m_Data[i + 4] * rhs->m_Data[9] + lhs->m_Data[i + 8] * rhs->m_Data[10] + lhs->m_Data[i + 12] * rhs->m_Data[11];
		res->m_Data[i + 12] = lhs->m_Data[i] * rhs->m_Data[12] + lhs->m_Data[i + 4] * rhs->m_Data[13] + lhs->m_Data[i + 8] * rhs->m_Data[14] + lhs->m_Data[i + 12] * rhs->m_Data[15];
	}
}

void  Matrix4x4::MultiplyMatrixArray4x4(const Matrix4x4* __restrict a, const Matrix4x4* __restrict b, Matrix4x4* __restrict res, size_t count)
{
	for (size_t i = 0; i < count; ++i)
	{
		MultiplyMatrices4x4(a + i, b + i, res + i);
	}
}

void Matrix4x4::MultiplyMatrixArrayWithBase4x4(const Matrix4x4* __restrict base, const Matrix4x4* __restrict a, const Matrix4x4* __restrict b, Matrix4x4* __restrict res, size_t count)
{
	Matrix4x4 tmp;
	for (size_t i = 0; i < count; ++i)
	{
		MultiplyMatrices4x4(base, a + i, &tmp);
		MultiplyMatrices4x4(&tmp, b + i, res + i);
	}
}

bool Matrix4x4::CompareApproximately(const Matrix4x4& lhs, const Matrix4x4& rhs, float dist)
{
	for (int i = 0; i < 16; i++)
	{
		if (!FloatConversion::CompareApproximately(lhs[i], rhs[i], dist))
			return false;
	}
	return true;
}

void Matrix4x4::TransformPoints3x3(const Matrix4x4& matrix, const UnityVector3* in, UnityVector3* out, int count)
{
	Matrix3x3 m = Matrix3x3(matrix);
	for (int i = 0; i < count; i++)
		out[i] = m.MultiplyPoint3(in[i]);
}

void Matrix4x4::TransformPoints3x4(const Matrix4x4& matrix, const UnityVector3* in, UnityVector3* out, int count)
{
	for (int i = 0; i < count; i++)
		out[i] = matrix.MultiplyPoint3(in[i]);
}

void Matrix4x4::TransformPoints3x3(const Matrix4x4& matrix, const UnityVector3* in, size_t inStride, UnityVector3* out, size_t outStride, int count)
{
	Matrix3x3 m = Matrix3x3(matrix);
	for (int i = 0; i < count; ++i, in = reinterpret_cast<UnityVector3*>((char*)in + inStride), out = reinterpret_cast<UnityVector3*>((char*)out + outStride))
	{
		*out = m.MultiplyPoint3(*in);
	}
}

void Matrix4x4::TransformPoints3x4(const Matrix4x4& matrix, const UnityVector3* in, size_t inStride, UnityVector3* out, size_t outStride, int count)
{
	for (int i = 0; i < count; ++i, in = reinterpret_cast<UnityVector3*>((char*)in + inStride), out = reinterpret_cast<UnityVector3*>((char*)out + outStride))
	{
		*out = matrix.MultiplyPoint3(*in);
	}
}

void Matrix4x4::MultiplyMatrices3x4(const Matrix4x4& lhs, const Matrix4x4& rhs, Matrix4x4& res)
{
	for (int i = 0; i < 3; i++)
	{
		res.m_Data[i] = lhs.m_Data[i] * rhs.m_Data[0] + lhs.m_Data[i + 4] * rhs.m_Data[1] + lhs.m_Data[i + 8] * rhs.m_Data[2];//  + lhs.m_Data[i+12] * rhs.m_Data[3];
		res.m_Data[i + 4] = lhs.m_Data[i] * rhs.m_Data[4] + lhs.m_Data[i + 4] * rhs.m_Data[5] + lhs.m_Data[i + 8] * rhs.m_Data[6];//  + lhs.m_Data[i+12] * rhs.m_Data[7];
		res.m_Data[i + 8] = lhs.m_Data[i] * rhs.m_Data[8] + lhs.m_Data[i + 4] * rhs.m_Data[9] + lhs.m_Data[i + 8] * rhs.m_Data[10];// + lhs.m_Data[i+12] * rhs.m_Data[11];
		res.m_Data[i + 12] = lhs.m_Data[i] * rhs.m_Data[12] + lhs.m_Data[i + 4] * rhs.m_Data[13] + lhs.m_Data[i + 8] * rhs.m_Data[14] + lhs.m_Data[i + 12];// * rhs.m_Data[15];
	}

	res.m_Data[3] = 0.0f;
	res.m_Data[7] = 0.0f;
	res.m_Data[11] = 0.0f;
	res.m_Data[15] = 1.0f;
}

bool Matrix4x4::Invert_Full(const Matrix4x4& inM, Matrix4x4& outM)
{
	return InvertMatrix4x4_Full(inM.m_Data, outM.m_Data);
}

bool Matrix4x4::Invert_General3D(const Matrix4x4& inM, Matrix4x4& outM)
{
	return InvertMatrix4x4_General3D(inM.m_Data, outM.m_Data);
}

#define MAT(m,r,c) (m)[(c)*4+(r)]

#define RETURN_ZERO \
{ \
	for (int i=0;i<16;i++) \
		out[i] = 0.0F; \
	return false; \
}

#define SWAP_ROWS(a, b) { float *_tmp = a; (a)=(b); (b)=_tmp; }

bool Matrix4x4::InvertMatrix4x4_Full(const float* m, float* out)
{
	float wtmp[4][8];
	float m0, m1, m2, m3, s;
	float* r0, * r1, * r2, * r3;

	r0 = wtmp[0], r1 = wtmp[1], r2 = wtmp[2], r3 = wtmp[3];

	r0[0] = MAT(m, 0, 0), r0[1] = MAT(m, 0, 1),
		r0[2] = MAT(m, 0, 2), r0[3] = MAT(m, 0, 3),
		r0[4] = 1.0, r0[5] = r0[6] = r0[7] = 0.0,

		r1[0] = MAT(m, 1, 0), r1[1] = MAT(m, 1, 1),
		r1[2] = MAT(m, 1, 2), r1[3] = MAT(m, 1, 3),
		r1[5] = 1.0, r1[4] = r1[6] = r1[7] = 0.0,

		r2[0] = MAT(m, 2, 0), r2[1] = MAT(m, 2, 1),
		r2[2] = MAT(m, 2, 2), r2[3] = MAT(m, 2, 3),
		r2[6] = 1.0, r2[4] = r2[5] = r2[7] = 0.0,

		r3[0] = MAT(m, 3, 0), r3[1] = MAT(m, 3, 1),
		r3[2] = MAT(m, 3, 2), r3[3] = MAT(m, 3, 3),
		r3[7] = 1.0, r3[4] = r3[5] = r3[6] = 0.0;

	/* choose pivot - or die */
	if (FloatConversion::Abs(r3[0]) > FloatConversion::Abs(r2[0])) SWAP_ROWS(r3, r2);
	if (FloatConversion::Abs(r2[0]) > FloatConversion::Abs(r1[0])) SWAP_ROWS(r2, r1);
	if (FloatConversion::Abs(r1[0]) > FloatConversion::Abs(r0[0])) SWAP_ROWS(r1, r0);
	if (0.0F == r0[0]) RETURN_ZERO

		/* eliminate first variable     */
		m1 = r1[0] / r0[0]; m2 = r2[0] / r0[0]; m3 = r3[0] / r0[0];
	s = r0[1]; r1[1] -= m1 * s; r2[1] -= m2 * s; r3[1] -= m3 * s;
	s = r0[2]; r1[2] -= m1 * s; r2[2] -= m2 * s; r3[2] -= m3 * s;
	s = r0[3]; r1[3] -= m1 * s; r2[3] -= m2 * s; r3[3] -= m3 * s;
	s = r0[4];
	if (s != 0.0F) { r1[4] -= m1 * s; r2[4] -= m2 * s; r3[4] -= m3 * s; }
	s = r0[5];
	if (s != 0.0F) { r1[5] -= m1 * s; r2[5] -= m2 * s; r3[5] -= m3 * s; }
	s = r0[6];
	if (s != 0.0F) { r1[6] -= m1 * s; r2[6] -= m2 * s; r3[6] -= m3 * s; }
	s = r0[7];
	if (s != 0.0F) { r1[7] -= m1 * s; r2[7] -= m2 * s; r3[7] -= m3 * s; }

	/* choose pivot - or die */
	if (FloatConversion::Abs(r3[1]) > FloatConversion::Abs(r2[1])) SWAP_ROWS(r3, r2);
	if (FloatConversion::Abs(r2[1]) > FloatConversion::Abs(r1[1])) SWAP_ROWS(r2, r1);
	if (0.0F == r1[1]) RETURN_ZERO;

	/* eliminate second variable */
	m2 = r2[1] / r1[1]; m3 = r3[1] / r1[1];
	r2[2] -= m2 * r1[2]; r3[2] -= m3 * r1[2];
	r2[3] -= m2 * r1[3]; r3[3] -= m3 * r1[3];
	s = r1[4]; if (0.0F != s) { r2[4] -= m2 * s; r3[4] -= m3 * s; }
	s = r1[5]; if (0.0F != s) { r2[5] -= m2 * s; r3[5] -= m3 * s; }
	s = r1[6]; if (0.0F != s) { r2[6] -= m2 * s; r3[6] -= m3 * s; }
	s = r1[7]; if (0.0F != s) { r2[7] -= m2 * s; r3[7] -= m3 * s; }

	/* choose pivot - or die */
	if (FloatConversion::Abs(r3[2]) > FloatConversion::Abs(r2[2])) SWAP_ROWS(r3, r2);
	if (0.0F == r2[2]) RETURN_ZERO;

	/* eliminate third variable */
	m3 = r3[2] / r2[2];
	r3[3] -= m3 * r2[3], r3[4] -= m3 * r2[4],
		r3[5] -= m3 * r2[5], r3[6] -= m3 * r2[6],
		r3[7] -= m3 * r2[7];

	/* last check */
	if (0.0F == r3[3]) RETURN_ZERO;

	s = 1.0F / r3[3];             /* now back substitute row 3 */
	r3[4] *= s; r3[5] *= s; r3[6] *= s; r3[7] *= s;

	m2 = r2[3];                 /* now back substitute row 2 */
	s = 1.0F / r2[2];
	r2[4] = s * (r2[4] - r3[4] * m2), r2[5] = s * (r2[5] - r3[5] * m2),
		r2[6] = s * (r2[6] - r3[6] * m2), r2[7] = s * (r2[7] - r3[7] * m2);
	m1 = r1[3];
	r1[4] -= r3[4] * m1, r1[5] -= r3[5] * m1,
		r1[6] -= r3[6] * m1, r1[7] -= r3[7] * m1;
	m0 = r0[3];
	r0[4] -= r3[4] * m0, r0[5] -= r3[5] * m0,
		r0[6] -= r3[6] * m0, r0[7] -= r3[7] * m0;

	m1 = r1[2];                 /* now back substitute row 1 */
	s = 1.0F / r1[1];
	r1[4] = s * (r1[4] - r2[4] * m1), r1[5] = s * (r1[5] - r2[5] * m1),
		r1[6] = s * (r1[6] - r2[6] * m1), r1[7] = s * (r1[7] - r2[7] * m1);
	m0 = r0[2];
	r0[4] -= r2[4] * m0, r0[5] -= r2[5] * m0,
		r0[6] -= r2[6] * m0, r0[7] -= r2[7] * m0;

	m0 = r0[1];                 /* now back substitute row 0 */
	s = 1.0F / r0[0];
	r0[4] = s * (r0[4] - r1[4] * m0), r0[5] = s * (r0[5] - r1[5] * m0),
		r0[6] = s * (r0[6] - r1[6] * m0), r0[7] = s * (r0[7] - r1[7] * m0);

	MAT(out, 0, 0) = r0[4]; MAT(out, 0, 1) = r0[5], MAT(out, 0, 2) = r0[6]; MAT(out, 0, 3) = r0[7],
		MAT(out, 1, 0) = r1[4]; MAT(out, 1, 1) = r1[5], MAT(out, 1, 2) = r1[6]; MAT(out, 1, 3) = r1[7],
		MAT(out, 2, 0) = r2[4]; MAT(out, 2, 1) = r2[5], MAT(out, 2, 2) = r2[6]; MAT(out, 2, 3) = r2[7],
		MAT(out, 3, 0) = r3[4]; MAT(out, 3, 1) = r3[5], MAT(out, 3, 2) = r3[6]; MAT(out, 3, 3) = r3[7];

	return true;
}

#undef SWAP_ROWS


bool Matrix4x4::InvertMatrix4x4_General3D(const float* in, float* out)
{
	float pos, neg, t;
	float det;

	// Calculate the determinant of upper left 3x3 sub-matrix and
	// determine if the matrix is singular.
	pos = neg = 0.0;
	t = MAT(in, 0, 0) * MAT(in, 1, 1) * MAT(in, 2, 2);
	if (t >= 0.0) pos += t; else neg += t;

	t = MAT(in, 1, 0) * MAT(in, 2, 1) * MAT(in, 0, 2);
	if (t >= 0.0) pos += t; else neg += t;

	t = MAT(in, 2, 0) * MAT(in, 0, 1) * MAT(in, 1, 2);
	if (t >= 0.0) pos += t; else neg += t;

	t = -MAT(in, 2, 0) * MAT(in, 1, 1) * MAT(in, 0, 2);
	if (t >= 0.0) pos += t; else neg += t;

	t = -MAT(in, 1, 0) * MAT(in, 0, 1) * MAT(in, 2, 2);
	if (t >= 0.0) pos += t; else neg += t;

	t = -MAT(in, 0, 0) * MAT(in, 2, 1) * MAT(in, 1, 2);
	if (t >= 0.0) pos += t; else neg += t;

	det = pos + neg;

	if (det * det < 1e-25)
		RETURN_ZERO;

	det = 1.0F / det;
	MAT(out, 0, 0) = ((MAT(in, 1, 1) * MAT(in, 2, 2) - MAT(in, 2, 1) * MAT(in, 1, 2)) * det);
	MAT(out, 0, 1) = (-(MAT(in, 0, 1) * MAT(in, 2, 2) - MAT(in, 2, 1) * MAT(in, 0, 2)) * det);
	MAT(out, 0, 2) = ((MAT(in, 0, 1) * MAT(in, 1, 2) - MAT(in, 1, 1) * MAT(in, 0, 2)) * det);
	MAT(out, 1, 0) = (-(MAT(in, 1, 0) * MAT(in, 2, 2) - MAT(in, 2, 0) * MAT(in, 1, 2)) * det);
	MAT(out, 1, 1) = ((MAT(in, 0, 0) * MAT(in, 2, 2) - MAT(in, 2, 0) * MAT(in, 0, 2)) * det);
	MAT(out, 1, 2) = (-(MAT(in, 0, 0) * MAT(in, 1, 2) - MAT(in, 1, 0) * MAT(in, 0, 2)) * det);
	MAT(out, 2, 0) = ((MAT(in, 1, 0) * MAT(in, 2, 1) - MAT(in, 2, 0) * MAT(in, 1, 1)) * det);
	MAT(out, 2, 1) = (-(MAT(in, 0, 0) * MAT(in, 2, 1) - MAT(in, 2, 0) * MAT(in, 0, 1)) * det);
	MAT(out, 2, 2) = ((MAT(in, 0, 0) * MAT(in, 1, 1) - MAT(in, 1, 0) * MAT(in, 0, 1)) * det);

	// Do the translation part
	MAT(out, 0, 3) = -(MAT(in, 0, 3) * MAT(out, 0, 0) +
		MAT(in, 1, 3) * MAT(out, 0, 1) +
		MAT(in, 2, 3) * MAT(out, 0, 2));
	MAT(out, 1, 3) = -(MAT(in, 0, 3) * MAT(out, 1, 0) +
		MAT(in, 1, 3) * MAT(out, 1, 1) +
		MAT(in, 2, 3) * MAT(out, 1, 2));
	MAT(out, 2, 3) = -(MAT(in, 0, 3) * MAT(out, 2, 0) +
		MAT(in, 1, 3) * MAT(out, 2, 1) +
		MAT(in, 2, 3) * MAT(out, 2, 2));

	MAT(out, 3, 0) = 0.0f;
	MAT(out, 3, 1) = 0.0f;
	MAT(out, 3, 2) = 0.0f;
	MAT(out, 3, 3) = 1.0f;

	return true;
}

#undef MAT
#undef RETURN_ZERO

Matrix4x4 Matrix4x4::Identity()
{
	Matrix4x4 temp;
	temp.SetIdentity();
	return temp;
}

const Matrix4x4 Matrix4x4::identity = Identity();
#include "Matrix3x3.hpp"
#include "Matrix4x4.hpp"
#include <utility>

Matrix3x3::Matrix3x3()
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
}

Matrix3x3::Matrix3x3(float m00, float m01, float m02, float m10, float m11, float m12, float m20, float m21, float m22)
{
	Get(0, 0) = m00;
	Get(1, 0) = m10;
	Get(2, 0) = m20;
	Get(0, 1) = m01;
	Get(1, 1) = m11;
	Get(2, 1) = m21;
	Get(0, 2) = m02;
	Get(1, 2) = m12;
	Get(2, 2) = m22;
}

Matrix3x3::Matrix3x3(const Matrix4x4& other)
{
	m_Data[0] = other.m_Data[0];
	m_Data[1] = other.m_Data[1];
	m_Data[2] = other.m_Data[2];

	m_Data[3] = other.m_Data[4];
	m_Data[4] = other.m_Data[5];
	m_Data[5] = other.m_Data[6];

	m_Data[6] = other.m_Data[8];
	m_Data[7] = other.m_Data[9];
	m_Data[8] = other.m_Data[10];
}

float& Matrix3x3::Get(int row, int column)
{
	return m_Data[row + (column * 3)];
}

const float& Matrix3x3::Get(int row, int column)const
{
	return m_Data[row + (column * 3)];
}

float& Matrix3x3::operator [] (int row)
{ 
	return m_Data[row];
}

float Matrix3x3::operator [] (int row) const
{ 
	return m_Data[row];
}

float* Matrix3x3::GetPtr()
{ 
	return m_Data;
}

const float* Matrix3x3::GetPtr() const
{ 
	return m_Data;
}

UnityVector3 Matrix3x3::GetColumn(int col) const 
{ 
	return UnityVector3(Get(0, col), Get(1, col), Get(2, col));
}

Matrix3x3& Matrix3x3::operator = (const Matrix4x4& other)
{
	m_Data[0] = other.m_Data[0];
	m_Data[1] = other.m_Data[1];
	m_Data[2] = other.m_Data[2];

	m_Data[3] = other.m_Data[4];
	m_Data[4] = other.m_Data[5];
	m_Data[5] = other.m_Data[6];

	m_Data[6] = other.m_Data[8];
	m_Data[7] = other.m_Data[9];
	m_Data[8] = other.m_Data[10];
	return *this;
}

Matrix3x3& Matrix3x3::operator *= (float f)
{
	for (int i = 0; i < 9; i++)
	{
		m_Data[i] *= f;
	}
	return *this;
}

Matrix3x3& Matrix3x3::operator /= (float f) 
{ 
	return *this *= (1.0F / f);
}

Matrix3x3& Matrix3x3::operator *= (const Matrix3x3& inM)
{
	int i;
	for (i = 0; i < 3; i++)
	{
		float v[3] = { Get(i, 0), Get(i, 1), Get(i, 2) };
		Get(i, 0) = v[0] * inM.Get(0, 0) + v[1] * inM.Get(1, 0) + v[2] * inM.Get(2, 0);
		Get(i, 1) = v[0] * inM.Get(0, 1) + v[1] * inM.Get(1, 1) + v[2] * inM.Get(2, 1);
		Get(i, 2) = v[0] * inM.Get(0, 2) + v[1] * inM.Get(1, 2) + v[2] * inM.Get(2, 2);
	}
	return *this;
}

Matrix3x3& Matrix3x3::operator *= (const Matrix4x4& inM)
{
	int i;
	for (i = 0; i < 3; i++)
	{
		float v[3] = { Get(i, 0), Get(i, 1), Get(i, 2) };
		Get(i, 0) = v[0] * inM.Get(0, 0) + v[1] * inM.Get(1, 0) + v[2] * inM.Get(2, 0);
		Get(i, 1) = v[0] * inM.Get(0, 1) + v[1] * inM.Get(1, 1) + v[2] * inM.Get(2, 1);
		Get(i, 2) = v[0] * inM.Get(0, 2) + v[1] * inM.Get(1, 2) + v[2] * inM.Get(2, 2);
	}
	return *this;
}

Matrix3x3 operator * (const Matrix3x3& lhs, const Matrix3x3& rhs)
{
	Matrix3x3 temp(lhs);
	temp *= rhs;
	return temp;
}

UnityVector3 Matrix3x3::MultiplyUnityVector3(const UnityVector3& v) const
{
	UnityVector3 res;
	res.x = m_Data[0] * v.x + m_Data[3] * v.y + m_Data[6] * v.z;
	res.y = m_Data[1] * v.x + m_Data[4] * v.y + m_Data[7] * v.z;
	res.z = m_Data[2] * v.x + m_Data[5] * v.y + m_Data[8] * v.z;
	return res;
}

void Matrix3x3::MultiplyUnityVector3(const UnityVector3& v, UnityVector3& output) const
{
	output.x = m_Data[0] * v.x + m_Data[3] * v.y + m_Data[6] * v.z;
	output.y = m_Data[1] * v.x + m_Data[4] * v.y + m_Data[7] * v.z;
	output.z = m_Data[2] * v.x + m_Data[5] * v.y + m_Data[8] * v.z;
}

UnityVector3 Matrix3x3::MultiplyPoint3(const UnityVector3& inV) const
{
	return MultiplyUnityVector3(inV);
}

UnityVector3 Matrix3x3::MultiplyUnityVector3Transpose(const UnityVector3& v) const
{
	UnityVector3 res;
	res.x = Get(0, 0) * v.x + Get(1, 0) * v.y + Get(2, 0) * v.z;
	res.y = Get(0, 1) * v.x + Get(1, 1) * v.y + Get(2, 1) * v.z;
	res.z = Get(0, 2) * v.x + Get(1, 2) * v.y + Get(2, 2) * v.z;
	return res;
}

UnityVector3 Matrix3x3::MultiplyPoint3Transpose(const UnityVector3& inV) const
{ 
	return MultiplyUnityVector3Transpose(inV);
}

float Matrix3x3::GetDeterminant() const
{
	float fCofactor0 = Get(0, 0) * Get(1, 1) * Get(2, 2);
	float fCofactor1 = Get(0, 1) * Get(1, 2) * Get(2, 0);
	float fCofactor2 = Get(0, 2) * Get(1, 0) * Get(2, 1);

	float fCofactor3 = Get(0, 2) * Get(1, 1) * Get(2, 0);
	float fCofactor4 = Get(0, 1) * Get(1, 0) * Get(2, 2);
	float fCofactor5 = Get(0, 0) * Get(1, 2) * Get(2, 1);

	return fCofactor0 + fCofactor1 + fCofactor2 - fCofactor3 - fCofactor4 - fCofactor5;
}

Matrix3x3& Matrix3x3::Transpose()
{
	std::swap(Get(0, 1), Get(1, 0));
	std::swap(Get(0, 2), Get(2, 0));
	std::swap(Get(2, 1), Get(1, 2));
	return *this;
}

bool Matrix3x3::Invert()
{
	///@TODO make a fast but robust inverse matrix 3x3
	Matrix4x4 m = *this;
	bool success = Matrix4x4::InvertMatrix4x4_Full(m.GetPtr(), m.GetPtr());
	*this = m;
	return success;
}

void Matrix3x3::InvertTranspose()
{
	Invert();
	Transpose();
}

Matrix3x3& Matrix3x3::SetIdentity()
{
	Get(0, 0) = 1.0F;	Get(0, 1) = 0.0F;	Get(0, 2) = 0.0F;
	Get(1, 0) = 0.0F;	Get(1, 1) = 1.0F;	Get(1, 2) = 0.0F;
	Get(2, 0) = 0.0F;	Get(2, 1) = 0.0F;	Get(2, 2) = 1.0F;
	return *this;
}

Matrix3x3& Matrix3x3::SetZero()
{
	Get(0, 0) = 0.0F;	Get(0, 1) = 0.0F;	Get(0, 2) = 0.0F;
	Get(1, 0) = 0.0F;	Get(1, 1) = 0.0F;	Get(1, 2) = 0.0F;
	Get(2, 0) = 0.0F;	Get(2, 1) = 0.0F;	Get(2, 2) = 0.0F;
	return *this;
}

Matrix3x3& Matrix3x3::SetFromToRotation(const UnityVector3& from, const UnityVector3& to)
{
	float mtx[3][3];
	FromToRotation(from.GetPtr(), to.GetPtr(), mtx);
	Get(0, 0) = mtx[0][0];	Get(0, 1) = mtx[0][1];	Get(0, 2) = mtx[0][2];
	Get(1, 0) = mtx[1][0];	Get(1, 1) = mtx[1][1];	Get(1, 2) = mtx[1][2];
	Get(2, 0) = mtx[2][0];	Get(2, 1) = mtx[2][1];	Get(2, 2) = mtx[2][2];
	return *this;
}

Matrix3x3& Matrix3x3::SetAxisAngle(const UnityVector3& rotationAxis, float radians)
{
	GetRotMatrixNormVec(m_Data, rotationAxis.GetPtr(), radians);
	return *this;
}

Matrix3x3& Matrix3x3::SetOrthoNormalBasis(const UnityVector3& inX, const UnityVector3& inY, const UnityVector3& inZ)
{
	Get(0, 0) = inX[0];	Get(0, 1) = inY[0];	Get(0, 2) = inZ[0];
	Get(1, 0) = inX[1];	Get(1, 1) = inY[1];	Get(1, 2) = inZ[1];
	Get(2, 0) = inX[2];	Get(2, 1) = inY[2];	Get(2, 2) = inZ[2];
	return *this;
}

Matrix3x3& Matrix3x3::SetOrthoNormalBasisInverse(const UnityVector3& inX, const UnityVector3& inY, const UnityVector3& inZ)
{
	Get(0, 0) = inX[0];	Get(1, 0) = inY[0];	Get(2, 0) = inZ[0];
	Get(0, 1) = inX[1];	Get(1, 1) = inY[1];	Get(2, 1) = inZ[1];
	Get(0, 2) = inX[2];	Get(1, 2) = inY[2];	Get(2, 2) = inZ[2];
	return *this;
}

Matrix3x3& Matrix3x3::SetScale(const UnityVector3& inScale)
{
	Get(0, 0) = inScale[0];	Get(0, 1) = 0.0F;			Get(0, 2) = 0.0F;
	Get(1, 0) = 0.0F;			Get(1, 1) = inScale[1];	Get(1, 2) = 0.0F;
	Get(2, 0) = 0.0F;			Get(2, 1) = 0.0F;			Get(2, 2) = inScale[2];
	return *this;
}

Matrix3x3& Matrix3x3::Scale(const UnityVector3& inScale)
{
	Get(0, 0) *= inScale[0];
	Get(1, 0) *= inScale[0];
	Get(2, 0) *= inScale[0];

	Get(0, 1) *= inScale[1];
	Get(1, 1) *= inScale[1];
	Get(2, 1) *= inScale[1];

	Get(0, 2) *= inScale[2];
	Get(1, 2) *= inScale[2];
	Get(2, 2) *= inScale[2];
	return *this;
}

bool Matrix3x3::IsIdentity(float threshold) 
{
	if (FloatConversion::CompareApproximately(Get(0, 0), 1.0f, threshold) && FloatConversion::CompareApproximately(Get(0, 1), 0.0f, threshold) && FloatConversion::CompareApproximately(Get(0, 2), 0.0f, threshold) && FloatConversion::CompareApproximately(Get(1, 0), 0.0f, threshold) && FloatConversion::CompareApproximately(Get(1, 1), 1.0f, threshold) && FloatConversion::CompareApproximately(Get(1, 2), 0.0f, threshold) && FloatConversion::CompareApproximately(Get(2, 0), 0.0f, threshold) && FloatConversion::CompareApproximately(Get(2, 1), 0.0f, threshold) && FloatConversion::CompareApproximately(Get(2, 2), 1.0f, threshold))
	{
		return true;
	}
	return false;
}

void Matrix3x3::MakePositive(UnityVector3& euler)
{
	const float negativeFlip = -0.0001F;
	const float positiveFlip = (PI * 2.0F) - 0.0001F;

	if (euler.x < negativeFlip)
		euler.x += 2.0 * PI;
	else if (euler.x > positiveFlip)
		euler.x -= 2.0 * PI;

	if (euler.y < negativeFlip)
		euler.y += 2.0 * PI;
	else if (euler.y > positiveFlip)
		euler.y -= 2.0 * PI;

	if (euler.z < negativeFlip)
		euler.z += 2.0 * PI;
	else if (euler.z > positiveFlip)
		euler.z -= 2.0 * PI;
}

void Matrix3x3::SanitizeEuler(UnityVector3& euler)
{
	MakePositive(euler);
}

void Matrix3x3::EulerToMatrix(const UnityVector3& v, Matrix3x3& matrix)
{
	float cx = cos(v.x);
	float sx = sin(v.x);
	float cy = cos(v.y);
	float sy = sin(v.y);
	float cz = cos(v.z);
	float sz = sin(v.z);

	matrix.Get(0, 0) = cy * cz + sx * sy * sz;
	matrix.Get(0, 1) = cz * sx * sy - cy * sz;
	matrix.Get(0, 2) = cx * sy;

	matrix.Get(1, 0) = cx * sz;
	matrix.Get(1, 1) = cx * cz;
	matrix.Get(1, 2) = -sx;

	matrix.Get(2, 0) = -cz * sy + cy * sx * sz;
	matrix.Get(2, 1) = cy * cz * sx + sy * sz;
	matrix.Get(2, 2) = cx * cy;
}

bool Matrix3x3::MatrixToEuler(const Matrix3x3& matrix, UnityVector3& v)
{
	if (matrix.Get(1, 2) < 0.999F)
	{
		if (matrix.Get(1, 2) > -0.999F)
		{
			v.x = asin(-matrix.Get(1, 2));
			v.y = atan2(matrix.Get(0, 2), matrix.Get(2, 2));
			v.z = atan2(matrix.Get(1, 0), matrix.Get(1, 1));
			SanitizeEuler(v);
			return true;
		}
		else
		{
			v.x = PI * 0.5F;
			v.y = atan2(matrix.Get(0, 1), matrix.Get(0, 0));
			v.z = 0.0F;
			SanitizeEuler(v);

			return false;
		}
	}
	else
	{
		v.x = -PI * 0.5F;
		v.y = atan2(-matrix.Get(0, 1), matrix.Get(0, 0));
		v.z = 0.0F;
		SanitizeEuler(v);
		return false;
	}
}

void Matrix3x3::FromToRotation(const float* from, const float* to, float mtx[3][3])
{
#define EPSILON 0.000001

#define CROSS(dest,v1,v2){                 \
          dest[0]=v1[1]*v2[2]-v1[2]*v2[1]; \
          dest[1]=v1[2]*v2[0]-v1[0]*v2[2]; \
          dest[2]=v1[0]*v2[1]-v1[1]*v2[0];}

#define DOT(v1,v2) (v1[0]*v2[0]+v1[1]*v2[1]+v1[2]*v2[2])

#define SUB(dest,v1,v2){       \
          dest[0]=v1[0]-v2[0]; \
          dest[1]=v1[1]-v2[1]; \
          dest[2]=v1[2]-v2[2];}


	float v[3];
	float e, h;
	CROSS(v, from, to);
	e = DOT(from, to);
	if (e > 1.0 - EPSILON)     /* "from" almost or equal to "to"-vector? */
	{
		/* return identity */
		mtx[0][0] = 1.0; mtx[0][1] = 0.0; mtx[0][2] = 0.0;
		mtx[1][0] = 0.0; mtx[1][1] = 1.0; mtx[1][2] = 0.0;
		mtx[2][0] = 0.0; mtx[2][1] = 0.0; mtx[2][2] = 1.0;
	}
	else if (e < -1.0 + EPSILON) /* "from" almost or equal to negated "to"? */
	{
		float up[3], left[3];
		float invlen;
		float fxx, fyy, fzz, fxy, fxz, fyz;
		float uxx, uyy, uzz, uxy, uxz, uyz;
		float lxx, lyy, lzz, lxy, lxz, lyz;
		/* left=CROSS(from, (1,0,0)) */
		left[0] = 0.0; left[1] = from[2]; left[2] = -from[1];
		if (DOT(left, left) < EPSILON) /* was left=CROSS(from,(1,0,0)) a good choice? */
		{
			/* here we now that left = CROSS(from, (1,0,0)) will be a good choice */
			left[0] = -from[2]; left[1] = 0.0; left[2] = from[0];
		}
		/* normalize "left" */
		invlen = 1.0f / sqrt(DOT(left, left));
		left[0] *= invlen;
		left[1] *= invlen;
		left[2] *= invlen;
		CROSS(up, left, from);

		fxx = -from[0] * from[0]; fyy = -from[1] * from[1]; fzz = -from[2] * from[2];
		fxy = -from[0] * from[1]; fxz = -from[0] * from[2]; fyz = -from[1] * from[2];

		uxx = up[0] * up[0]; uyy = up[1] * up[1]; uzz = up[2] * up[2];
		uxy = up[0] * up[1]; uxz = up[0] * up[2]; uyz = up[1] * up[2];

		lxx = -left[0] * left[0]; lyy = -left[1] * left[1]; lzz = -left[2] * left[2];
		lxy = -left[0] * left[1]; lxz = -left[0] * left[2]; lyz = -left[1] * left[2];

		mtx[0][0] = fxx + uxx + lxx; mtx[0][1] = fxy + uxy + lxy; mtx[0][2] = fxz + uxz + lxz;
		mtx[1][0] = mtx[0][1];   mtx[1][1] = fyy + uyy + lyy; mtx[1][2] = fyz + uyz + lyz;
		mtx[2][0] = mtx[0][2];   mtx[2][1] = mtx[1][2];   mtx[2][2] = fzz + uzz + lzz;
	}
	else
	{
		float hvx, hvz, hvxy, hvxz, hvyz;
		h = (1.0f - e) / DOT(v, v);
		hvx = h * v[0];
		hvz = h * v[2];
		hvxy = hvx * v[1];
		hvxz = hvx * v[2];
		hvyz = hvz * v[1];
		mtx[0][0] = e + hvx * v[0]; mtx[0][1] = hvxy - v[2];     mtx[0][2] = hvxz + v[1];
		mtx[1][0] = hvxy + v[2];  mtx[1][1] = e + h * v[1] * v[1]; mtx[1][2] = hvyz - v[0];
		mtx[2][0] = hvxz - v[1];  mtx[2][1] = hvyz + v[0];     mtx[2][2] = e + hvz * v[2];
	}

#undef EPSILON
#undef CROSS
#undef DOT
#undef SUB
}

bool Matrix3x3::LookRotationToMatrix(const UnityVector3& viewVec, const UnityVector3& upVec, Matrix3x3* m)
{
	UnityVector3 z = viewVec;
	float mag = UnityVector3::Magnitude(z);
	if (mag < UnityVector3::epsilon)
	{
		m->SetIdentity();
		return false;
	}
	z /= mag;

	UnityVector3 x = UnityVector3::Cross(upVec, z);
	mag = UnityVector3::Magnitude(x);
	if (mag < UnityVector3::epsilon)
	{
		m->SetIdentity();
		return false;
	}
	x /= mag;

	UnityVector3 y(UnityVector3::Cross(z, x));
	if (!FloatConversion::CompareApproximately(UnityVector3::SqrMagnitude(y), 1.0F))
		return false;

	m->SetOrthoNormalBasis(x, y, z);
	return true;
}

void Matrix3x3::GetRotMatrixNormVec(float* out, const float* inVec, float radians)
{

	float s, c;
	float vx, vy, vz, xx, yy, zz, xy, yz, zx, xs, ys, zs, one_c;

	s = sin(radians);
	c = cos(radians);

	vx = inVec[0];
	vy = inVec[1];
	vz = inVec[2];

#define M(row,col)  out[row*3 + col]
	xx = vx * vx;
	yy = vy * vy;
	zz = vz * vz;
	xy = vx * vy;
	yz = vy * vz;
	zx = vz * vx;
	xs = vx * s;
	ys = vy * s;
	zs = vz * s;
	one_c = 1.0F - c;

	M(0, 0) = (one_c * xx) + c;
	M(1, 0) = (one_c * xy) - zs;
	M(2, 0) = (one_c * zx) + ys;

	M(0, 1) = (one_c * xy) + zs;
	M(1, 1) = (one_c * yy) + c;
	M(2, 1) = (one_c * yz) - xs;

	M(0, 2) = (one_c * zx) - ys;
	M(1, 2) = (one_c * yz) + xs;
	M(2, 2) = (one_c * zz) + c;

#undef M
}


void Matrix3x3::OrthoNormalize(Matrix3x3& matrix)
{
	UnityVector3* c0 = (UnityVector3*)matrix.GetPtr() + 0;
	UnityVector3* c1 = (UnityVector3*)matrix.GetPtr() + 3;
	UnityVector3* c2 = (UnityVector3*)matrix.GetPtr() + 6;
	UnityVector3::OrthoNormalize(c0, c1, c2);
}

Matrix3x3 Matrix3x3::Zero()
{
	Matrix3x3 temp;
	temp.SetZero();
	return temp;
}

Matrix3x3 Matrix3x3::Identity()
{
	Matrix3x3 temp;
	temp.SetIdentity();
	return temp;
}

const Matrix3x3 Matrix3x3::zero = Zero();
const Matrix3x3 Matrix3x3::identity = Identity();
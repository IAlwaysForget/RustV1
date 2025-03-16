#ifndef MATRIX4X4_HPP
#define MATRIX4X4_HPP

#include "Vector3.hpp"
#include "Vector4.hpp"
#include "Quaternion.hpp"

class Quaternion;

class Matrix4x4
{
public:
	float m_Data[16];

	Matrix4x4();
	Matrix4x4(const Matrix3x3& other);
	explicit Matrix4x4(const float data[16]);

	float& Get(int row, int column);
	const float& Get(int row, int column)const;
	float* GetPtr();
	const float* GetPtr()const;

	float operator [] (int index) const;
	float& operator [] (int index);

	Matrix4x4& operator *= (const Matrix4x4& inM);
	Matrix4x4& operator=(const Matrix3x3& other);

	bool IsIdentity(float epsilon = UnityVector3::epsilon) const;
	double GetDeterminant() const;

	Matrix4x4& Invert_Full();

	Matrix4x4& Transpose();
	Matrix4x4& Copy(const Matrix4x4& inM);
	Matrix4x4& SetIdentity();

	Matrix4x4& SetPerspective(float fovy, float aspect, float zNear, float zFar);
	Matrix4x4& SetPerspectiveCotan(float cotanHalfFOV, float zNear, float zFar);
	Matrix4x4& SetOrtho(float left, float right, float bottom, float top, float zNear, float zFar);
	Matrix4x4& SetFrustum(float left, float right, float bottom, float top, float nearval, float farval);

	UnityVector3 GetAxisX() const;
	UnityVector3 GetAxisY() const;
	UnityVector3 GetAxisZ() const;
	UnityVector3 GetPosition() const;
	UnityVector4 GetRow(int row) const;
	UnityVector4 GetColumn(int col) const;

	void SetAxisX(const UnityVector3& v);
	void SetAxisY(const UnityVector3& v);
	void SetAxisZ(const UnityVector3& v);
	void SetPosition(const UnityVector3& v);
	void SetRow(int row, const UnityVector4& v);
	void SetColumn(int col, const UnityVector4& v);

	Matrix4x4& SetTranslate(const UnityVector3& inTrans);
	Matrix4x4& SetOrthoNormalBasis(const UnityVector3& inX, const UnityVector3& inY, const UnityVector3& inZ);
	Matrix4x4& SetOrthoNormalBasisInverse(const UnityVector3& inX, const UnityVector3& inY, const UnityVector3& inZ);
	Matrix4x4& SetScale(const UnityVector3& inScale);
	Matrix4x4& SetPositionAndOrthoNormalBasis(const UnityVector3& inPosition, const UnityVector3& inX, const UnityVector3& inY, const UnityVector3& inZ);

	Matrix4x4& Translate(const UnityVector3& inTrans);
	Matrix4x4& Scale(const UnityVector3& inScale);

	Matrix4x4& SetFromToRotation(const UnityVector3& from, const UnityVector3& to);

	void SetTR(const UnityVector3& pos, const Quaternion& q);
	void SetTRS(const UnityVector3& pos, const Quaternion& q, const UnityVector3& s);
	void SetTRInverse(const UnityVector3& pos, const Quaternion& q);

	UnityVector3 MultiplyPoint3(const UnityVector3& v) const;
	void MultiplyPoint3(const UnityVector3& v, UnityVector3& output) const;
	UnityVector3 MultiplyUnityVector3(const UnityVector3& v) const;
	void MultiplyUnityVector3(const UnityVector3& v, UnityVector3& output) const;
	bool PerspectiveMultiplyPoint3(const UnityVector3& v, UnityVector3& output) const;
	bool PerspectiveMultiplyUnityVector3(const UnityVector3& v, UnityVector3& output) const;
	UnityVector3 InverseMultiplyPoint3Affine(const UnityVector3& inV) const;
	UnityVector3 InverseMultiplyUnityVector3Affine(const UnityVector3& v) const;
	bool IsFinite(const Matrix4x4& f);

	static void CopyMatrix(const float* __restrict lhs, float* __restrict res);
	static void TransposeMatrix4x4(const Matrix4x4* __restrict lhs, Matrix4x4* __restrict res);
	static void MultiplyMatrices4x4(const Matrix4x4* __restrict lhs, const Matrix4x4* __restrict rhs, Matrix4x4* __restrict res);
	static void MultiplyMatrixArray4x4(const Matrix4x4* __restrict a, const Matrix4x4* __restrict b, Matrix4x4* __restrict res, size_t count);
	static void MultiplyMatrixArrayWithBase4x4(const Matrix4x4* __restrict base, const Matrix4x4* __restrict a, const Matrix4x4* __restrict b, Matrix4x4* __restrict res, size_t count);
	static bool CompareApproximately(const Matrix4x4& lhs, const Matrix4x4& rhs, float dist = UnityVector3::epsilon);
	static void TransformPoints3x3(const Matrix4x4& matrix, const UnityVector3* input, UnityVector3* ouput, int count);
	static void TransformPoints3x4(const Matrix4x4& matrix, const UnityVector3* input, UnityVector3* ouput, int count);
	static void TransformPoints3x3(const Matrix4x4& matrix, const UnityVector3* input, size_t inStride, UnityVector3* ouput, size_t outStride, int count);
	static void TransformPoints3x4(const Matrix4x4& matrix, const UnityVector3* input, size_t inStride, UnityVector3* ouput, size_t outStride, int count);
	static void MultiplyMatrices3x4(const Matrix4x4& lhs, const Matrix4x4& rhs, Matrix4x4& res);
	static bool Invert_Full(const Matrix4x4& inM, Matrix4x4& outM);
	static bool Invert_General3D(const Matrix4x4& inM, Matrix4x4& outM);
	static bool InvertMatrix4x4_Full(const float* m, float* out);
	static bool InvertMatrix4x4_General3D(const float* in, float* out);	

	static Matrix4x4 Identity();

	static const Matrix4x4 identity;
};

#endif
#ifndef MATRIX3X3_HPP
#define MATRIX3X3_HPP

#include "Vector3.hpp"

class Matrix3x3
{
public:
	float m_Data[9];

	Matrix3x3();
	Matrix3x3(float m00, float m01, float m02, float m10, float m11, float m12, float m20, float m21, float m22);
	explicit Matrix3x3(const class Matrix4x4& m);

	float& Get(int row, int column);
	const float& Get(int row, int column)const;

	float& operator [] (int row);
	float operator [] (int row) const;

	float* GetPtr();
	const float* GetPtr() const;
	UnityVector3 GetColumn(int col) const;

	Matrix3x3& operator = (const Matrix4x4& m);
	Matrix3x3& operator *= (float f);
	Matrix3x3& operator /= (float f);
	Matrix3x3& operator *= (const Matrix3x3& inM);
	Matrix3x3& operator *= (const class Matrix4x4& inM);
	friend Matrix3x3 operator * (const Matrix3x3& lhs, const Matrix3x3& rhs);

	UnityVector3 MultiplyUnityVector3(const UnityVector3& inV) const;
	void MultiplyUnityVector3(const UnityVector3& inV, UnityVector3& output) const;
	UnityVector3 MultiplyPoint3(const UnityVector3& inV) const;
	UnityVector3 MultiplyUnityVector3Transpose(const UnityVector3& inV) const;
	UnityVector3 MultiplyPoint3Transpose(const UnityVector3& inV) const;
	float GetDeterminant() const;

	Matrix3x3& Transpose();
	bool Invert();
	void InvertTranspose();

	Matrix3x3& SetIdentity();
	Matrix3x3& SetZero();
	Matrix3x3& SetFromToRotation(const UnityVector3& from, const UnityVector3& to);
	Matrix3x3& SetAxisAngle(const UnityVector3& rotationAxis, float radians);
	Matrix3x3& SetOrthoNormalBasis(const UnityVector3& inX, const UnityVector3& inY, const UnityVector3& inZ);
	Matrix3x3& SetOrthoNormalBasisInverse(const UnityVector3& inX, const UnityVector3& inY, const UnityVector3& inZ);
	Matrix3x3& SetScale(const UnityVector3& inScale);
	Matrix3x3& Scale(const UnityVector3& inScale);

	bool IsIdentity(float threshold = UnityVector3::epsilon);

	static void MakePositive(UnityVector3& euler);
	static void SanitizeEuler(UnityVector3& euler);
	static void EulerToMatrix(const UnityVector3& v, Matrix3x3& matrix);
	static bool MatrixToEuler(const Matrix3x3& matrix, UnityVector3& v);
	static void FromToRotation(const float* from, const float* to, float mtx[3][3]);
	static bool LookRotationToMatrix(const UnityVector3& viewVec, const UnityVector3& upVec, Matrix3x3* m);
	static void GetRotMatrixNormVec(float* out, const float* inVec, float radians);
	static void OrthoNormalize(Matrix3x3& matrix);

	static Matrix3x3 Zero();
	static Matrix3x3 Identity();

	static const Matrix3x3 zero;
	static const Matrix3x3 identity;
};

#endif
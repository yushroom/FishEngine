#pragma once

#include "Vector3.hpp"
#include "Vector4.hpp"
#include "Quaternion.hpp"

namespace FishEngine
{

	// Matrices are row major.
	class FE_EXPORT Matrix4x4
	{
	public:

		union
		{
			float m[4][4];
			struct { Vector4 rows[4]; };
		};

		static const Matrix4x4 identity;
		static const Matrix4x4 zero;

		Matrix4x4();

		Matrix4x4(const float mat[4][4]);

		Matrix4x4(
			const float m00, const float m01, const float m02, const float m03, // row 0
			const float m10, const float m11, const float m12, const float m13, // row 1
			const float m20, const float m21, const float m22, const float m23, // row 2
			const float m30, const float m31, const float m32, const float m33); // row 3

		Matrix4x4(const Vector4& row0, const Vector4& row1, const Vector4& row2, const Vector4& row3);

		const float* data() const { return m[0]; }
		float*       data()       { return m[0]; }

		const Vector4 operator[](int row) const { return rows[row]; }
		Vector4&      operator[](int row)       { return rows[row]; }

		// The inverse of this matrix (Read Only).
		Matrix4x4 inverse() const;

		// Returns the transpose of this matrix (Read Only).
		Matrix4x4 transpose() const;

		// Is this the identity matrix?
		bool isIdentity() const;

		// The determinant of the matrix.
		float determinant() const;

		// Returns the Inverse of mat.
		static Matrix4x4 Inverse(const Matrix4x4& mat);

		// The determinant of mat.
		static float Determinant(const Matrix4x4& mat);

		// Returns the transpose of mat.
		static Matrix4x4 Transpose(const Matrix4x4& mat);

		// Sets this matrix to a translation, rotation and scaling matrix.
		void SetTRS(const Vector3& pos, const Quaternion& q, const Vector3& s);

		// Creates a translation, rotation and scaling matrix.
		static Matrix4x4 TRS( const Vector3& pos, const Quaternion& q, const Vector3& s);

		// get concatenation of transforms and its inverse
		static void TRS(
			const Vector3&      translation,
			const Quaternion&   rotation,
			const Vector3&      scale,
			Matrix4x4&          outLocalToWorld,
			Matrix4x4&          outWorldToLocal);

		static void Decompose(
			const Matrix4x4&    transformation,
			Vector3*            outTranslation,
			Quaternion*         outRotation,
			Vector3*            outScale);

		// Creates an orthogonal projection matrix.
		static Matrix4x4 Ortho(
			const float left,
			const float right,
			const float bottom,
			const float top,
			const float zNear,
			const float zFar);

		// Creates a perspective projection matrix.
		static Matrix4x4 Perspective(
			const float fovy,
			const float aspect,
			const float zNear,
			const float zFar);

		void             operator*=(const Matrix4x4& rhs);
		friend Matrix4x4 operator* (const Matrix4x4& lhs, const Matrix4x4& rhs);
		friend Vector4   operator* (const Matrix4x4& lhs, const Vector4&   rhs);
		friend bool      operator==(const Matrix4x4& lhs, const Matrix4x4& rhs);
		friend bool      operator!=(const Matrix4x4& lhs, const Matrix4x4& rhs);

		// Get a column of the matrix.
		Vector4 GetColumn(int i) const;

		// Returns a row of the matrix.
		Vector4 GetRow(int i) const;

		// Sets a column of the matrix.
		void SetColumn(int i, const Vector4& v);

		// Sets a row of the matrix.
		void SetRow(int i, const Vector4& v);


		// Transforms a position by this matrix (generic).
		Vector3 MultiplyPoint(const float x, const float y, const float z) const;

		// Transforms a position by this matrix (generic).
		Vector3 MultiplyPoint(const Vector3& v) const;

		// Transforms a position by this matrix (fast).
		Vector3 MultiplyPoint3x4(const float x, const float y, const float z) const;

		// Transforms a position by this matrix (fast).
		Vector3 MultiplyPoint3x4(const Vector3& v) const;

		// Transforms a direction by this matrix.
		Vector3 MultiplyVector(const float x, const float y, const float z) const;

		// Transforms a direction by this matrix.
		Vector3 MultiplyVector(const Vector3& v) const;


		// Creates a scaling matrix.
		static Matrix4x4 Scale(float scale);

		// Creates a scaling matrix.
		static Matrix4x4 Scale(const float x, const float y, const float z);

		// Creates a scaling matrix.
		static Matrix4x4 Scale(const Vector3& scale);

		Quaternion ToRotation() const;

		// rotation should be unit quaternion
		static Matrix4x4 FromRotation( const Quaternion& rotation );

		/**
		 * Build a left handed look at view matrix(world space to local space).
		 *
		 * @param const Vector3 & eye Position of the camera
		 * @param const Vector3 & target Position where the camera is looking at
		 * @param const Vector3 & up Normalized up vector, how the camera is oriented. Typically (0, 0, 1)
		 * @return FishEngine::Matrix4x4
		 */
		 static Matrix4x4 LookAt(
			const Vector3& eye,
			const Vector3& target,
			const Vector3& up);
	};
}

#include "Matrix4x4.inl"

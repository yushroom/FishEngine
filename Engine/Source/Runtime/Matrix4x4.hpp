#ifndef Matrix4x4_hpp
#define Matrix4x4_hpp

#include "Vector3.hpp"
#include "Vector4.hpp"
#include "Quaternion.hpp"

namespace FishEngine {

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

		Matrix4x4()
		{
			m[0][0] = 1; m[0][1] = 0; m[0][2] = 0; m[0][3] = 0;
			m[1][0] = 0; m[1][1] = 1; m[1][2] = 0; m[1][3] = 0;
			m[2][0] = 0; m[2][1] = 0; m[2][2] = 1; m[2][3] = 0;
			m[3][0] = 0; m[3][1] = 0; m[3][2] = 0; m[3][3] = 1;
		}

		Matrix4x4(const float mat[4][4])
		{
			memcpy(m, mat, 16 * sizeof(float));
		}

		Matrix4x4(
			const float m00, const float m01, const float m02, const float m03, // row 0
			const float m10, const float m11, const float m12, const float m13, // row 1
			const float m20, const float m21, const float m22, const float m23, // row 2
			const float m30, const float m31, const float m32, const float m33) // row 3
		{
			m[0][0] = m00; m[0][1] = m01; m[0][2] = m02; m[0][3] = m03;
			m[1][0] = m10; m[1][1] = m11; m[1][2] = m12; m[1][3] = m13;
			m[2][0] = m20; m[2][1] = m21; m[2][2] = m22; m[2][3] = m23;
			m[3][0] = m30; m[3][1] = m31; m[3][2] = m32; m[3][3] = m33;
		}

		Matrix4x4(const Vector4& row0, const Vector4& row1, const Vector4& row2, const Vector4& row3)
			: rows{ row0, row1, row2, row3 }
		{

		}

		const float* data() const
		{
			return m[0];
		}

		float* data() {
			return m[0];
		}

		Vector4& operator[](int index)
		{
			return rows[index];
		}

		const Vector4 operator[](int index) const
		{
			return rows[index];
		}

		// The inverse of this matrix (Read Only).
		Matrix4x4 inverse() const
		{
			return Matrix4x4::Inverse(*this);
		}

		// Returns the transpose of this matrix (Read Only).
		Matrix4x4 transpose() const
		{
			return Matrix4x4::Transpose(*this);
		}

		// Is this the identity matrix?
		bool isIdentity() const;


		// The determinant of the matrix.
		float determinant() const
		{
			return Matrix4x4::Determinant(*this);
		}

		// Returns the Inverse of mat.
		static Matrix4x4 Inverse(const Matrix4x4& mat);

		// The determinant of mat.
		static float Determinant(const Matrix4x4& mat);

		// Returns the transpose of mat.
		static Matrix4x4 Transpose(const Matrix4x4& mat);

		// Sets this matrix to a translation, rotation and scaling matrix.
		void SetTRS(const Vector3& pos, const Quaternion& q, const Vector3& s)
		{
			*this = Matrix4x4::TRS(pos, q, s);
		}

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


		friend Matrix4x4 operator*(const Matrix4x4& lhs, const Matrix4x4& rhs)
		{
			Matrix4x4 result;
			for (int i = 0; i < 4; i++)
			{
				for (int j = 0; j < 4; j++)
				{
					result.m[i][j] = lhs.m[i][0] * rhs.m[0][j] + lhs.m[i][1] * rhs.m[1][j] + lhs.m[i][2] * rhs.m[2][j] + lhs.m[i][3] * rhs.m[3][j];
				}
			}
			return result;
		}

		friend const Vector4 operator*(const Matrix4x4& lhs, const Vector4& rhs)
		{
			return Vector4(
				Vector4::Dot(lhs.rows[0], rhs),
				Vector4::Dot(lhs.rows[1], rhs),
				Vector4::Dot(lhs.rows[2], rhs),
				Vector4::Dot(lhs.rows[3], rhs));
		}


		void operator*=(const Matrix4x4& rhs)
		{
			for (int i = 0; i < 4; i++)
			{
				float f[4];
				for (int j = 0; j < 4; j++)
				{
					f[j] = m[i][0] * rhs.m[0][j] + m[i][1] * rhs.m[1][j] +
						m[i][2] * rhs.m[2][j] + m[i][3] * rhs.m[3][j];
				}
				for (int j = 0; j < 4; j++) {
					m[i][j] = f[j];
				}
			}
		}

		//friend Matrix4x4 operator*(const Matrix4x4& m, const float f);
		//{
		//    Matrix4x4 result(m);
		//    for (int i = 0; i < 4; ++i)
		//        for (int j = 0; j < 4; ++j)
		//            result.m[i][j] *= f;
		//    return result;
		//}

		friend bool operator ==(const Matrix4x4& lhs, const Matrix4x4& rhs)
		{
			return lhs.GetColumn(0) == rhs.GetColumn(0) && lhs.GetColumn(1) == rhs.GetColumn(1) && lhs.GetColumn(2) == rhs.GetColumn(2) && lhs.GetColumn(3) == rhs.GetColumn(3);
		}

		friend bool operator !=(const Matrix4x4& lhs, const Matrix4x4& rhs)
		{
			return !(lhs == rhs);
		}

		// Get a column of the matrix.
		Vector4 GetColumn(int i) const
		{
			return Vector4(m[0][i], m[1][i], m[2][i], m[3][i]);
		}

		// Returns a row of the matrix.
		Vector4 GetRow(int i) const
		{
			return rows[i];
		}

		// Sets a column of the matrix.
		void SetColumn(int i, const Vector4& v)
		{
			m[0][i] = v.x;
			m[1][i] = v.y;
			m[2][i] = v.z;
			m[3][i] = v.w;
		}

		// Sets a row of the matrix.
		void SetRow(int i, const Vector4& v)
		{
			m[i][0] = v.x;
			m[i][1] = v.y;
			m[i][2] = v.z;
			m[i][3] = v.w;
		}


		// Transforms a position by this matrix (generic).
		Vector3 MultiplyPoint(const float x, const float y, const float z) const
		{
			Vector3 result;
			result.x = m[0][0] * x + m[0][1] * y + m[0][2] * z + m[0][3];
			result.y = m[1][0] * x + m[1][1] * y + m[1][2] * z + m[1][3];
			result.z = m[2][0] * x + m[2][1] * y + m[2][2] * z + m[2][3];
			float num = m[3][0] * x + m[3][1] * y + m[3][2] * z + m[3][3];
			num = 1.f / num;
			result.x *= num;
			result.y *= num;
			result.z *= num;
			return result;
		}

		// Transforms a position by this matrix (generic).
		Vector3 MultiplyPoint(const Vector3& v) const
		{
			return MultiplyPoint(v.x, v.y, v.z);
		}

		// Transforms a position by this matrix (fast).
		Vector3 MultiplyPoint3x4(const float x, const float y, const float z) const
		{
			Vector3 result;
			result.x = m[0][0] * x + m[0][1] * y + m[0][2] * z + m[0][3];
			result.y = m[1][0] * x + m[1][1] * y + m[1][2] * z + m[1][3];
			result.z = m[2][0] * x + m[2][1] * y + m[2][2] * z + m[2][3];
			return result;
		}

		// Transforms a position by this matrix (fast).
		Vector3 MultiplyPoint3x4(const Vector3& v) const
		{
			return MultiplyPoint3x4(v.x, v.y, v.z);
		}

		// Transforms a direction by this matrix.
		Vector3 MultiplyVector(const float x, const float y, const float z) const
		{
			Vector3 result;
			result.x = m[0][0] * x + m[0][1] * y + m[0][2] * z;
			result.y = m[1][0] * x + m[1][1] * y + m[1][2] * z;
			result.z = m[2][0] * x + m[2][1] * y + m[2][2] * z;
			return result;
		}

		// Transforms a direction by this matrix.
		Vector3 MultiplyVector(const Vector3& v) const
		{
			return MultiplyVector(v.x, v.y, v.z);
		}


		// Creates a scaling matrix.
		static Matrix4x4 Scale(float scale)
		{
			return Scale(scale, scale, scale);
		}

		// Creates a scaling matrix.
		static Matrix4x4 Scale(const float x, const float y, const float z)
		{
			return Matrix4x4(
				x, 0, 0, 0,
				0, y, 0, 0,
				0, 0, z, 0,
				0, 0, 0, 1);
		}

		// Creates a scaling matrix.
		static Matrix4x4 Scale(const Vector3& scale)
		{
			return Scale(scale.x, scale.y, scale.z);
		}

		Quaternion ToRotation() const;

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

	public:
		
	};
}

#endif //Matrix4x4_hpp

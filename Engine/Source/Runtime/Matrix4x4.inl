#pragma once

namespace FishEngine
{
	inline Matrix4x4::Matrix4x4()
	{
		m[0][0] = 1; m[0][1] = 0; m[0][2] = 0; m[0][3] = 0;
		m[1][0] = 0; m[1][1] = 1; m[1][2] = 0; m[1][3] = 0;
		m[2][0] = 0; m[2][1] = 0; m[2][2] = 1; m[2][3] = 0;
		m[3][0] = 0; m[3][1] = 0; m[3][2] = 0; m[3][3] = 1;
	}

	inline Matrix4x4::Matrix4x4(const float mat[4][4])
	{
		memcpy(m, mat, 16 * sizeof(float));
	}

	inline Matrix4x4::Matrix4x4(const float m00, const float m01, const float m02, const float m03, /* row 0 */ const float m10, const float m11, const float m12, const float m13, /* row 1 */ const float m20, const float m21, const float m22, const float m23, /* row 2 */ const float m30, const float m31, const float m32, const float m33) // row 3
	{
		m[0][0] = m00; m[0][1] = m01; m[0][2] = m02; m[0][3] = m03;
		m[1][0] = m10; m[1][1] = m11; m[1][2] = m12; m[1][3] = m13;
		m[2][0] = m20; m[2][1] = m21; m[2][2] = m22; m[2][3] = m23;
		m[3][0] = m30; m[3][1] = m31; m[3][2] = m32; m[3][3] = m33;
	}

	inline Matrix4x4::Matrix4x4(const Vector4& row0, const Vector4& row1, const Vector4& row2, const Vector4& row3) : rows{ row0, row1, row2, row3 }
	{

	}

	inline Matrix4x4 Matrix4x4::inverse() const
	{
		return Matrix4x4::Inverse(*this);
	}

	inline Matrix4x4 Matrix4x4::transpose() const
	{
		return Matrix4x4::Transpose(*this);
	}

	inline float Matrix4x4::determinant() const
	{
		return Matrix4x4::Determinant(*this);
	}

	inline Matrix4x4 Matrix4x4::Transpose(const Matrix4x4& mat)
	{
		Matrix4x4 result;
		for (int i = 0; i < 4; ++i)
		{
			for (int j = 0; j < 4; ++j)
			{
				result.m[i][j] = mat.m[j][i];
			}
		}
		return result;
	}

	inline void Matrix4x4::SetTRS(const Vector3& pos, const Quaternion& q, const Vector3& s)
	{
		*this = Matrix4x4::TRS(pos, q, s);
	}

	inline Matrix4x4 Matrix4x4::Scale(float scale)
	{
		return Scale(scale, scale, scale);
	}

	inline Matrix4x4 Matrix4x4::Scale(const float x, const float y, const float z)
	{
		return Matrix4x4(
			x, 0, 0, 0,
			0, y, 0, 0,
			0, 0, z, 0,
			0, 0, 0, 1);
	}

	inline Matrix4x4 Matrix4x4::Scale(const Vector3& scale)
	{
		return Scale(scale.x, scale.y, scale.z);
	}

	inline Vector4 Matrix4x4::GetColumn(int i) const
	{
		return Vector4(m[0][i], m[1][i], m[2][i], m[3][i]);
	}

	inline Vector4 Matrix4x4::GetRow(int i) const
	{
		return rows[i];
	}

	inline void Matrix4x4::SetColumn(int i, const Vector4& v)
	{
		m[0][i] = v.x;
		m[1][i] = v.y;
		m[2][i] = v.z;
		m[3][i] = v.w;
	}

	inline void Matrix4x4::SetRow(int i, const Vector4& v)
	{
		m[i][0] = v.x;
		m[i][1] = v.y;
		m[i][2] = v.z;
		m[i][3] = v.w;
	}

	inline Vector3 Matrix4x4::MultiplyPoint(const float x, const float y, const float z) const
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

	inline Vector3 Matrix4x4::MultiplyPoint(const Vector3& v) const
	{
		return MultiplyPoint(v.x, v.y, v.z);
	}

	inline Vector3 Matrix4x4::MultiplyPoint3x4(const float x, const float y, const float z) const
	{
		Vector3 result;
		result.x = m[0][0] * x + m[0][1] * y + m[0][2] * z + m[0][3];
		result.y = m[1][0] * x + m[1][1] * y + m[1][2] * z + m[1][3];
		result.z = m[2][0] * x + m[2][1] * y + m[2][2] * z + m[2][3];
		return result;
	}

	inline Vector3 Matrix4x4::MultiplyPoint3x4(const Vector3& v) const
	{
		return MultiplyPoint3x4(v.x, v.y, v.z);
	}

	inline Vector3 Matrix4x4::MultiplyVector(const float x, const float y, const float z) const
	{
		Vector3 result;
		result.x = m[0][0] * x + m[0][1] * y + m[0][2] * z;
		result.y = m[1][0] * x + m[1][1] * y + m[1][2] * z;
		result.z = m[2][0] * x + m[2][1] * y + m[2][2] * z;
		return result;
	}

	inline FishEngine::Vector3 Matrix4x4::MultiplyVector(const Vector3& v) const
	{
		return MultiplyVector(v.x, v.y, v.z);
	}


	inline void Matrix4x4::operator*=(const Matrix4x4& rhs)
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

	inline Matrix4x4 operator*(const Matrix4x4& lhs, const Matrix4x4& rhs)
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

	inline Vector4 operator*(const Matrix4x4& lhs, const Vector4& rhs)
	{
		return Vector4(
			Vector4::Dot(lhs.rows[0], rhs),
			Vector4::Dot(lhs.rows[1], rhs),
			Vector4::Dot(lhs.rows[2], rhs),
			Vector4::Dot(lhs.rows[3], rhs));
	}

	inline bool operator ==(const Matrix4x4& lhs, const Matrix4x4& rhs)
	{
		return lhs.GetColumn(0) == rhs.GetColumn(0) && lhs.GetColumn(1) == rhs.GetColumn(1) && lhs.GetColumn(2) == rhs.GetColumn(2) && lhs.GetColumn(3) == rhs.GetColumn(3);
	}

	inline bool operator !=(const Matrix4x4& lhs, const Matrix4x4& rhs)
	{
		return !(lhs == rhs);
	}
}
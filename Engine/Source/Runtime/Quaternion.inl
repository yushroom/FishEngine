#pragma once

namespace FishEngine
{
	inline void Quaternion::setEulerAngles(const Vector3& angles)
	{
		*this = Quaternion::Euler(Internal_MakePositive(angles));
	}

	inline bool operator != (const Quaternion& lhs, const Quaternion& rhs)
	{
		return Quaternion::Dot(lhs, rhs) <= 0.999999f;
	}

	inline bool operator==(const Quaternion& lhs, const Quaternion& rhs)
	{
		return Quaternion::Dot(lhs, rhs) > 0.999999f;
	}

	inline float Quaternion::Dot(const Quaternion& a, const Quaternion& b)
	{
		return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
	}

	inline void Quaternion::NormalizeSelf()
	{
		float inv_len = 1.0f / std::sqrtf(x*x + y*y + z*z + w*w);
		x *= inv_len;
		y *= inv_len;
		z *= inv_len;
		w *= inv_len;
	}

	inline Quaternion Quaternion::inverse() const
	{
		return Inverse(*this);
	}

	inline void Quaternion::Set(float new_x, float new_y, float new_z, float new_w)
	{
		x = new_x;
		y = new_y;
		z = new_z;
		w = new_w;
	}

	inline Quaternion Quaternion::Euler(const Vector3& euler)
	{
		return Euler(euler.x, euler.y, euler.z);
	}

	inline Quaternion Quaternion::operator*=(const Quaternion & rhs)
	{
		(*this) = (*this) * rhs;
		return *this;
	}

	inline Quaternion Quaternion::operator*(const Quaternion & rhs) const
	{
		// [p.w*q.v + q.w*p.v + corss(p.v, q.v), p.w*q.w-dot(p.v, q.v)]
		return Quaternion(
			w * rhs.x + x * rhs.w + y * rhs.z - z * rhs.y,
			w * rhs.y + y * rhs.w + z * rhs.x - x * rhs.z,
			w * rhs.z + z * rhs.w + x * rhs.y - y * rhs.x,
			w * rhs.w - x * rhs.x - y * rhs.y - z * rhs.z);
	}

	inline Vector3 Quaternion::operator*(const Vector3 & point) const
	{
		float num = x * 2.f;
		float num2 = y * 2.f;
		float num3 = z * 2.f;
		float num4 = x * num;
		float num5 = y * num2;
		float num6 = z * num3;
		float num7 = x * num2;
		float num8 = x * num3;
		float num9 = y * num3;
		float num10 = w * num;
		float num11 = w * num2;
		float num12 = w * num3;
		Vector3 result;
		result.x = (1.f - (num5 + num6)) * point.x + (num7 - num12) * point.y + (num8 + num11) * point.z;
		result.y = (num7 + num12) * point.x + (1.f - (num4 + num6)) * point.y + (num9 - num10) * point.z;
		result.z = (num8 - num11) * point.x + (num9 + num10) * point.y + (1.f - (num4 + num5)) * point.z;
		return result;
	}


	inline Quaternion Quaternion::operator-() const
	{
		return Quaternion(-x, -y, -z, -w);
	}


	inline Quaternion operator* (Quaternion const & q, float scale)
	{
		return Quaternion(q.x*scale, q.y*scale, q.z*scale, q.w*scale);
	}

	inline Quaternion operator*(float scale, Quaternion const & q)
	{
		return Quaternion(q.x*scale, q.y*scale, q.z*scale, q.w*scale);
	}

	inline Quaternion & Quaternion::operator+=(const Quaternion& rhs)
	{
		x += rhs.x;
		y += rhs.y;
		z += rhs.z;
		w += rhs.w;
		return *this;
	}

	inline Quaternion & Quaternion::operator-=(const Quaternion& rhs)
	{
		x -= rhs.x;
		y -= rhs.y;
		z -= rhs.z;
		w -= rhs.w;
		return *this;
	}

	inline Quaternion operator + (const Quaternion& lhs, const Quaternion& rhs)
	{
		return Quaternion(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z, lhs.w + rhs.w);
	}

	inline Quaternion operator - (const Quaternion& lhs, const Quaternion& rhs)
	{
		return Quaternion(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z, lhs.w - rhs.w);
	}


	inline void Quaternion::SetFromToRotation(const Vector3& fromDirection, const Vector3& toDirection)
	{
		*this = Quaternion::FromToRotation(fromDirection, toDirection);
	}

	inline void Quaternion::SetLookRotation(const Vector3& view, const Vector3& up /*= Vector3::up*/)
	{
		*this = Quaternion::LookRotation(view, up);
	}

	inline Quaternion Quaternion::Slerp(const Quaternion& a, const Quaternion& b, float t)
	{
		return SlerpUnclamped(a, b, Mathf::Clamp01(t));
	}

	inline Quaternion Quaternion::Lerp(const Quaternion& a, const Quaternion& b, float t)
	{
		return LerpUnclamped(a, b, Mathf::Clamp01(t));
	}

	inline Quaternion Quaternion::Inverse(const Quaternion& rotation)
	{
		float num = 1.0f / Dot(rotation, rotation);
		return Quaternion(-rotation.x * num, -rotation.y * num, -rotation.z*num, rotation.w*num);
	}
}

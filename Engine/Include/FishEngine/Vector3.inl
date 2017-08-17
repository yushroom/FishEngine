#pragma once

namespace FishEngine
{
	/************************************************************************/
	/* Implementations                                                      */
	/************************************************************************/

	inline Vector3::Vector3(float x, float y, float z)
		: x(x), y(y), z(z)
	{
		Assert(!hasNaNs());
	}

	inline Vector3::Vector3(float x, float y)
		: x(x), y(y), z(0)
	{
		Assert(!hasNaNs());
	}

	inline Vector3::Vector3(float* array)
		: x(array[0]), y(array[1]), z(array[2])
	{
	}

	inline Vector3::Vector3()
		: Vector3(0, 0, 0)
	{

	}

	inline Vector3 Vector3::normalized() const
	{
		return Vector3::Normalize(*this);
	}

	inline float Vector3::magnitude() const
	{
		return ::std::sqrtf(x*x + y*y + z*z);
	}

	inline float Vector3::sqrMagnitude() const
	{
		return x*x + y*y + z*z;
	}

	inline Vector3 Vector3::Slerp(const Vector3& a, const Vector3& b, float t)
	{
		return SlerpUnclamped(a, b, Mathf::Clamp01(t));
	}

	inline void Vector3::Set(float newX, float newY, float newZ)
	{
		x = newX;
		y = newY;
		z = newZ;
	}

	inline void Vector3::Scale(const Vector3& scale)
	{
		x *= scale.x;
		y *= scale.y;
		z *= scale.z;
	}

	inline Vector3&    Vector3::operator=(const Vector3& v) { Assert(!v.hasNaNs()); x = v.x; y = v.y; z = v.z; return *this; }
	inline Vector3     Vector3::operator-() const { return Vector3(-x, -y, -z); }

	inline Vector3     Vector3::operator+(const Vector3& v) const { Assert(!v.hasNaNs()); return Vector3(x + v.x, y + v.y, z + v.z); }
	inline Vector3     Vector3::operator-(const Vector3& v) const { Assert(!v.hasNaNs()); return Vector3(x - v.x, y - v.y, z - v.z); }
	inline Vector3     Vector3::operator*(const Vector3& v) const { Assert(!v.hasNaNs()); return Vector3(x * v.x, y * v.y, z * v.z); }
	inline void        Vector3::operator+=(const Vector3& v) { Assert(!v.hasNaNs()); x += v.x; y += v.y; z += v.z; }
	inline void        Vector3::operator-=(const Vector3& v) { Assert(!v.hasNaNs()); x -= v.x; y -= v.y; z -= v.z; }

	inline Vector3     Vector3::operator+(const float f) const { Assert(!isnan(f)); return Vector3(x + f, y + f, z + f); }
	inline Vector3     Vector3::operator-(const float f) const { Assert(!isnan(f)); return Vector3(x - f, y - f, z - f); }
	inline Vector3     Vector3::operator*(const float f) const { Assert(!isnan(f)); return Vector3(x * f, y * f, z * f); }
	inline Vector3     Vector3::operator/(const float f) const { Assert(!isnan(f) && f != 0.f); return Vector3(x / f, y / f, z / f); }
	inline void        Vector3::operator+=(const float f) { Assert(!isnan(f)); x += f; y += f; z += f; }
	inline void        Vector3::operator-=(const float f) { Assert(!isnan(f)); x -= f; y -= f; z -= f; }
	inline void        Vector3::operator*=(const float f) { Assert(!isnan(f)); x *= f; y *= f; z *= f; }
	inline void        Vector3::operator/=(const float f) { Assert(!isnan(f)); x /= f; y /= f; z /= f; }

	inline Vector3  operator*(const float f, const Vector3& v) { Assert(!isnan(f) && !v.hasNaNs()); return Vector3(v.x * f, v.y * f, v.z * f); }
	inline Vector3  operator/(const float f, const Vector3& v) { Assert(!isnan(f) && !v.hasNaNs()); return Vector3(f / v.x, f / v.y, f / v.z); }


	inline float Vector3::Angle(const Vector3& from, const Vector3& to)
	{
		return ::std::acosf(Mathf::Clamp(Dot(Normalize(from), Normalize(to)), -1.0f, 1.0f)) * Mathf::Rad2Deg;
	}

	inline float Vector3::Magnitude(const Vector3& a)
	{
		return ::std::sqrtf(a.x * a.x + a.y * a.y + a.z * a.z);
	}

	inline float Vector3::SqrMagnitude(const Vector3& a)
	{
		return a.x * a.x + a.y * a.y + a.z * a.z;
	}

	inline Vector3 Vector3::ClampMagnitude(const Vector3& vector, float maxLength)
	{
		if (vector.sqrMagnitude() > maxLength * maxLength)
		{
			return vector.normalized() * maxLength;
		}
		return vector;
	}


	inline float Vector3::Dot(const Vector3& lhs, const Vector3& rhs)
	{
		return lhs.x*rhs.x + lhs.y*rhs.y + lhs.z*rhs.z;
	}

	inline Vector3 Vector3::Cross(const Vector3& lhs, const Vector3& rhs)
	{
		return Vector3(lhs.y * rhs.z - lhs.z * rhs.y, lhs.z * rhs.x - lhs.x * rhs.z, lhs.x * rhs.y - lhs.y * rhs.x);
	}

	inline float Vector3::Distance(const Vector3& a, const Vector3& b)
	{
		return Magnitude(a - b);
	}

	inline float Vector3::DistanceSquared(const Vector3& a, const Vector3& b)
	{
		return SqrMagnitude(a - b);
	}

	// a*(1-t) + b*t
	inline Vector3 Vector3::Lerp(const Vector3& a, const Vector3& b, float t)
	{
		t = Mathf::Clamp01(t);
		return Vector3(a.x + (b.x - a.x) * t, a.y + (b.y - a.y) * t, a.z + (b.z - a.z) * t);
	}

	// a*(1-t) + b*t
	inline Vector3 Vector3::LerpUnClamped(const Vector3& a, const Vector3& b, float t)
	{
		return Vector3(a.x + (b.x - a.x) * t, a.y + (b.y - a.y) * t, a.z + (b.z - a.z) * t);
	}

	inline Vector3 Vector3::Max(const Vector3& lhs, const Vector3& rhs)
	{
		return Vector3(std::max(lhs.x, rhs.x), std::max(lhs.y, rhs.y), std::max(lhs.z, rhs.z));
	}

	inline Vector3 Vector3::Min(const Vector3& lhs, const Vector3& rhs)
	{
		return Vector3(std::min(lhs.x, rhs.x), std::min(lhs.y, rhs.y), std::min(lhs.z, rhs.z));
	}

	inline Vector3 Vector3::Normalize(const Vector3& value)
	{
		float num = value.magnitude();
		if (num > 1E-05f)
		{
			return value / num;
		}
		return Vector3::zero;
	}

	inline void Vector3::Normalize()
	{
		float len = magnitude();
		if (len > 1e-5f) {
			x /= len;
			y /= len;
			z /= len;
		}
		else {
			x = y = z = 0;
		}
	}

	inline Vector3 Vector3::Scale(const Vector3& a, const Vector3& b)
	{
		return Vector3(a.x*b.x, a.y*b.y, a.z*b.z);
	}
}
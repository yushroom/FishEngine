#ifndef Quaternion_hpp
#define Quaternion_hpp

#include "Vector3.hpp"

namespace FishEngine
{

	class FE_EXPORT Quaternion
	{
	public:
		
		union
		{
			struct { float x, y, z, w; };
			float m[4];
		};

		static constexpr float kEpsilon = 1E-06f;

		static const Quaternion identity;

		Quaternion(float x, float y, float z, float w)
			: x(x), y(y), z(z), w(w)
		{
			NormalizeSelf();
		}

		Quaternion() : Quaternion(0, 0, 0, 1) {}

		// Returns the euler angle representation of the rotation.
		Vector3 eulerAngles() const;

		// Set the euler angle representation of the rotation.
		void setEulerAngles(const Vector3& angles)
		{
			*this = Quaternion::Euler( Internal_MakePositive( angles ) );
		}

		float operator[](int index) const
		{
			return m[index];
		}

		float& operator[](int index)
		{
			return m[index];
		}

		void Set(float new_x, float new_y, float new_z, float new_w)
		{
			x = new_x;
			y = new_y;
			z = new_z;
			w = new_w;
		}

		// Creates a rotation which rotates angle degrees around axis.
		static Quaternion AngleAxis(const float angle, const Vector3& axis);

		// Converts a rotation to angle-axis representation (angles in degrees).
		void ToAngleAxis(float* angle, Vector3* axis);

		// Creates a rotation which rotates from fromDirection to toDirection.
		// Usually you use this to rotate a transform so that one of its axes eg. the y-axis - follows a target direction toDirection in world space.
		static Quaternion FromToRotation(const Vector3& fromDirection, const Vector3& toDirection);

		// Creates a rotation which rotates from fromDirection to toDirection.
		void SetFromToRotation(const Vector3& fromDirection, const Vector3& toDirection)
		{
			*this = Quaternion::FromToRotation(fromDirection, toDirection);
		}

		/**
		 * Creates a rotation with the specified forward and upwards directions.
		 *
		 * @param const Vector3 & forward The direction to look in.
		 * @param const Vector3 & upwards The vector that defines in which direction up is.
		 * @return FishEngine::Quaternion 
		 */
		static Quaternion LookRotation(const Vector3& forward, const Vector3& upwards = Vector3::up);

		/**
		 * Creates a rotation with the specified forward and upwards directions.
		 *
		 * @param const Vector3 & view The direction to look in.
		 * @param const Vector3 & up The vector that defines in which direction up is.
		 * @return void
		 */
		void SetLookRotation(const Vector3& view, const Vector3& up = Vector3::up)
		{
			*this = Quaternion::LookRotation(view, up);
		}


		// Spherically interpolates between a and b by t. The parameter t is clamped to the range [0, 1].
		static Quaternion Slerp(const Quaternion& a, const Quaternion& b, float t);

		// >Spherically interpolates between a and b by t. The parameter t is not clamped.
		static Quaternion SlerpUnclamped(const Quaternion& a, const Quaternion& b, float t);

		// Interpolates between a and b by t and normalizes the result afterwards. The parameter t is clamped to the range [0, 1].
		//static Quaternion Lerp(const Quaternion& a, const Quaternion& b, float t);

		// Interpolates between a and b by t and normalizes the result afterwards.The parameter t is not clamped.
		//static Quaternion LerpUnclamped(const Quaternion& a, const Quaternion& b, float t);

		// Rotates a rotation from towards to.
		static Quaternion RotateTowards(const Quaternion& from, const Quaternion& to, float maxDegreesDelta);

		// Returns the Inverse of rotation.
		static Quaternion Inverse(const Quaternion& rotation)
		{
			float num = 1.0f / Dot(rotation, rotation);
			return Quaternion(-rotation.x * num, -rotation.y * num, -rotation.z*num, rotation.w*num);
		}

		// Returns a rotation that rotates z degrees around the z axis, x degrees around the x axis, and y degrees around the y axis(in that order).
		static Quaternion Euler(float x, float y, float z);

		// Returns a rotation that rotates z degrees around the z axis, x degrees around the x axis, and y degrees around the y axis (in that order).
		static Quaternion Euler(const Vector3& euler)
		{
			return Euler(euler.x, euler.y, euler.z);
		}


		Quaternion operator *(const Quaternion & rhs) const
		{
			// [p.w*q.v + q.w*p.v + corss(p.v, q.v), p.w*q.w-dot(p.v, q.v)]
			return Quaternion(
							  w * rhs.x + x * rhs.w + y * rhs.z - z * rhs.y,
							  w * rhs.y + y * rhs.w + z * rhs.x - x * rhs.z,
							  w * rhs.z + z * rhs.w + x * rhs.y - y * rhs.x,
							  w * rhs.w - x * rhs.x - y * rhs.y - z * rhs.z);
		}

		Quaternion operator *=(const Quaternion & rhs)
		{
			(*this) = (*this) * rhs;
			return *this;
		}
		
		Vector3 operator *(const Vector3 & point) const;
		

		friend bool operator==(const Quaternion& lhs, const Quaternion& rhs)
		{
			return Quaternion::Dot(lhs, rhs) > 0.999999f;
		}

		friend bool operator!=(const Quaternion& lhs, const Quaternion& rhs)
		{
			return Quaternion::Dot(lhs, rhs) <= 0.999999f;
		}

		// The dot product between two rotations.
		static float Dot(const Quaternion& a, const Quaternion& b)
		{
			return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
		}

		// Returns the angle in degrees between two rotations a and b.
		static float Angle(const Quaternion& a, const Quaternion& b)
		{
			float f = Quaternion::Dot(a, b);
			return Mathf::Acos(Mathf::Min(Mathf::Abs(f), 1.f)) * 2.f * 57.29578f;
		}

		void NormalizeSelf();

		// Returns the Inverse of this rotation.
		Quaternion inverse() const
		{
			return Inverse(*this);
		}

	private:
		static Vector3 Internal_MakePositive(Vector3 euler)
		{
			float num = -0.005729578f;
			float num2 = 360.f + num;
			if (euler.x < num)
			{
				euler.x += 360.f;
			}
			else if (euler.x > num2)
			{
				euler.x -= 360.f;
			}
			if (euler.y < num)
			{
				euler.y += 360.f;
			}
			else if (euler.y > num2)
			{
				euler.y -= 360.f;
			}
			if (euler.z < num)
			{
				euler.z += 360.f;
			}
			else if (euler.z > num2)
			{
				euler.z -= 360.f;
			}
			return euler;
		}
	};
}

#endif // Quaternion_hpp

#include "Quaternion.hpp"
#include "Matrix4x4.hpp"

namespace FishEngine {

	const Quaternion Quaternion::identity(0, 0, 0, 1);

	Vector3 Quaternion::eulerAngles() const
	{
		// http://www.euclideanspace.com/maths/geometry/rotations/conversions/quaternionToEuler/
		// https://en.wikipedia.org/wiki/Conversion_between_quaternions_and_Euler_angles#Quaternion_to_Euler_Angles_Conversion
		float heading, attitude, bank;  // y, z, x
		float sqx = x*x;
		float sqy = y*y;
		float sqz = z*z;
		float sqw = w*w;
		float unit = sqx + sqy + sqz + sqw;
		float test = x*y + z*w;
		if (test > 0.499f * unit) {
			heading = 2 * atan2(x, w);
			attitude = Mathf::PI / 2.f;
			bank = 0.0f;
		}
		else {
			if (test < -0.499f * unit) {
				heading = -2 * atan2(x, w);
				attitude = -Mathf::PI / 2;
				bank = 0.f;
			}
			else {
				heading = atan2(2 * (y*w - x*z), sqx - sqy - sqz + sqw);
				attitude = asin(2 * test / unit);
				bank = atan2(2 * (x*w - y*z), -sqx + sqy - sqz + sqw);
			}
		}
		return Vector3(bank, heading, attitude) * Mathf::Rad2Deg;
		//return Vector3(pitch(), yaw(), roll());
	}


	float Quaternion::Angle(const Quaternion& a, const Quaternion& b)
	{
		float f = Quaternion::Dot(a, b);
		return Mathf::Acos(Mathf::Min(Mathf::Abs(f), 1.f)) * 2.f * 57.29578f;
	}


	FishEngine::Vector3 Quaternion::Internal_MakePositive(Vector3 euler)
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


	FishEngine::Quaternion Quaternion::AngleAxis(const float angle, const Vector3& axis)
	{
		float length = axis.magnitude();
		if (length <= Quaternion::kEpsilon)
		{
			return Quaternion::identity;
		}

		//auto a = axis.normalized();
		Quaternion result;

		float angleHalf = Mathf::Radians(angle) * 0.5f;

		float s = Mathf::Sin(angleHalf) / length;

		result.w = Mathf::Cos(angleHalf);
		result.x = axis.x * s;
		result.y = axis.y * s;
		result.z = axis.z * s;
		return result;
	}


	void Quaternion::ToAngleAxis(float* angle, Vector3* axis)
	{
		*angle = Mathf::Degrees( 2.0f * std::acosf(w) );
		if (Mathf::CompareApproximately(*angle, 0.0f))
		{
			*axis = Vector3::xAxis;
		}
		else
		{
			float k = 1.0f / sqrtf(1.0f - w*w);
			axis->Set(x*k, y*k, z*k);
		}
	}


	void SinCos(float degrees, float *s, float * c)
	{
		float rad = Mathf::Radians(degrees);
		*s = std::sinf(rad);
		*c = std::cosf(rad);
	}


	Quaternion Quaternion::Euler(float x, float y, float z)
	{
#if 0
		// http://www.euclideanspace.com/maths/geometry/rotations/conversions/eulerToQuaternion/index.htm
        Quaternion result;
        auto rad = euler * (Mathf::Deg2Rad * 0.5f);
        //auto c = Vector3(cos(rad.x), cos(rad.y), cos(rad.z));
        //auto s = Vector3(sin(rad.x), sin(rad.y), sin(rad.z));
        //result.w = c.x * c.y * c.z + s.x * s.y * s.z;
        //result.x = s.x * c.y * c.z - c.x * s.y * s.z;
        //result.y = c.x * s.y * c.z + s.x * c.y * s.z;
        //result.z = c.x * c.y * s.z - s.x * s.y * c.z;

        // heading, attitude, bank -> y,z,x
        auto c = Vector3(cos(rad.y), cos(rad.z), cos(rad.x));
        auto s = Vector3(sin(rad.y), sin(rad.z), sin(rad.x));
        result.w = c.x * c.y * c.z - s.x * s.y * s.z;
        result.x = s.x * s.y * c.z + c.x * c.y * s.z;
        result.y = s.x * c.y * c.z + c.x * s.y * s.z;
        result.z = c.x * s.y * c.z - s.x * c.y * s.z;
        return result;
#elif 0
		return AngleAxis(y, Vector3{0,1,0}) * AngleAxis(x, Vector3{1, 0, 0}) * AngleAxis(z, Vector3{0, 0, 1});
#else
		float sx, cx;
		SinCos(x / 2.0f, &sx, &cx);

		float sy, cy;
		SinCos(y / 2.0f, &sy, &cy);

		float sz, cz;
		SinCos(z / 2.0f, &sz, &cz);

		Quaternion qx(sx, 0.0f, 0.0f, cx);
		Quaternion qy(0.0f, sy, 0.0f, cy);
		Quaternion qz(0.0f, 0.0f, sz, cz);

		// TODO
		return qy * qx * qz;
#endif
	}


	Quaternion Quaternion::FromToRotation(const Vector3& fromDirection, const Vector3& toDirection)
	{
		auto from = fromDirection.normalized();
		auto to = toDirection.normalized();
		const float cos_theta = Vector3::Dot(from, to);
		if (cos_theta == 1.0f)
		{
			return Quaternion::identity;
		}
		else
		{
			if (cos_theta == -1.0f)
			{
				return Quaternion(1, 0, 0, 0);
			}
			else
			{
				Vector3 w = Vector3::Cross(from, to);
				return Quaternion(w.x, w.y, w.z, 1 + cos_theta);
			}
		}
	}


	FishEngine::Quaternion Quaternion::LookRotation(const Vector3& forward, const Vector3& upwards /*= Vector3::up*/)
	{
		Vector3 z = forward;
		float length = z.magnitude();
		if (length < Vector3::kEpsilon)		// new forward is zero, do nothing
			return Quaternion::identity;
		z /= length;

		do {
			Vector3 x = Vector3::Cross(upwards, z);
			length = x.magnitude();
			if (length < Vector3::kEpsilon)
				break;
			x /= length;

			Vector3 y(Vector3::Cross(z, x));
			if (!Mathf::CompareApproximately(y.sqrMagnitude(), 1.0F))
				break;

			Matrix4x4 m;
			m[0][0] = x[0];	m[0][1] = y[0];	m[0][2] = z[0];
			m[1][0] = x[1];	m[1][1] = y[1];	m[1][2] = z[1];
			m[2][0] = x[2];	m[2][1] = y[2];	m[2][2] = z[2];
			return m.ToRotation();
		} while (false);

		return Quaternion::FromToRotation(Vector3::zAxis, z);
	}


	FishEngine::Quaternion Quaternion::SlerpUnclamped(const Quaternion& a, const Quaternion& b, float t)
	{
#if 0
		// 3D Math Primer for Graphics and Game Development, 2nd
		// p262, list 8.3

		// Compute the "cosine of the angle" between the quaternions
		float cosOmega = Quaternion::Dot(a, b);

		// If negative dot, negate one of the input
		// quaternions, to take the shorter 4D "arc"
		Quaternion end = b;
		if (cosOmega < 0.0f)
		{
			end.w = -b.w;
			end.x = -b.x;
			end.y = -b.y;
			end.z = -b.z;
			cosOmega = -cosOmega;
		}

		// Check if they are very close together, to protect
		// against divide-by-zero
		float k0, k1;
		if (cosOmega > 0.9999f)
		{
			// Very close - just use linear interpolation
			k0 = 1.0f - t;
			k1 = t;
		}
		else
		{
			// Compute the sin of the angle using the
			// trig identity sin^2(omega)+cos^2(omega) = 1
			float sinOmega = sqrtf(1.0f - cosOmega*cosOmega);
			// Compute the angle from its sine and cosine
			float omega = atan2(sinOmega, cosOmega);
			// Compute inverse of denominator, so we only have
			// to divide once
			float oneOverSinOmega = 1.0f / sinOmega;
			// Compute interpolation parameters
			k0 = sin((1.0f - t) * omega) * oneOverSinOmega;
			k1 = sin(t * omega) * oneOverSinOmega;
		}
		Quaternion q;
		q.w = a.w*k0 + end.w*k1;
		q.x = a.x*k0 + end.x*k1;
		q.y = a.y*k0 + end.y*k1;
		q.z = a.z*k0 + end.z*k1;
		return q;
#else
		// from Assimp

		float cosTheta = Quaternion::Dot(a, b);

		// adjust signs (if necessary)
		Quaternion end = b;
		if (cosTheta < 0.0f)
		{
			cosTheta = -cosTheta;
			end.x = -end.x;
			end.y = -end.y;
			end.z = -end.z;
			end.w = -end.w;
		}

		float sclp, sclq;
		if ((1.0f - cosTheta) > 0.0001f) {
			// Standard case (slerp)
			float theta, sinTheta;
			theta    = std::acos(cosTheta);
			sinTheta = std::sin(theta);
			sclp     = std::sin((1.0f - t) * theta) / sinTheta;
			sclq     = std::sin(t * theta) / sinTheta;
		}
		else {
			// Very close, do LERP (because it's faster)
			sclp = 1.0f - t;
			sclq = t;
		}

		return Quaternion(
			sclp * a.x + sclq * end.x,
			sclp * a.y + sclq * end.y,
			sclp * a.z + sclq * end.z,
			sclp * a.w + sclq * end.w);
#endif
	}


	FishEngine::Quaternion Quaternion::LerpUnclamped(const Quaternion& a, const Quaternion& b, float t)
	{
		if (Quaternion::Dot(a, b) < 0.0f)
		{
			return Quaternion(
				a.x + t*(-b.x - a.x),
				a.y + t*(-b.y - a.y),
				a.z + t*(-b.z - a.z),
				a.w + t*(-b.w - a.w));
		}
		else
		{
			return Quaternion(
				a.x + t*(b.x - a.x),
				a.y + t*(b.y - a.y),
				a.z + t*(b.z - a.z),
				a.w + t*(b.w - a.w));
		}
	}


	FishEngine::Quaternion Quaternion::RotateTowards(const Quaternion& from, const Quaternion& to, float maxDegreesDelta)
	{
		float num = Quaternion::Angle(from, to);
		if (num == 0.f)
		{
			return to;
		}
		float t = Mathf::Min(1.f, maxDegreesDelta / num);
		return Quaternion::SlerpUnclamped(from, to, t);
	}
}

#include "Vector3.hpp"
#include "Time.hpp"
#include "Quaternion.hpp"

using  namespace FishEngine;

const Vector3 Vector3::back		( 0,  0, -1);
const Vector3 Vector3::down		( 0, -1,  0);
const Vector3 Vector3::forward	( 0,  0,  1);
const Vector3 Vector3::left		(-1,  0,  0);
const Vector3 Vector3::one		( 1,  1,  1);
const Vector3 Vector3::right	( 1,  0,  0);
const Vector3 Vector3::up		( 0,  1,  0);
const Vector3 Vector3::zero		( 0,  0,  0);

const Vector3 Vector3::xAxis	( 1,  0,  0);
const Vector3 Vector3::yAxis	( 0,  1,  0);
const Vector3 Vector3::zAxis	( 0,  0,  1);

Vector3 Vector3::Project(const Vector3& vector, const Vector3& onNormal)
{
	float num = Dot(onNormal, onNormal);
	if (num < Mathf::Epsilon)
	{
		return Vector3::zero;
	}
	return onNormal * Vector3::Dot(vector, onNormal) / num;
}

Vector3 Vector3::ProjectOnPlane(const Vector3& vector, const Vector3& planeNormal)
{
	return vector - Vector3::Project(vector, planeNormal);
}

Vector3 Vector3::SmoothDamp(
	const Vector3& current,
	const Vector3& target,
	Vector3& currentVelocity/*ref*/,
	float smoothTime,
	float maxSpeed /*= Mathf::PositiveInfinity*/)
{
	float deltaTime = Time::deltaTime();
	return Vector3::SmoothDamp(current, target, currentVelocity, smoothTime, maxSpeed, deltaTime);
}

Vector3 Vector3::SmoothDamp(
	const Vector3& current,
	const Vector3& target,
	Vector3& currentVelocity/*ref*/,
	float smoothTime,
	float maxSpeed,
	float deltaTime)
{
	smoothTime = Mathf::Max(0.0001f, smoothTime);
	float num = 2.f / smoothTime;
	float num2 = num * deltaTime;
	float d = 1.f / (1.f + num2 + 0.48f * num2 * num2 + 0.235f * num2 * num2 * num2);
	Vector3 vector = current - target;
	Vector3 vector2 = target;
	float maxLength = maxSpeed * smoothTime;
	vector = Vector3::ClampMagnitude(vector, maxLength);
	Vector3 newTarget = current - vector;
	Vector3 vector3 = (currentVelocity + num * vector) * deltaTime;
	currentVelocity = (currentVelocity - num * vector3) * d;
	Vector3 vector4 = newTarget + (vector + vector3) * d;
	if (Vector3::Dot(vector2 - current, vector4 - vector2) > 0.f)
	{
		vector4 = vector2;
		currentVelocity = (vector4 - vector2) / deltaTime;
	}
	return vector4;
}

// return a normalized vector w such that w is perpendicular with v
Vector3 GetOrthoNormalVector(const Vector3& v)
{
	Vector3 result;
	float abs_x = fabsf(v.x);
	float abs_y = fabsf(v.y);
	float abs_z = fabsf(v.z);

	if (abs_z > abs_x && abs_z > abs_y)  // z is max
	{
		float a = v.y*v.y + v.z*v.z;
		float k = 1.0f / sqrtf(a);
		result.x = 0;
		result.y = -v.z*k;
		result.z = v.y*k;
	}
	else
	{
		float a = v.x*v.x + v.y*v.y;
		float k = 1.0f / sqrtf(a);
		result.x = -v.y*k;
		result.y = v.x*k;
		result.z = 0;
	}
	return result;
}


Vector3 Vector3::SlerpUnclamped(const Vector3& a, const Vector3& b, float t)
{
	float len1 = a.magnitude();
	float len2 = b.magnitude();

	if (len1 < Vector3::kEpsilon || len2 < Vector3::kEpsilon)
	{
		return Lerp(a, b, t);
	}

	float lerpedLength = Mathf::Lerp(len1, len2, t);
	float dot = Vector3::Dot(a, b) / (len1 * len2);
	if (dot > 1.0f - Vector3::kEpsilon)	// almost the same
	{
		return Lerp(a, b, t);
	}
	else if (dot < -1.0f + Vector3::kEpsilon) // almost opposite
	{
		Vector3 a_normalized = a / len1;
		Vector3 axis = GetOrthoNormalVector(a_normalized);
		Quaternion rotation = Quaternion::AngleAxis(Mathf::Degrees(t), axis);
		Vector3 result = rotation * a_normalized;
		return result * lerpedLength;
	}
	else
	{
		Vector3 axis = Vector3::Cross(a, b);
		Vector3 a_normalized = a / len1;
		axis.Normalize();
		float angle = std::acosf(dot) * t;
		Quaternion rotation = Quaternion::AngleAxis(angle, axis);
		Vector3 result = rotation * a_normalized;
		return result * lerpedLength;
	}
}

void Vector3::OrthoNormalize(Vector3 * normal, Vector3 * tangent)
{
	float length = Magnitude(*normal);
	if (length > Vector3::kEpsilon)
	{
		*normal /= length;
	}
	else
	{
		*normal = Vector3(1.0F, 0.0F, 0.0F);
	}

	float dot = Dot(*normal, *tangent);
	*tangent -= dot * *normal;
	length = Magnitude(*tangent);
	if (length < Vector3::kEpsilon)
	{
		*tangent = GetOrthoNormalVector(*normal);
	}
	else
	{
		*tangent /= length;
	}
}


void Vector3::OrthoNormalize(
	Vector3 * normal,
	Vector3 * tangent,
	Vector3 * binormal)
{
	float length = Magnitude(*normal);
	if (length > Vector3::kEpsilon)
	{
		*normal /= length;
	}
	else
	{
		*normal = Vector3(1.0F, 0.0F, 0.0F);
	}

	float dot0 = Dot(*normal, *tangent);
	*tangent -= dot0 * *normal;
	length = Magnitude(*tangent);
	if (length > Vector3::kEpsilon)
	{
		*tangent /= length;
	}
	else
	{
		*tangent = GetOrthoNormalVector(*normal);
	}

	float dot1 = Dot(*tangent, *binormal);
	dot0 = Dot(*normal, *binormal);
	*binormal -= dot0 * *normal + dot1 * *tangent;
	length = Magnitude(*binormal);
	if (length > Vector3::kEpsilon)
	{
		*binormal /= length;
	}
	else
	{
		*binormal = Cross(*normal, *tangent);
	}
}

Vector3 Vector3::RotateTowards(
	const Vector3& current,
	const Vector3& target,
	float maxRadiansDelta,
	float maxMagnitudeDelta)
{
	float length1 = Magnitude(current);
	float length2 = Magnitude(target);

	if (length1 < Vector3::kEpsilon || length2 < Vector3::kEpsilon)
	{
		return MoveTowards(current, target, maxMagnitudeDelta);
	}

	Vector3 a_normalized = current / length1;
	Vector3 b_normalized = target / length2;

	float dot = Dot(a_normalized, b_normalized);
	if (dot > 1.0F - Vector3::kEpsilon) // almost the same
	{
		return MoveTowards(current, target, maxMagnitudeDelta);
	}

	Vector3 axis;
	float angle = 0;

	if (dot < -1.0F + Vector3::kEpsilon) // almost opposite
	{
		axis = GetOrthoNormalVector(a_normalized);
		angle = maxRadiansDelta;
	}
	else
	{
		angle = acos(dot);
		angle = std::min(maxRadiansDelta, angle);
		axis = Cross(a_normalized, b_normalized);
	}
	Quaternion rotation = Quaternion::AngleAxis(angle, axis);
	Vector3 rotated = rotation * a_normalized;
	rotated *= Mathf::MoveTowards(length1, length2, maxMagnitudeDelta);
	return rotated;
}

Vector3 Vector3::MoveTowards(
	const Vector3& current,
	const Vector3& target,
	float maxDistanceDelta)
{
	Vector3 a = target - current;
	float magnitude = a.magnitude();
	if (magnitude <= maxDistanceDelta || magnitude == 0.f)
	{
		return target;
	}
	return current + a / magnitude * maxDistanceDelta;
}
#include "Vector3.hpp"

namespace FishEngine {

    const Vector3 Vector3::back(0, 0, -1);
    const Vector3 Vector3::down(0, -1, 0);
    const Vector3 Vector3::forward(0, 0, 1);
    const Vector3 Vector3::left(-1, 0, 0);
    const Vector3 Vector3::one(1, 1, 1);
    const Vector3 Vector3::right(1, 0, 0);
    const Vector3 Vector3::up(0, 1, 0);
    const Vector3 Vector3::zero(0, 0, 0);

    const Vector4 Vector4::one(1, 1, 1, 1);
    const Vector4 Vector4::zero(0, 0, 0, 0);

    Vector3 Vector3::Project(const Vector3& vector, const Vector3& onNormal)
    {
        float num = Dot(onNormal, onNormal);
        if (num < 1e-5f)    // TODO: use Mathf::Epsilon
        {
            return Vector3::zero;
        }
        return onNormal * Vector3::Dot(vector, onNormal) / num;
    }

    Vector3 Vector3::ProjectOnPlane(const Vector3& vector, const Vector3& planeNormal)
    {
        return vector - Vector3::Project(vector, planeNormal);
    }

    FishEngine::Vector3 Vector3::SmoothDamp(const Vector3& current, const Vector3& target, Vector3& currentVelocity/*ref*/, float smoothTime, float maxSpeed, float deltaTime)
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

    FishEngine::Vector3 Vector3::MoveTowards(const Vector3& current, const Vector3& target, float maxDistanceDelta)
    {
        Vector3 a = target - current;
        float magnitude = a.magnitude();
        if (magnitude <= maxDistanceDelta || magnitude == 0.f)
        {
            return target;
        }
        return current + a / magnitude * maxDistanceDelta;
    }

}
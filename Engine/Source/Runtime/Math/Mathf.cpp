#include <FishEngine/Mathf.hpp>
#include <FishEngine/Time.hpp>


float FishEngine::Mathf::SmoothDamp(float current, float target, float & /*ref*/ currentVelocity, float smoothTime, float maxSpeed /*= Mathf::Infinity*/)
{
	float deltaTime = Time::deltaTime();
	return Mathf::SmoothDamp(current, target, currentVelocity, smoothTime, maxSpeed, deltaTime);
}

float FishEngine::Mathf::SmoothDampAngle(float current, float target, float& currentVelocity, float smoothTime, float maxSpeed /*= Mathf::Infinity*/)
{
	float deltaTime = Time::deltaTime();
	return Mathf::SmoothDampAngle(current, target, currentVelocity, smoothTime, maxSpeed, deltaTime);
}

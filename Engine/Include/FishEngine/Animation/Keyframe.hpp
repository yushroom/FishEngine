#pragma once

namespace FishEngine
{
	// A single keyframe that can be injected into an animation curve.
	struct Keyframe
	{
	public:
		Keyframe(float time, float value)
			: time(time), value(value)
		{
		}

		Keyframe(float time, float value, float inTangent, float outTangent)
			: time(time), value(value), inTangent(inTangent), outTangent(outTangent)
		{
		}


		// The time of the keyframe.
		// In a 2D graph you could think of this as the x-value.
		float time;


		// The value of the curve at keyframe.
		// In a 2D graph you could think of this as the y-value.
		float value;

		// Describes the tangent when approaching this point from the previous point in the curve.
		// The angle needs to be expressed in radians.
		// The tangent is really just the gradient of the slope, as in "change in y / change in x"
		float inTangent = 0;

		// Describes the tangent when leaving this point towards the next point in the curve.
		// The angle needs to be expressed in radians.
		// The tangent is really just the gradient of the slope, as in "change in y / change in x"
		float outTangent = 0;


		//int tangentMode = 0;
	};
}
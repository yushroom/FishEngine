#pragma once

namespace FishEditor
{
	enum class SerializedPropertyType
	{
		Generic = -1,
		Integer,
		Boolean,
		Float,
		String,
		Color,
		ObjectReference,
		LayerMask,
		Enum,
		Vector2,
		Vector3,
		Vector4,
		Rect,
		ArraySize,
		Character,
		AnimationCurve,
		Bounds,
		Gradient,
		Quaternion
	};
}

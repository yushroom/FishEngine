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
		Quaternion,
		
		Int8,
		Uint8,
		Int16,
		Uint16,
		Int32,
		Uint32,
		Int64,
		Uint64,
		Double,
	};
}

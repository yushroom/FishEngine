#pragma once

#include "Object.hpp"
#include "ReflectClass.hpp"

namespace FishEngine
{
	class Avatar : public Object
	{
	public:
		InjectClassName(Avatar);
		std::map<std::string, int> m_boneToIndex;
		std::map<int, std::string> m_indexToBone;
		std::vector<Matrix4x4> m_matrixPalette;
	};

	class Meta(NonSerializable) MatrixPalette
	{
	public:
		mutable std::vector<Matrix4x4> m_matrixPalette;
		void UpdateMatrixPalette() const;
	};
}

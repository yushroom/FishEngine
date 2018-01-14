#pragma once

#include "FishEditor.hpp"

namespace FishEditor
{
	class PropertyModification
	{
	public:
		FishEngine::ObjectPtr	target;
		std::string				propertyPath;
		std::string				value;
		FishEngine::ObjectPtr	objectReference;
	};
}

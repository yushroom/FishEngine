#pragma once

#include <cstdint>

namespace FishEditor
{
	class ProjectSettings
	{
	public:
		ProjectSettings() = delete;

		static uint32_t NextFileID();
	};
}
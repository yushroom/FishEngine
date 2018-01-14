#pragma once

#include "AssetImporter.hpp"

namespace FishEditor
{
	class NativeFormatImporter : public AssetImporter
	{
	public:
		InjectClassName(NativeFormatImporter);

		NativeFormatImporter() = default;
		NativeFormatImporter( const NativeFormatImporter&) = delete;
		NativeFormatImporter& operator=(const NativeFormatImporter&) = delete;

		virtual ~NativeFormatImporter() = default;

		FishEngine::ObjectPtr Load(FishEngine::Path const & path);
	};
}
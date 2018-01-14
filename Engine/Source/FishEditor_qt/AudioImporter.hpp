#pragma once

#include "AssetImporter.hpp"
#include "AudioImporterSampleSettings.hpp"

namespace FishEditor
{
	class AudioImporter : public AssetImporter
	{
	public:
		InjectClassName(AudioImporter);

		FishEngine::AudioClipPtr Import(FishEngine::Path const & path);

		AudioImporterSampleSettings & defaultSampleSettings() { return m_defaultSampleSettings; }

		int originalSize() const { return m_origSize; }
		int importedSize() const { return m_compSize; }

	private:

		// The default sample settings for the AudioClip importer.
		AudioImporterSampleSettings m_defaultSampleSettings;
		
		//Force this clip to mono ?
		bool m_forceToMono = false;

		// Corresponding to the "Load In Background" flag in the AudioClip inspector, when this flag is set, the loading of the clip will happen delayed without blocking the main thread.
		bool m_loadInBackground = false;

		// Preloads audio data of the clip when the clip asset is loaded.
		// When this flag is off, scripts have to call AudioClip.LoadAudioData() to load the data before the clip can be played.
		// Properties like length, channels and format are available before the audio data has been loaded.
		bool m_preloadAudioData = true;

		Meta(NonSerializable)
		int m_origSize = 0;

		Meta(NonSerializable)
		int m_compSize = 0;
	};
}

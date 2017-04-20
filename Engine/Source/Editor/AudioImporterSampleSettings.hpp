#pragma once

#include <AudioClipLoadType.hpp>
#include <AudioCompressionFormat.hpp>
#include "AudioSampleRateSetting.hpp"

namespace FishEditor
{
	/// <summary>
	///   <para>This structure contains a collection of settings used to define how an AudioClip should be imported.
	///
	/// This  structure is used with the AudioImporter to define how the AudioClip should be imported and treated during loading within the scene.</para>
	/// </summary>
	struct AudioImporterSampleSettings
	{
		/// <summary>
		///   <para>LoadType defines how the imported AudioClip data should be loaded.</para>
		/// </summary>
		FishEngine::AudioClipLoadType loadType = FishEngine::AudioClipLoadType::DecompressOnLoad;

		/// <summary>
		///   <para>Defines how the sample rate is modified (if at all) of the importer audio file.</para>
		/// </summary>
		AudioSampleRateSetting sampleRateSetting = AudioSampleRateSetting::PreserveSampleRate;

		/// <summary>
		///   <para>Target sample rate to convert to when samplerateSetting is set to OverrideSampleRate.</para>
		/// </summary>
		uint32_t sampleRateOverride;

		/// <summary>
		///   <para>CompressionFormat defines the compression type that the audio file is encoded to. Different compression types have different performance and audio artifact characteristics.</para>
		/// </summary>
		FishEngine::AudioCompressionFormat compressionFormat = FishEngine::AudioCompressionFormat::Vorbis;

		/// <summary>
		///   <para>Audio compression quality (0-1)
		///
		/// Amount of compression. The value roughly corresponds to the ratio between the resulting and the source file sizes.</para>
		/// </summary>
		float quality = 100;

		int conversionMode;
	};
}

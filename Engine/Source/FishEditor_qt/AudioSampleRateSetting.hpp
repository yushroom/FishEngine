#pragma once

namespace FishEditor
{
	/// <summary>
	///   <para>The sample rate setting used within the AudioImporter. This defines the sample rate conversion of audio on import.</para>
	/// </summary>
	enum class AudioSampleRateSetting
	{
		/// <summary>
		///   <para>Do not change the sample rate of the imported audio file. The sample rate will be preserved for the imported AudioClip.</para>
		/// </summary>
		PreserveSampleRate,
		/// <summary>
		///   <para>Let Unity deduce the optimal sample rate for the AudioClip being imported. The audio file will be analysed and a minimal sample rate chosen while still preserving audio quality.</para>
		/// </summary>
		OptimizeSampleRate,
		/// <summary>
		///   <para>Override the sample rate of the imported audio file with a custom value.</para>
		/// </summary>
		OverrideSampleRate
	};
}

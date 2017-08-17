#pragma once

namespace FishEngine
{
	/// <summary>
	///   <para>Type of the imported(native) data.</para>
	/// </summary>
	enum class AudioType
	{
		/// <summary>
		///   <para>3rd party / unknown plugin format.</para>
		/// </summary>
		UNKNOWN,
		/// <summary>
		///   <para>Acc - not supported.</para>
		/// </summary>
		ACC,
		/// <summary>
		///   <para>Aiff.</para>
		/// </summary>
		AIFF,
		/// <summary>
		///   <para>Impulse tracker.</para>
		/// </summary>
		IT = 10,
		/// <summary>
		///   <para>Protracker / Fasttracker MOD.</para>
		/// </summary>
		MOD = 12,
		/// <summary>
		///   <para>MP2/MP3 MPEG.</para>
		/// </summary>
		MPEG,
		/// <summary>
		///   <para>Ogg vorbis.</para>
		/// </summary>
		OGGVORBIS,
		/// <summary>
		///   <para>ScreamTracker 3.</para>
		/// </summary>
		S3M = 17,
		/// <summary>
		///   <para>Microsoft WAV.</para>
		/// </summary>
		WAV = 20,
		/// <summary>
		///   <para>FastTracker 2 XM.</para>
		/// </summary>
		XM,
		/// <summary>
		///   <para>Xbox360 XMA.</para>
		/// </summary>
		XMA,
		/// <summary>
		///   <para>VAG.</para>
		/// </summary>
		VAG,
		/// <summary>
		///   <para>iPhone hardware decoder, supports AAC, ALAC and MP3. Extracodecdata is a pointer to an FMOD_AUDIOQUEUE_EXTRACODECDATA structure.</para>
		/// </summary>
		AUDIOQUEUE
	};
}
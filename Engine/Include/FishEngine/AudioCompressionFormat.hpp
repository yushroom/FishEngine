#pragma once

namespace FishEngine
{
	/// <summary>
	///   <para>An enum containing different compression types.</para>
	/// </summary>
	enum class AudioCompressionFormat
	{
		/// <summary>
		///   <para>Uncompressed pulse-code modulation.</para>
		/// </summary>
		PCM,
		/// <summary>
		///   <para>Vorbis compression format.</para>
		/// </summary>
		Vorbis,
		/// <summary>
		///   <para>Adaptive differential pulse-code modulation.</para>
		/// </summary>
		ADPCM,
		/// <summary>
		///   <para>MPEG Audio Layer III.</para>
		/// </summary>
		MP3,
		/// <summary>
		///   <para>Sony proprietary hardware format.</para>
		/// </summary>
		VAG,
		/// <summary>
		///   <para>Sony proprietory hardware codec.</para>
		/// </summary>
		HEVAG,
		/// <summary>
		///   <para>Xbox One proprietary hardware format.</para>
		/// </summary>
		XMA,
		/// <summary>
		///   <para>AAC Audio Compression.</para>
		/// </summary>
		AAC,
		/// <summary>
		///   <para>Nintendo ADPCM audio compression format.</para>
		/// </summary>
		GCADPCM,
		/// <summary>
		///   <para>Sony proprietary hardware format.</para>
		/// </summary>
		ATRAC9
	};
}

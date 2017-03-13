#pragma once

namespace FishEditor
{
	// Select this to set basic parameters depending on the purpose of your texture.
	enum class TextureImporterType
	{
		Default,        // This is the most common setting used for all the textures in general.
		NormalMap,      // Select this to turn the color channels into a format suitable for real - time normal mapping.
		GUI,            // Use this if your texture is going to be used on any HUD / GUI Controls.
		Sprite,         // Select this if you will be using your texture for Sprite graphics.
		Cursor,         // Use this if your texture is going to be used as a cursor.
		Cookie,         // This sets up your texture with the basic parameters used for the Cookies of your lights.
		Lightmap,       // This sets up your texture with the parameters used by the lightmap.
		SingleChannel,  //Use this for texture containing a single channel.
	};

	enum class TextureImporterShape
	{
		Texture2D,
		TextureCube,
	};

	enum class TextureImporterCompression
	{
		Uncompressed,
		Compressed,
		CompressedHQ,
		CompressedLQ,
	};

	enum class TextureImporterNPOTScale
	{
		None,
		ToNearest,
		ToLarger,
		ToSmaller,
	};

	enum class TextureImporterMipFilter
	{
		BoxFilter,
		KaiserFilter,
	};

	enum class TextureImporterAlphaSource
	{
		None,			// No Alpha will be used.
		FromInput,		// Use Alpha from the input texture if one is provided.
		FromGrayScale,	// Generate Alpha from image gray scale.
	};

	enum class TextureImporterGenerateCubemap
	{
		Spheremap,		// Generate cubemap from spheremap texture.
		Cylindrical,	// Generate cubemap from cylindrical texture.
		FullCubemap,	// Generate cubemap from vertical or horizontal cross texture.
		AutoCubemap,	// Automatically determine type of cubemap generation from the source image.
	};
}
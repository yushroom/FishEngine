#include <FishEngine/Color.hpp>

namespace FishEngine 
{
	const Color Color::red(1.f, 0.f, 0.f, 1.f);
	const Color Color::green(0.f, 1.f, 0.f, 1.f);
	const Color Color::blue(0.f, 0.f, 1.f, 1.f);
	const Color Color::white(1.f, 1.f, 1.f, 1.f);
	const Color Color::black(0.f, 0.f, 0.f, 1.f);
	const Color Color::yellow(1.f, 0.921568632f, 0.0156862754f, 1.f);
	const Color Color::cyan(0.f, 1.f, 1.f, 1.f);
	const Color Color::magenta(1.f, 0.f, 1.f, 1.f);
	const Color Color::gray(0.5f, 0.5f, 0.5f, 1.f);
	const Color Color::grey(0.5f, 0.5f, 0.5f, 1.f);
	const Color Color::clear(0.f, 0.f, 0.f, 0.f);

	void Color::RGBToHSV(const Color& rgbColor, float* H, float* S, float* V)
	{
		if (rgbColor.b > rgbColor.g && rgbColor.b > rgbColor.r)
		{
			Color::RGBToHSVHelper(4.f, rgbColor.b, rgbColor.r, rgbColor.g, H, S, V);
		}
		else if (rgbColor.g > rgbColor.r)
		{
			Color::RGBToHSVHelper(2.f, rgbColor.g, rgbColor.b, rgbColor.r, H, S, V);
		}
		else
		{
			Color::RGBToHSVHelper(0.f, rgbColor.r, rgbColor.g, rgbColor.b, H, S, V);
		}
	}

	Color Color::HSVToRGB(float H, float S, float V, bool hdr /*= true*/)
	{
		Color white = Color::white;
		if (S == 0.f)
		{
			white.r = V;
			white.g = V;
			white.b = V;
		}
		else if (V == 0.f)
		{
			white.r = 0.f;
			white.g = 0.f;
			white.b = 0.f;
		}
		else
		{
			white.r = 0.f;
			white.g = 0.f;
			white.b = 0.f;
			float num = H * 6.f;
			int num2 = (int)Mathf::Floor(num);
			float num3 = num - (float)num2;
			float num4 = V * (1.f - S);
			float num5 = V * (1.f - S * num3);
			float num6 = V * (1.f - S * (1.f - num3));
			int num7 = num2;
			switch (num7 + 1)
			{
			case 0:
				white.r = V;
				white.g = num4;
				white.b = num5;
				break;
			case 1:
				white.r = V;
				white.g = num6;
				white.b = num4;
				break;
			case 2:
				white.r = num5;
				white.g = V;
				white.b = num4;
				break;
			case 3:
				white.r = num4;
				white.g = V;
				white.b = num6;
				break;
			case 4:
				white.r = num4;
				white.g = num5;
				white.b = V;
				break;
			case 5:
				white.r = num6;
				white.g = num4;
				white.b = V;
				break;
			case 6:
				white.r = V;
				white.g = num4;
				white.b = num5;
				break;
			case 7:
				white.r = V;
				white.g = num6;
				white.b = num4;
				break;
			}
			if (!hdr)
			{
				white.r = Mathf::Clamp(white.r, 0.f, 1.f);
				white.g = Mathf::Clamp(white.g, 0.f, 1.f);
				white.b = Mathf::Clamp(white.b, 0.f, 1.f);
			}
		}
		return white;
	}


	void Color::RGBToHSVHelper(float offset, float dominantcolor, float colorone, float colortwo, float* H, float* S, float* V)
	{
		*V = dominantcolor;
		if (*V != 0.f)
		{
			float num;
			if (colorone > colortwo)
			{
				num = colortwo;
			}
			else
			{
				num = colorone;
			}
			float num2 = *V - num;
			if (num2 != 0.f)
			{
				*S = num2 / *V;
				*H = offset + (colorone - colortwo) / num2;
			}
			else
			{
				*S = 0.f;
				*H = offset + (colorone - colortwo);
			}
			*H /= 6.f;
			if (*H < 0.f)
			{
				*H += 1.f;
			}
		}
		else
		{
			*S = 0.f;
			*H = 0.f;
		}
	}

}

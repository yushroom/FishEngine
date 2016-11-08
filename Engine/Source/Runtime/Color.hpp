#ifndef Color_hpp
#define Color_hpp

#include "Mathf.hpp"
#include "Vector3.hpp"
#include "Vector4.hpp"

namespace FishEngine
{
    class Color
    {
    public:
        union 
        {
            float m[4];
            struct { float r, g, b, a; };
        };

        Color() : r(1.f), g(1.f), b(1.f), a(1.f) {}
        Color(float r, float g, float b, float a = 1.f) : r(r), g(g), b(b), a(a) {}
        Color(const Vector4& v) : r(v.x), g(v.y), b(v.z), a(v.w) {}

        operator Vector4() const
        {
            return Vector4(r, g, b, a);
        }

        float operator[](int index) const
        {
            return m[index];
        }

        float& operator[](int index)
        {
            return m[index];
        }

        float grayscale() const
        {
            return 0.299f * r + 0.587f * g + 0.114f * b;
        }

        float maxColorComponent() const
        {
            return Mathf::Max(Mathf::Max(r, g), b);
        }

        static Color Lerp(const Color& a, const Color& b, float t)
        {
            t = Mathf::Clamp01(t);
            return Color(a.r + (b.r - a.r) * t, a.g + (b.g - a.g) * t, a.b + (b.b - a.b) * t, a.a + (b.a - a.a) * t);
        }

        static Color LerpUnclamped(const Color& a, const Color& b, float t)
        {
            return Color(a.r + (b.r - a.r) * t, a.g + (b.g - a.g) * t, a.b + (b.b - a.b) * t, a.a + (b.a - a.a) * t);
        }

        Color RGBMultiplied(float multiplier)
        {
            return Color(r * multiplier, g * multiplier, b * multiplier, a);
        }

        Color AlphaMultiplied(float multiplier)
        {
            return Color(r, g, b, a * multiplier);
        }

        Color RGBMultiplied(const Color& multiplier)
        {
            return Color(r * multiplier.r, g * multiplier.g, b * multiplier.b, a);
        }

        friend Color operator *(const Color& a, float b)
        {
            return Color(a.r * b, a.g * b, a.b * b, a.a * b);
        }

        friend Color operator *(float b, const Color& a)
        {
            return Color(a.r * b, a.g * b, a.b * b, a.a * b);
        }

        friend Color operator /(const Color& a, float b)
        {
            return Color(a.r / b, a.g / b, a.b / b, a.a / b);
        }

        //friend bool operator ==(Color lhs, Color rhs)
        //{
        //    return lhs == rhs;
        //}

        //static bool operator !=(Color lhs, Color rhs)
        //{
        //    return !(lhs == rhs);
        //}

        static void RGBToHSV(const Color& rgbColor, float* H, float* S, float* V);

        // Creates an RGB color from HSV input.
        // param H Hue [0..1].
        // param S Saturation [0..1].
        // param V Value [0..1].
        // param hdr Output HDR colors. If true, the returned color will not be clamped to [0..1].
        // return An opaque color with HSV matching the input.
        static Color HSVToRGB(float H, float S, float V, bool hdr = true);


        static const Color red;
        static const Color green;
        static const Color blue;
        static const Color white;
        static const Color black;
        static const Color yellow;
        static const Color cyan;
        static const Color magenta;
        static const Color gray;
        static const Color grey;
        static const Color clear;

    private:
        static void RGBToHSVHelper(float offset, float dominantcolor, float colorone, float colortwo, float* H, float* S, float* V);
    };
}


#endif // Color_hpp
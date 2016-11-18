#ifndef Vector2_hpp
#define Vector2_hpp

#include "Mathf.hpp"
#include "FishEngine.hpp"

namespace FishEngine
{
    class Vector2
    {
    public:
        union {
            struct { float x, y; };
            float m[2];
        };

        Vector2()
            : x(0), y(0)
        {
        }

        Vector2(const float x, const float y)
            : x(x), y(y)
        {
        }

        float operator[](const int index) const
        {
            Assert(index == 0 || index == 1);
            return m[index];
        }

        void Set(const float newX, const float newY)
        {
            x = newX;
            y = newY;
        }

        Vector2 normalized() const
        {
            float inv_mag = 1.0f / (x*x + y*y);
            return Vector2(x / inv_mag, y / inv_mag);
        }

        float magnitude() const
        {
            return ::std::sqrtf(x*x + y*y);
        }

        float sqrMagnitude() const
        {
            return x*x + y*y;
        }

        // (0, 0)
        static Vector2 zero;

        // (1, 1)
        static Vector2 one;

        // (0, 1)
        static Vector2 up;

        // (0, -1)
        static Vector2 down;

        // (-1, 0)
        static Vector2 left;

        // (1, 0)
        static Vector2 right;

        // Linearly interpolates between vectors a and b by t.
        // return a*(1-t)+b*t
        static Vector2 Lerp(const Vector2& a, const Vector2& b, float t)
        {
            t = Mathf::Clamp01(t);
            return Vector2(a.x + (b.x - a.x) * t, a.y + (b.y - a.y) * t);
        }

        static Vector2 LerpUnclamped(const Vector2& a, const Vector2& b, float t)
        {
            return Vector2(a.x + (b.x - a.x) * t, a.y + (b.y - a.y) * t);
        }

        // Moves a point current towards target.
        static Vector2 MoveTowards(const Vector2& current, const Vector2& target, float maxDistanceDelta)
        {
            Vector2 a = target - current;
            float magnitude = a.magnitude();
            if (magnitude <= maxDistanceDelta || magnitude == 0.f)
            {
                return target;
            }
            return current + a * (maxDistanceDelta / magnitude);
        }

        // Multiplies two vectors component-wise.
        static Vector2 Scale(const Vector2& a, const Vector2& b)
        {
            return Vector2(a.x * b.x, a.y * b.y);
        }

        // Multiplies every component of this vector by the same component of scale.
        void Scale(const Vector2& scale)
        {
            x *= scale.x;
            y *= scale.y;
        }

        // Makes this vector have a magnitude of 1.
        void Normalize()
        {
            float magnitude = this->magnitude();
            if (magnitude > 1E-05f)
            {
                x /= magnitude;
                y /= magnitude;
            }
            else
            {
                *this = Vector2::zero;
            }
        }

        Vector2 operator-() const
        {
            return Vector2(-x, -y);
        }

        Vector2 operator+(const Vector2& rhs) const
        {
            return Vector2(x + rhs.x, y + rhs.y);
        }

        Vector2 operator-(const Vector2& rhs) const
        {
            return Vector2(x - rhs.x, y - rhs.y);
        }

        Vector2 operator*(const float rhs) const
        {
            return Vector2(x * rhs, y * rhs);
        }

        Vector2 operator/(const float rhs) const
        {
            return Vector2(x / rhs, y / rhs);
        }

        friend Vector2 operator*(const float lhs, const Vector2& rhs)
        {
            return Vector2(lhs * rhs.x, lhs * rhs.y);
        }
    };

}

#endif // Vector2_hpp

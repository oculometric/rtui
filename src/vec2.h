#pragma once

namespace RTUI
{

struct Vec2
{
    int x = 0;
    int y = 0;
    
    inline Vec2& operator+=(const Vec2& other) { x += other.x; y += other.y; return *this; }
    inline Vec2& operator-=(const Vec2& other) { x -= other.x; y -= other.y; return *this; }
    inline Vec2& operator*=(const Vec2& other) { x *= other.x; y *= other.y; return *this; }
    inline Vec2& operator/=(const Vec2& other) { x /= other.x; y /= other.y; return *this; }
    inline Vec2& operator*=(const int other) { x *= other; y *= other; return *this; }
    inline Vec2& operator/=(const int other) { x /= other; y /= other; return *this; }
};

inline Vec2 operator+(const Vec2& a, const Vec2& b) { return Vec2{ a.x + b.x, a.y + b.y }; }
inline Vec2 operator-(const Vec2& a, const Vec2& b) { return Vec2{ a.x - b.x, a.y - b.y }; }
inline Vec2 operator*(const Vec2& a, const Vec2& b) { return Vec2{ a.x * b.x, a.y * b.y }; }
inline Vec2 operator/(const Vec2& a, const Vec2& b) { return Vec2{ a.x / b.x, a.y / b.y }; }
inline Vec2 operator*(const Vec2& a, const int b) { return Vec2{ a.x * b, a.y * b }; }
inline Vec2 operator/(const Vec2& a, const int b) { return Vec2{ a.x / b, a.y / b }; }

}

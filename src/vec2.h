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
    inline bool operator==(const Vec2& other) const { return x == other.x && y == other.y; }
};

inline Vec2 operator+(const Vec2& a, const Vec2& b) { return Vec2{ a.x + b.x, a.y + b.y }; }
inline Vec2 operator-(const Vec2& a, const Vec2& b) { return Vec2{ a.x - b.x, a.y - b.y }; }
inline Vec2 operator*(const Vec2& a, const Vec2& b) { return Vec2{ a.x * b.x, a.y * b.y }; }
inline Vec2 operator/(const Vec2& a, const Vec2& b) { return Vec2{ a.x / b.x, a.y / b.y }; }
inline Vec2 operator*(const Vec2& a, const int b) { return Vec2{ a.x * b, a.y * b }; }
inline Vec2 operator/(const Vec2& a, const int b) { return Vec2{ a.x / b, a.y / b }; }

struct Box2
{
    Vec2 min;
    Vec2 max;
    
    inline Vec2 size() const { return max - min; }
};

struct Transform2
{
    Vec2 min;
    Vec2 max;
    
    int left_clip = 0;
    int right_clip = 0;
    int top_clip = 0;
    int bottom_clip = 0;
};

}

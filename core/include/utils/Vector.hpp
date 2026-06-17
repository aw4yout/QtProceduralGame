#pragma once

#include "Math.hpp"

#include <cstdint>
#include <cmath>

namespace gen::utils {

template<Number T>
struct Vector2
{
    template<Number U>
    constexpr Vector2<U> as() const
    {
        return { static_cast<U>(x), static_cast<U>(y) };
    }

    constexpr auto operator<=>(const Vector2&) const = default;

    T x{}, y{};
};

template<Number T>
struct Vector3
{
    template<Number U>
    constexpr Vector3<U> as() const
    {
        return { static_cast<U>(x), static_cast<U>(y), static_cast<U>(z) };
    }

    constexpr auto operator<=>(const Vector3&) const = default;

    T x{}, y{}, z{};
};

using Vector2i = Vector2<int32_t>;
using Vector2f = Vector2<float>;
using Vector2d = Vector2<double>;

using Vector3i = Vector3<int32_t>;
using Vector3f = Vector3<float>;
using Vector3d = Vector3<double>;

template<Number T, Number U>
Vector2<T> operator+(Vector2<T> l, Vector2<U> r)
{
    return { l.x + r.x, l.y + r.y };
}

template<Number T, Number U>
Vector2<T> operator-(Vector2<T> l, Vector2<U> r)
{
    return { l.x - r.x, l.y - r.y };
}

template<Number T>
Vector2<T> operator-(Vector2<T> r)
{
    return { -r.x, -r.y };
}

template<Number T, Number U>
Vector2<T> operator*(Vector2<T> l, Vector2<U> r)
{
    return { l.x * r.x, l.y * r.y };
}

template<Number T, Number U>
Vector2<T> operator*(Vector2<T> l, U r)
{
    return { l.x * r, l.y * r };
}

template<Number T, Number U>
Vector2<T> operator/(Vector2<T> l, U r)
{
    return { l.x / r, l.y / r };
}

template<Number T, Number U>
Vector3<T> operator+(Vector3<T> l, Vector3<U> r)
{
    return { l.x + r.x, l.y + r.y, l.z + r.z };
}

template<Number T, Number U>
Vector3<T> operator-(Vector3<T> l, Vector3<U> r)
{
    return { l.x - r.x, l.y - r.y, l.z - r.z };
}

template<Number T>
Vector3<T> operator-(Vector3<T> r)
{
    return { -r.x, -r.y, -r.z };
}

template<Number T, Number U>
Vector3<T> operator*(Vector3<T> l, Vector3<U> r)
{
    return { l.x * r.x, l.y * r.y, l.z * r.z };
}

template<Number T, Number U>
Vector3<T> operator*(Vector3<T> l, U r)
{
    return { l.x * r, l.y * r, l.z * r };
}

template<Number T, Number U>
Vector3<T> operator/(Vector3<T> l, U r)
{
    return { l.x / r, l.y / r, l.z / r };
}

template<Number T>
T length(Vector2<T> v)
{
    return std::sqrt(v.x * v.x + v.y * v.y);
}

template<Number T>
T length(Vector3<T> v)
{
    return std::sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}

template<Number T>
struct Vector2Hash
{
    size_t operator()(const Vector2<T>& v) const noexcept
    {
        const auto h1 = std::hash<T>{}(v.x);
        const auto h2 = std::hash<T>{}(v.y);

        h1 ^= h2 + 0x9e3779b9 + (h1 << 6) + (h1 >> 2);
        return h1;
    }
};

template<Number T>
struct Vector3Hash
{
    size_t operator()(const Vector3<T>& v) const noexcept
    {
        const auto h1 = std::hash<T>{}(v.x);
        const auto h2 = std::hash<T>{}(v.y);
        const auto h3 = std::hash<T>{}(v.z);

        h1 ^= h2 + 0x9e3779b9 + (h1 << 6) + (h1 >> 2);
        h1 ^= h3 + 0x9e3779b9 + (h1 << 6) + (h1 >> 2);
        return h1;
    }
};

struct Vector2iHash
{
    size_t operator()(const Vector2i& v) const noexcept
    {
        return static_cast<size_t>(v.x) * 73856093u
             ^ static_cast<size_t>(v.y) * 19349663u;
    }
};

struct Vector3iHash
{
    size_t operator()(const Vector3i& v) const noexcept
    {
        return static_cast<size_t>(v.x) * 73856093u
             ^ static_cast<size_t>(v.y) * 19349663u
             ^ static_cast<size_t>(v.z) * 83492791u;
    }
};

} // gen::utils

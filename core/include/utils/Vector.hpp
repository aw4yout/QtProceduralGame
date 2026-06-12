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

} // gen::utils

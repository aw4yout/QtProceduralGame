#pragma once

#include <concepts>
#include <cmath>
#include <algorithm>

namespace gen::utils {

template<typename T>
concept Number = std::integral<T> || std::floating_point<T>;

template<std::floating_point T>
T mix(const T x, const T y, const T a)
{
    return x * (1.0 - a) + y * a;
}

template<std::floating_point T>
T smoothMin(T a, T b, T k)
{
    const T h = std::max(k - std::abs(a - b), 0.0f) / k;
    return std::min(a, b) - h * h * k * (1.0f / 4.0f);
}

template<std::floating_point T>
T smoothstep(T a, T b, T x)
{
    const auto t = std::clamp((x - a) / (b - a), 0.0f, 1.0f);
    return t * t * (3.0f - 2.0f * t);
}

}

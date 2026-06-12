#pragma once

#include <concepts>

namespace gen::utils {

template<typename T>
concept Number = std::integral<T> || std::floating_point<T>;

template<std::floating_point T>
T mix(const T x, const T y, const T a)
{
    return x * (1.0 - a) + y * a;
}

}

#pragma once
#include <array>

template <typename T>
unsigned 
constexpr lengthof_v = 0;

template <typename T, unsigned N>
unsigned 
constexpr lengthof_v<T[N]> = N;

template <typename T, unsigned N>
unsigned 
constexpr lengthof_v<std::array<T,N>> = N;

#define lengthof(expr)  lengthof_v<decltype(expr)>

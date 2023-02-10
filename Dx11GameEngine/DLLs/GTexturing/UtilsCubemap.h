#pragma once

#include "Bitmap.h"

Bitmap convertEquirectangularMapToVerticalCross(const Bitmap& b);
Bitmap convertVerticalCrossToCubeMapFaces(const Bitmap& b);


template <typename T>
T clamp(T v, T a, T b)
{
	if (v < a) return a;
	if (v > b) return b;
	return v;
}


inline static constexpr float M_PI = 3.14159265359f;

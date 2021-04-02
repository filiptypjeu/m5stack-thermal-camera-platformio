#pragma once

#include "env.h"

namespace Interpolation {

    float get_point(float *p, uint8_t rows, uint8_t cols, int8_t x, int8_t y);

    void set_point(float *p, uint8_t rows, uint8_t cols, int8_t x, int8_t y, float f);

    // src is a grid src_rows * src_cols
    // dest is a pre-allocated grid, dest_rows*dest_cols
    void interpolate_image(float *src, uint8_t src_rows, uint8_t src_cols, float *dest, uint8_t dest_rows, uint8_t dest_cols);

    // p is a list of 4 points, 2 to the left, 2 to the right
    float cubicInterpolate(float p[], float x);

    // p is a 16-point 4x4 array of the 2 rows & columns left/right/above/below
    float bicubicInterpolate(float p[], float x, float y);

    // src is rows*cols and dest is a 4-point array passed in already allocated!
    void get_adjacents_1d(float *src, float *dest, uint8_t rows, uint8_t cols, int8_t x, int8_t y);

    // src is rows*cols and dest is a 16-point array passed in already allocated!
    void get_adjacents_2d(float *src, float *dest, uint8_t rows, uint8_t cols, int8_t x, int8_t y);

}

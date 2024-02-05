//
// Created by Austin Taack on 8/6/23.
//

#ifndef RASTERIZER_POINT2_H
#define RASTERIZER_POINT2_H


#include "double3.h"

struct point2 {
    int x;
    int y;
    double z;
    double intensity;
    math3d::double3 normal;

    point2 (const int x, const int y) : x{x}, y{y}, z{0}, intensity{1.0}, normal{} {}
    point2 (const int x, const int y, const double z) : x{x}, y{y}, z{z}, intensity{1.0}, normal{} {}
    point2 (const int x, const int y, const double z, const double intensity) : x{x}, y{y}, z{z}, intensity{intensity},
            normal{}{}
    point2 (const int x, const int y, const double z, const double intensity, const math3d::double3 &normal) :
            x{x}, y{y}, z{z}, intensity{intensity}, normal{normal} {}
};

#endif //RASTERIZER_POINT2_H

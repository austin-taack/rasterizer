//
// Created by Austin Taack on 8/6/23.
//

#ifndef RASTERIZER_TRIANGLE_H
#define RASTERIZER_TRIANGLE_H


#include "double3.h"
#include "point2.h"

struct triangle {
    point2 a;
    point2 b;
    point2 c;
    math3d::color color;

    triangle(const point2 &a, const point2 &b, const point2 &c, const math3d::color &color) :
    a{a}, b{b}, c{c}, color{color} {}
};


#endif //RASTERIZER_TRIANGLE_H

//
// Created by Austin Taack on 8/9/23.
//

#ifndef RASTERIZER_CLIPPING_PLANE_H
#define RASTERIZER_CLIPPING_PLANE_H


#include "double3.h"

class clipping_plane {
private:
    math3d::double3 normal;
    double distance_to_orig;

public:
    clipping_plane(const math3d::double3 &normal, const double displacement_to_origin) :
    normal{normal}, distance_to_orig{displacement_to_origin} {}

    [[nodiscard]] double distance(const math3d::double3 &p) const;

    [[nodiscard]] math3d::double3 lintersect(const math3d::double3 &p1, const math3d::double3 &p2) const;
};


#endif //RASTERIZER_CLIPPING_PLANE_H

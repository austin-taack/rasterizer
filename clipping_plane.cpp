//
// Created by Austin Taack on 8/9/23.
//
#include "clipping_plane.h"

double clipping_plane::distance(const math3d::double3 &p) const {
    return math3d::dot(normal, p) - distance_to_orig;
}

math3d::double3 clipping_plane::lintersect(const math3d::double3 &p1, const math3d::double3 &p2) const {
    double t = (distance_to_orig - math3d::dot(normal, p1)) / math3d::dot(normal, p2 - p1);
    return p1 + t * (p2 - p1);
}


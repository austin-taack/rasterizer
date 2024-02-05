//
// Created by Austin Taack on 8/6/23.
//

#ifndef RASTERIZER_CAMERA_H
#define RASTERIZER_CAMERA_H


#include <utility>

#include "double3.h"
#include "point2.h"
#include "matrix_3x3.h"
#include "clipping_plane.h"
#include "face_vert.h"

class camera {
private:
    math3d::double3 location; // position of the camera
    math3d::matrix_3x3 rotation{}; // matrix representing camera rotation
    const double d; // distance from camera to viewport
    const double v_width; // width of the viewport
    const double v_height; // height of the viewport
    std::vector<clipping_plane> clips{}; // set of planes to clip objects against

public:
    camera();

    camera(const math3d::double3 &location, const math3d::matrix_3x3 &rotation, double d, double v_width,
           double v_height);

    [[nodiscard]] double get_d() const { return d; }

    [[nodiscard]] math3d::double3 get_location() const { return location; }

    [[nodiscard]] point2 project_to_image(const math3d::double3 &p, int image_width, int image_height) const;

    [[nodiscard]] bool clip(std::vector<math3d::double3> &vertices, std::vector<math3d::double3> &v_norms,
                    std::vector<std::vector<face_vert>> &faces, const math3d::double3 &center, double radius) const;
};


#endif //RASTERIZER_CAMERA_H

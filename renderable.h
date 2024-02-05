//
// Created by Austin Taack on 8/6/23.
//

#ifndef RASTERIZER_RENDERABLE_H
#define RASTERIZER_RENDERABLE_H


#include <vector>
#include <string>

#include "double3.h"
#include "triangle.h"
#include "matrix_3x3.h"
#include "camera.h"
#include "canvas.h"
#include "face_vert.h"

class renderable {
protected:
    std::vector<math3d::double3> vertices{};
    std::vector<math3d::double3> v_norms{};
    std::vector<std::vector<face_vert>> faces{};
    math3d::double3 center{};
    double radius{0};

public:
    renderable() = default;

    explicit renderable(const std::string &filename);

    explicit renderable(const std::vector<math3d::double3> &vertices) : vertices{vertices} {}

    void transform(double scale, const math3d::matrix_3x3 &rotation, const math3d::double3 &translation);

    virtual void to_image(const camera &camera, canvas &canvas);
};


#endif //RASTERIZER_RENDERABLE_H

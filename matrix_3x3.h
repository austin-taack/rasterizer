//
// Created by Austin Taack on 8/6/23.
//

#ifndef RASTERIZER_MATRIX_3X3_H
#define RASTERIZER_MATRIX_3X3_H


#include <vector>

#include "double3.h"

namespace math3d {

    class matrix_3x3 {
    private:
        std::vector<std::vector<double>> m;

    public:
        matrix_3x3() : m(3, std::vector<double>(3)) { m[0][0] = 1; m[1][1] = 1; m[2][2] = 1; }

        matrix_3x3(const double3 &p, const double3 &q, const double3 &r) :
        m{
            {p.x(), p.y(), p.z()},
            {q.x(), q.y(), q.z()},
            {r.x(), r.y(), r.z()}
        } {}

        // Operator overloading
        std::vector<double> &operator[](int index);

        std::vector<double> operator[](int index) const;

        matrix_3x3 &operator*= (double scalar);

        matrix_3x3 &operator/=(double scalar);

        // Linear algebra
        [[nodiscard]] matrix_3x3 inverse() const;

        [[nodiscard]] double determinant() const;

        matrix_3x3 transpose();
    };

    // Operator overloading
    double3 operator*(const matrix_3x3 &matrix, const double3 &vector);

    matrix_3x3 operator*(double scalar, const matrix_3x3 &matrix);

    matrix_3x3 operator/(const matrix_3x3 &matrix, double scalar);

} // math3d

#endif //RASTERIZER_MATRIX_3X3_H

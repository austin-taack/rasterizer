//
// Created by Austin Taack on 8/6/23.
//

#include "matrix_3x3.h"

namespace math3d {
    std::vector<double> &matrix_3x3::operator[](int index) {
        return m[index];
    }

    std::vector<double> matrix_3x3::operator[] (const int index) const {
        return m[index];
    }

    matrix_3x3 &matrix_3x3::operator*=(const double scalar) {
        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 3; ++j) {
                m[i][j] *= scalar;
            }
        }

        return *this;
    }

    matrix_3x3 &matrix_3x3::operator/=(const double scalar) {
        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 3; ++j) {
                m[i][j] /= scalar;
            }
        }

        return *this;
    }

    matrix_3x3 matrix_3x3::inverse() const {
        // Calculates the classical adjoint
        matrix_3x3 cofactors {
                {m[1][1] * m[2][2] - m[1][2] * m[2][1],
                 - (m[1][0] * m[2][2] - m[1][2] * m[2][0]),
                 m[1][0] * m[2][1] - m[1][1] * m[2][0]},
                {- (m[0][1] * m[2][2] - m[0][2] * m[2][1]),
                 m[0][0] * m[2][2] - m[0][2] * m[2][0],
                 - (m[0][0] * m[2][1] - m[0][1] * m[2][0])},
                {m[0][1] * m[1][2] - m[0][2] * m[1][1],
                 - (m[0][0] * m[1][2] - m[0][2] * m[1][0]),
                 m[0][0] * m[1][1] - m[0][1] * m[1][0]}
        };

        // Calculates the inverse matrix as the classical adjoint over the determinant
        return cofactors.transpose() / determinant();
    }

    double matrix_3x3::determinant() const {
        return  m[0][0] * m[1][1] * m[2][2] + m[0][1] * m[1][2] * m[2][0] + m[0][2] * m[1][0] * m[2][1] -
                m[0][2] * m[1][1] * m[2][0] - m[0][1] * m[1][0] * m[2][2] - m[0][0] * m[1][2] * m[2][1];
    }

    matrix_3x3 matrix_3x3::transpose() {
        return {
                {m[0][0], m[1][0], m[2][0]},
                {m[0][1], m[1][1], m[2][1]},
                {m[0][2], m[1][2], m[2][2]}
        };
    }

    double3 operator* (const matrix_3x3 &matrix, const double3 &vector) {
        double a = matrix[0][0] * vector.x() + matrix[0][1] * vector.y() + matrix[0][2] * vector.z();
        double b = matrix[1][0] * vector.x() + matrix[1][1] * vector.y() + matrix[1][2] * vector.z();
        double c = matrix[2][0] * vector.x() + matrix[2][1] * vector.y() + matrix[2][2] * vector.z();
        return {a, b, c};
    }

    matrix_3x3 operator*(const double scalar, const matrix_3x3 &matrix) {
        matrix_3x3 product{};
        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 3; ++j) {
                product[i][j] = scalar * matrix[i][j];
            }
        }
        return product;
    }

    matrix_3x3 operator/(const matrix_3x3 &matrix, const double scalar) {
        return (1.0 / scalar) * matrix;
    }
} // math3d
#include "double3.h"

#include <cmath>

namespace math3d {

    double3 &double3::operator= (const double3 &new3) {
        d3[0] = new3.x();
        d3[1] = new3.y();
        d3[2] = new3.z();
        return *this;
    }

    double3 &double3::operator+= (const double3 &add3) {
        d3[0] += add3.x();
        d3[1] += add3.y();
        d3[2] += add3.z();
        return *this;
    }

    double3 &double3::operator-= (const double3 &sub3) {
        d3[0] -= sub3.x();
        d3[1] -= sub3.y();
        d3[2] -= sub3.z();
        return *this;
    }

    double3 &double3::operator*= (const double scalar) {
        d3[0] *= scalar;
        d3[1] *= scalar;
        d3[2] *= scalar;
        return *this;
    }

    double3 &double3::operator/= (const double scalar) {
        d3[0] /= scalar;
        d3[1] /= scalar;
        d3[2] /= scalar;
        return *this;
    }

    double double3::length () const {
        return sqrt(length_squared());
    }

    double double3::length_squared () const {
        return (d3[0] * d3[0]) + (d3[1] * d3[1]) + (d3[2] * d3[2]);
    }

    double3 double3::norm () const {
        return *this / length();
    }

    double3 &double3::round() {
        d3[0] = ::round(d3[0]);
        d3[1] = ::round(d3[1]);
        d3[2] = ::round(d3[2]);
        return *this;
    }

    double3 operator- (const double3 &orig) {
        return {-orig.x(), -orig.y(), -orig.z()};
    }

    double3 operator+ (const double3 &add1, const double3 &add2) {
        return {add1.x() + add2.x(), add1.y() + add2.y(), add1.z() + add2.z()};
    }

    double3 operator- (const double3 &sub1, const double3 &sub2) {
        return {sub1.x() - sub2.x(), sub1.y() - sub2.y(), sub1.z() - sub2.z()};
    }

    double3 operator* (double scalar, const double3 &factor) {
        return {scalar * factor.x(), scalar * factor.y(), scalar * factor.z()};
    }

    double3 operator* (const double3 &factor, double scalar) {
        return {scalar * factor.x(), scalar * factor.y(), scalar * factor.z()};
    }

    double3 operator/ (const double3 &div, double scalar) {
        return {div.x() / scalar, div.y() / scalar, div.z() / scalar};
    }

    std::ostream &operator<< (std::ostream &out, const double3 &d3) {
        out << d3.x() << " " << d3.y() << " " << d3.z();
        return out;
    }

    double distance (const double3 &d1, const double3 &d2) {
        return (d2 - d1).length();
    }

    double dot (const double3 &factor1, const double3 &factor2) {
        return (factor1.x() * factor2.x()) + (factor1.y() * factor2.y()) + (factor1.z() * factor2.z());
    }

    double3 cross (const double3 &factor1, const double3 &factor2) {
        return {
                (factor1.y() * factor2.z()) - (factor1.z() * factor2.y()),
                (factor1.z() * factor2.x()) - (factor1.x() * factor2.z()),
                (factor1.x() * factor2.y()) - (factor1.y() * factor2.x())
        };
    }

}

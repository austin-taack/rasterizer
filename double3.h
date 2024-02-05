#ifndef MATH3D_DOUBLE3_H
#define MATH3D_DOUBLE3_H

#include <ostream>

namespace math3d {

    class double3 {

    private:
        double d3[3];

    public:
        // Constructors
        double3 () : d3{0, 0, 0} {}

        double3 (double x, double y, double z) : d3{x, y, z} {}

        double3 (const double3 &copy) : d3{copy.x(), copy.y(), copy.z()} {}

        // Getters
        [[nodiscard]] double x () const { return d3[0]; }

        [[nodiscard]] double y () const { return d3[1]; }

        [[nodiscard]] double z () const { return d3[2]; }

        // Setters
        void set_x (double x) { d3[0] = x; }

        void set_y (double y) { d3[1] = y; }

        void set_z (double z) { d3[2] = z; }

        // Arithmetic
        double3 &round ();

        double3 &operator= (const double3 &new3);

        double3 &operator+= (const double3 &add3);

        double3 &operator-= (const double3 &sub3);

        double3 &operator*= (double scalar);

        double3 &operator/= (double scalar);

        // Geometry
        [[nodiscard]] double length () const;

        [[nodiscard]] double length_squared () const;

        [[nodiscard]] double3 norm () const;
    };

    // Operator overloading
    double3 operator- (const double3 &orig);

    double3 operator+ (const double3 &add1, const double3 &add2);

    double3 operator- (const double3 &sub1, const double3 &sub2);

    double3 operator* (double scalar, const double3 &factor);

    double3 operator* (const double3 &factor, double scalar);

    double3 operator/ (const double3 &div, double scalar);

    std::ostream &operator<< (std::ostream &out, const double3 &d3);

    // Geometry
    double distance (const double3 &d1, const double3 &d2);

    // Linear algebra
    double dot (const double3 &factor1, const double3 &factor2);

    double3 cross (const double3 &factor1, const double3 &factor2);

    // Aliases
    using color = double3;
}

#endif //MATH3D_DOUBLE3_H

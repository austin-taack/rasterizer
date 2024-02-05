//
// Created by Austin Taack on 8/6/23.
//

#include "camera.h"
#include "double3.h"

#include <cmath>

camera::camera() : location{0, 0, 0}, d{1}, v_width{1}, v_height{1} {
    clips.push_back({{0, 0, 1}, 1});
    clips.push_back({{1.0 / sqrt(2), 0, 1.0 / sqrt(2)}, 0});
    clips.push_back({{-1.0 / sqrt(2), 0, 1.0 / sqrt(2)}, 0});
    clips.push_back({{0, 1.0 / sqrt(2), 1.0 / sqrt(2)}, 0});
    clips.push_back({{0, -1.0 / sqrt(2), 1.0 / sqrt(2)}, 0});
    //clips.push_back({{-sqrt(5) / sqrt(6), 0, 1.0 / sqrt(6)}, 1}); // Clipping test
}

camera::camera(const math3d::double3 &location, const math3d::matrix_3x3 &rotation, const double d,
               const double v_width, const double v_height) : location{location}, d{d}, v_width{v_width},
               v_height{v_height} {

    this->rotation = rotation.inverse();
    clips.push_back({{0, 0, 1}, 1});
    clips.push_back({{1.0 / sqrt(2), 0, 1.0 / sqrt(2)}, 0});
    clips.push_back({{-1.0 / sqrt(2), 0, 1.0 / sqrt(2)}, 0});
    clips.push_back({{0, 1.0 / sqrt(2), 1.0 / sqrt(2)}, 0});
    clips.push_back({{0, -1.0 / sqrt(2), 1.0 / sqrt(2)}, 0});
    //clips.push_back({{-sqrt(5) / sqrt(6), 0, 1.0 / sqrt(6)}, 2}); // Clipping test
}

point2 camera::project_to_image(const math3d::double3 &p, const int image_width, const int image_height) const {
    // Calculate points on the viewport
    double vx = p.x() * d / p.z();
    double vy = p.y() * d / p.z();

    // Convert from viewport to canvas
    int x = (int) (vx * image_width / v_width) + image_width / 2;
    int y = (int) (- vy * image_height / v_height) + image_height / 2;

    return {x, y, p.z()};
}

void swap(math3d::double3 &p1, math3d::double3 &p2, int &i1, int &i2) {
    math3d::double3 temp_p = p1;
    p1 = p2;
    p2 = temp_p;

    int temp_i = i1;
    i1 = i2;
    i2 = temp_i;
}

bool camera::clip(std::vector<math3d::double3> &vertices, std::vector<math3d::double3> &v_norms,
                    std::vector<std::vector<face_vert>> &faces, const math3d::double3 &center, double radius) const {

    // Transform camera
    for (auto &vertex : vertices) {
        vertex = vertex - location;
        vertex = rotation * vertex;
    }

    for (auto &v_norm : v_norms) v_norm = rotation * v_norm;

    // Clip against given planes
    for (const auto &plane : clips) {
        /*double dist = plane.distance(center);
        if (dist < -radius) return true;

        if (dist < radius) {
            int i = 0;
            int end = (int) faces.size();
            while (i < end) {
                math3d::double3 a = vertices[faces[i][0].vertex - 1];
                math3d::double3 b = vertices[faces[i][1].vertex - 1];
                math3d::double3 c = vertices[faces[i][2].vertex - 1];

                int i_a = 0;
                int i_b = 1;
                int i_c = 2;

                if (plane.distance(a) < plane.distance(b))
                    swap(a, b, i_a, i_b);
                if (plane.distance(a) < plane.distance(c))
                    swap(a, c, i_a, i_c);
                if (plane.distance(b) < plane.distance(c))
                    swap(b, c, i_b, i_c);

                if (plane.distance(a) < 0) {
                    faces.erase(std::begin(faces) + i);
                    --end;
                } else if (plane.distance(b) < 0) {
                    int v = (int) vertices.size() + 1;
                    vertices.push_back(plane.lintersect(a, b));
                    vertices.push_back(plane.lintersect(a, c));
                    faces[i] = {faces[i][i_a], {v, 0, 0}, {v + 1, 0, 0}};
                    ++i;
                } else if (plane.distance(c) < 0) {
                    int v = (int) vertices.size() + 1;
                    vertices.push_back(plane.lintersect(a, c));
                    vertices.push_back(plane.lintersect(b, c));
                    faces[i] = {faces[i][i_a], faces[i][i_b], {v, 0, 0}};
                    faces.push_back({faces[i][i_b], {v, 0, 0}, {v + 1, 0, 0}});
                    ++i;
                } else {
                    ++i;
                }
            }
        }*/

        double dist = plane.distance(center);
        if (dist < -radius) return true;

        if (dist < radius) {
            int i = 0;
            int end = (int) faces.size();

            while (i < end) {
                std::vector<face_vert> face = faces[i];
                int num_valid_vertices = 0;
                for (int j = 0; j < 3; ++j)
                    if (plane.distance(vertices[face[j].vertex - 1]) > 0)
                        ++num_valid_vertices;


                switch (num_valid_vertices) {
                    case 3: // All vertices in clipping volume
                        ++i;
                        break;

                    case 2: // Only 2 vertices in clipping volume
                        while (plane.distance(vertices[face[2].vertex - 1]) >= 0) {
                            face_vert temp = face[0];
                            face[0] = face[1];
                            face[1] = face[2];
                            face[2] = temp;
                        }

                        face[2].vertex = (int) vertices.size() + 1;
                        faces[i] = face;
                        faces.push_back({
                            {(int) vertices.size() + 1, face[2].texture, face[2].normal},
                            face[1],
                            {(int) vertices.size() + 2, face[2].texture, face[2].normal}
                        });

                        vertices.push_back(plane.lintersect(vertices[face[0].vertex - 1], vertices[face[2].vertex - 1]));
                        vertices.push_back(plane.lintersect(vertices[face[1].vertex - 1], vertices[face[2].vertex - 1]));

                        ++i;
                        break;

                    case 1: // Only 1 vertex in clipping volume
                        while (plane.distance(vertices[face[0].vertex - 1]) <= 0) {
                            face_vert temp = face[0];
                            face[0] = face[1];
                            face[1] = face[2];
                            face[2] = temp;
                        }

                        face[1].vertex = (int) vertices.size() + 1;
                        face[2].vertex = (int) vertices.size() + 2;
                        faces[i] = face;

                        vertices.push_back(plane.lintersect(vertices[face[0].vertex - 1], vertices[face[1].vertex - 1]));
                        vertices.push_back(plane.lintersect(vertices[face[0].vertex - 1], vertices[face[2].vertex - 1]));

                        ++i;
                        break;

                    default: // No vertices in clipping volume
                        faces.erase(std::begin(faces) + i);
                        --end;
                        break;
                }
            }
        }
    }

    return false;
}

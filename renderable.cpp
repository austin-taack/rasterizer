//
// Created by Austin Taack on 8/6/23.
//

#include "renderable.h"

#include <fstream>
#include <iostream>

#include "colors.h"

// Create model from wavefront .obj file
renderable::renderable(const std::string &filename) {
    // Read input file
    std::ifstream model_input {filename};
    while (model_input) {
        std::string str_input;
        model_input >> str_input;

        if (str_input == "v") {
            double x, y, z;
            model_input >> x;
            model_input >> y;
            model_input >> z;
            vertices.emplace_back(x, y, z);

        } else if (str_input == "f") {
            std::vector<face_vert> face;
            std::string line;
            std::getline(model_input, line);
            line.erase(std::begin(line));

            try {
                while (!line.empty()) {
                    auto found = line.find('/');
                    int vertex = std::stoi(line.substr(0, found));
                    if (vertex < 0) vertex = -vertex;
                    line.erase(0, found + 1);

                    found = line.find('/');
                    int texture = std::stoi(line.substr(0, found));
                    if (texture < 0) texture = -texture;
                    line.erase(0, found + 1);

                    found = line.find(' ');
                    auto end = (found == std::string::npos) ?
                            std::end(line) : std::begin(line) + (long)  found + 1;
                    int normal = std::stoi(line.substr(0, found));
                    if (normal < 0) normal = -normal;
                    line.erase(std::begin(line), end);

                    if (face.size() < 3) {
                        face.push_back({vertex, texture, normal});
                        if (face.size() == 3) {
                            faces.push_back(face);
                        }
                    } else {
                        faces.push_back({face[0], face[2], {vertex, texture, normal}});
                    }
                }
            } catch (...) { continue; }

        } else if (str_input == "vn") {
            double x, y, z;
            model_input >> x;
            model_input >> y;
            model_input >> z;
            v_norms.emplace_back(x, y, z);

        }
    }

    // Calculate center
    for (const auto &vertex : vertices)
        center += vertex;

    center /= (double) vertices.size();

    // Calculate radius
    for (const auto &vertex : vertices) {
        double r = math3d::distance(vertex, center);
        if (r > radius) radius = r;
    }
}

// Transform model by a given translation, rotation, and scaling
void renderable::transform(double scale, const math3d::matrix_3x3 &rotation, const math3d::double3 &translation) {
    for (auto &vertex : vertices) {
        vertex *= scale;
        vertex = rotation * vertex;
        vertex += translation;
    }

    radius *= scale;
    center = rotation * center;
    center += translation;
}

void renderable::to_image(const camera &camera, canvas &canvas) {
    // Clip object
    std::vector<std::vector<face_vert>> clipped_faces {faces};
    std::vector<math3d::double3> clipped_vertices {vertices};
    std::vector<math3d::double3> clipped_v_norms {v_norms};

    if (camera.clip(clipped_vertices, clipped_v_norms, clipped_faces, center, radius)) return;

    // Project 3D points onto image
    std::vector<point2> projections{};

    // Uncomment light_vecs when using Gouraud lighting
    //std::vector<math3d::double3> light_vecs{};
    math3d::double3 light = {0, 10, 0};
    for (auto &vertex : clipped_vertices) {
        projections.push_back(camera.project_to_image(vertex, canvas.width(), canvas.height()));
        //light_vecs.push_back(light - vertex);
    }


    // Create random color generator
    // rand_color rand{};

    // Draw triangles to canvas
    for (auto &face : clipped_faces) {
        point2 a = projections[face[0].vertex - 1];
        point2 b = projections[face[1].vertex - 1];
        point2 c = projections[face[2].vertex - 1];

        // Back face culling
        if (b.x != a.x) {
            double slope = (double) (b.y - a.y) / (b.x - a.x);
            int extrap = (int) round(slope * (c.x - b.x) + b.y);
            if ((a.x < b.x && c.y <= extrap) || (a.x > b.x && c.y >= extrap)) continue;
        } else if (a.y < b.y && b.x < c.x) {
            continue;
        } else if (a.y > b.y && b.x > c.x) {
            continue;
        }

        a.normal = clipped_v_norms[face[0].normal - 1];
        b.normal = clipped_v_norms[face[1].normal - 1];
        c.normal = clipped_v_norms[face[2].normal - 1];

        canvas.put_shaded_triangle(a, b, c, {200, 162, 200}, camera, light);
    }
}

//
// Created by Austin Taack on 7/31/23.
//

#ifndef RASTERIZER_CANVAS_H
#define RASTERIZER_CANVAS_H


#include <vector>
#include <iostream>

#include <SDL.h>

#include "double3.h"
#include "point2.h"
#include "triangle.h"
#include "camera.h"


class canvas {

private:
    std::vector<std::vector<math3d::color>> canv;
    std::vector<std::vector<double>> z_buff;
    const int canvas_width;
    const int canvas_height;
    SDL_Window* window;
    SDL_Renderer* renderer;

    // Helper member functions
    void buffer_pixel(int x, int y, const math3d::color &color);

public:
    // Constructor and destructor
    canvas (int width, int height);

    ~canvas();

    // Getters
    [[nodiscard]] int width() const { return canvas_width; }

    [[nodiscard]] int height() const { return canvas_height; }

    // Rendering function
    void render() const;

    // Drawing functions
    void put_pixel(int x, int y, const math3d::color &color);

    void put_pixel(int x, int y, double z, const math3d::color &color);

    void put_line(const point2 &p1, const point2 &p2, const math3d::color &color);

    void put_wireframe_triangle(const point2 &a, const point2 &b, const point2 &c, const math3d::color &color);

    void put_wireframe_triangle(const triangle &triangle);

    void put_triangle(const point2 &a, const point2 &b, const point2 &c, const math3d::color &color);

    void put_shaded_triangle(const point2 &a, const point2 &b, const point2 &c, const math3d::color &color,
                             const camera &camera, const math3d::double3 &point_light);

    void put_triangle(const triangle &triangle);
};


#endif //RASTERIZER_CANVAS_H

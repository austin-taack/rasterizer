//
// Created by Austin Taack on 7/31/23.
//

#include "canvas.h"

#include <iostream>
#include <cmath>

// Wrapper for SDL window and pixel buffer
canvas::canvas (const int width, const int height) :
        canvas_width {width}, canvas_height {height},
        z_buff(width, (std::vector<double> (height, 0.0))),
        canv(width,(std::vector<math3d::color> (height, {0, 0, 0}))),
        window{nullptr}, renderer{nullptr} {

    SDL_Init(SDL_INIT_VIDEO);

    window = SDL_CreateWindow("Rasterizer", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                              canvas_width, canvas_height, SDL_WINDOW_SHOWN);

    renderer = SDL_CreateRenderer(window, -1, 0);

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
}

// Destroy SDL window
canvas::~canvas() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

// Copy contents of the pixel buffer onto the window and hold until the program is closed
void canvas::render() const {
    std::cout << "P3\n" << canvas_width << ' ' << canvas_height << "\n255" << std::endl;
    for (int y = 0; y < canvas_height; ++y) {
        for (int x = 0; x < canvas_width; ++x) {
            SDL_SetRenderDrawColor(renderer, canv[x][y].x(), canv[x][y].y(), canv[x][y].z(), 255);
            SDL_RenderDrawPoint(renderer, x, y);
        }
    }

    SDL_RenderPresent(renderer);

    SDL_Event event;
    while (true) {
        if (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) break;
        }
    }
}

// Add a pixel to the pixel buffer
void canvas::buffer_pixel(const int x, const int y, const math3d::color &color) {
    int red = (color.x() > 0) ? ((color.x() < 256) ? (int) color.x() : 256) : 0;
    int green = (color.y() > 0) ? ((color.y() < 256) ? (int) color.y() : 256) : 0;
    int blue = (color.z() > 0) ? ((color.z() < 256) ? (int) color.z() : 256) : 0;
    canv[x][y] = {(double) red, (double) green, (double) blue};
}

// Forces a given point onto the pixel buffer (ignores z-buffering)
void canvas::put_pixel(const int x, const int y, const math3d::color &color) {
    if (x >= 0 && x < canvas_width && y >= 0 && y < canvas_height)
        buffer_pixel(x, y, color);
}

// Attempts to place a pixel into the pixel buffer (uses z-buffering)
void canvas::put_pixel(const int x, const int y, const double z, const math3d::color &color) {
    double z_val = 1.0 / z;
    if (x >= 0 && x < canvas_width && y >= 0 && y < canvas_height) {
        if (z_val > z_buff[x][y]) {
            buffer_pixel(x, y, color);
            z_buff[x][y] = z_val;
        }
    }
}

// Swaps the contents of two independent and dependent variables
void swap(int &x1, int &x2, int &y1, int &y2) {
    int temp = x1;
    x1 = x2;
    x2 = temp;

    temp = y1;
    y1 = y2;
    y2 = temp;
}

// Swaps the contents of two points
void swap(point2 &p1, point2 &p2) {
    swap (p1.x, p2.x, p1.y, p2.y);

    double temp = p1.z;
    p1.z = p2.z;
    p2.z = temp;

    temp = p1.intensity;
    p1.intensity = p2.intensity;
    p2.intensity = temp;

    math3d::double3 norm_temp = p1.normal;
    p1.normal = p2.normal;
    p2.normal = norm_temp;
}

// Creates a linear interpolation between two points
std::vector<double> linterp(int i1, double d1, int i2, double d2) {
    if (i1 == i2) return {(double) d1};

    double slope = (double) (d2 - d1) / (i2 - i1);
    double d = d1;

    std::vector<double> values {};

    for (int i = i1; i <= i2; ++i) {
        values.push_back(d);
        d += slope;
    }

    return values;
}

// Draws a line on the canvas between the two given points
void canvas::put_line(const point2 &p1, const point2 &p2, const math3d::color &color) {
    int x1{p1.x}, x2{p2.x}, y1{p1.y}, y2{p2.y};

    if (fabs(p2.x - p1.x) > fabs (p2.y-p1.y)) {
        if (x1 > x2) swap(x1, x2, y1, y2);
        std::vector<double> y = linterp(x1, y1, x2, y2);
        for (int x = x1; x <= x2; ++x) {
            int y_n = (int) round(y[x - x1]);
            put_pixel (x, y_n, color);
        }
    } else {
        if (y1 > y2) swap(x1, x2, y1, y2);
        std::vector<double> x = linterp(y1, x1, y2, x2);
        for (int y = y1; y <= y2; ++y) {
            int x_n = (int) round(x[y - y1]);
            put_pixel (x_n, y, color);
        }
    }
}

// Draws a wireframe triangle onto the canvas between points a, b, and c
void canvas::put_wireframe_triangle(const point2 &a, const point2 &b, const point2 &c, const math3d::color &color) {
    put_line(a, b, color);
    put_line(a, c, color);
    put_line(b, c, color);
}

// Draws the given triangle as a wireframe onto the canvas
void canvas::put_wireframe_triangle(const triangle &triangle) {
    put_wireframe_triangle(triangle.a, triangle.b, triangle.c, triangle.color);
}

// Sorts the points of a triangle by their y-value
void order_triangle(point2 &top, point2 &mid, point2 &bottom) {
    if (top.y > mid.y) swap(top, mid);
    if (top.y > bottom.y) swap(top, bottom);
    if (mid.y > bottom.y) swap(mid, bottom);
}

// Interpolates the sides of a given triangle and returns them in bound1 and bound2
void calculate_triangle_sides(const double top_x, const int top_y, const double mid_x, const int mid_y,
                              const double bottom_x, const int bottom_y, std::vector<double> &bound1,
                              std::vector<double> &bound2) {

    bound1 = linterp(top_y, top_x, bottom_y, bottom_x);
    bound2 = linterp(top_y, top_x, mid_y, mid_x);
    std::vector<double> bound2_end = linterp(mid_y, mid_x, bottom_y, bottom_x);

    bound2.pop_back();
    bound2.insert(std::end(bound2), std::begin(bound2_end), std::end(bound2_end));
}

// Determines all the points along the perimeter of a triangle, given in left and right
bool calculate_triangle(const double top_x, const int top_y, const double mid_x, const int mid_y, const double bottom_x,
                        const int bottom_y, std::vector<double> &left, std::vector<double> &right) {

    calculate_triangle_sides(top_x, top_y, mid_x, mid_y, bottom_x, bottom_y, left, right);
    int compare_y = mid_y - top_y;
    bool switched = left[compare_y] > right[compare_y];

    if (switched) {
        std::vector<double> temp = left;
        left = right;
        right = temp;
    }

    return switched;
}

// Determines all the points along the perimeter of a triangle, given in left and right
void calculate_triangle(const double top_x, const int top_y, const double mid_x, const int mid_y, const double bottom_x,
                        const int bottom_y, std::vector<double> &left, std::vector<double> &right,
                        const bool switched) {

    calculate_triangle_sides(top_x, top_y, mid_x, mid_y, bottom_x, bottom_y, left, right);

    if (switched) {
        std::vector<double> temp = left;
        left = right;
        right = temp;
    }
}

// Draws a filled triangle onto the canvas with vertices a, b, and c
void canvas::put_triangle(const point2 &a, const point2 &b, const point2 &c, const math3d::color &color) {
    point2 top{a}, mid{b}, bottom{c};
    order_triangle(top, mid, bottom);
    std::vector<double> left{}, right{};
    calculate_triangle(top.x, top.y, mid.x, mid.y, bottom.x, bottom.y, left, right);

    for (int y = top.y; y <= bottom.y; ++y) {
        for (int x = (int) round(left[y - top.y]); x <= (int) round(right[y - top.y]); ++x) {
            put_pixel(x, y, color);
        }
    }
}

void canvas::put_triangle(const triangle &triangle) {
    put_triangle(triangle.a, triangle.b, triangle.c, triangle.color);
}

// Draw a triangle with points a, b, and c that is shaded with respect to a camera and a point light
void canvas::put_shaded_triangle(const point2 &a, const point2 &b, const point2 &c, const math3d::color &color,
                                 const camera &camera, const math3d::double3 &point_light) {

    point2 top{a}, mid{b}, bottom{c};
    order_triangle(top, mid, bottom);
    std::vector<double> left{}, right{};
    bool switched = calculate_triangle(top.x, top.y, mid.x, mid.y,
                                       bottom.x, bottom.y, left, right);

    std::vector<double> z_left{}, z_right{};
    calculate_triangle(top.z, top.y, mid.z, mid.y,
                       bottom.z, bottom.y, z_left, z_right, switched);

    std::vector<double> norm_x_left{}, norm_x_right{};
    calculate_triangle(top.normal.x(), top.y, mid.normal.x(), mid.y,
                       bottom.normal.x(), bottom.y, norm_x_left, norm_x_right, switched);

    std::vector<double> norm_y_left{}, norm_y_right{};
    calculate_triangle(top.normal.y(), top.y, mid.normal.y(), mid.y,
                       bottom.normal.y(), bottom.y, norm_y_left, norm_y_right, switched);

    std::vector<double> norm_z_left{}, norm_z_right{};
    calculate_triangle(top.normal.z(), top.y, mid.normal.z(), mid.y,
                       bottom.normal.z(), bottom.y, norm_z_left, norm_z_right, switched);

    for (int y = top.y; y <= bottom.y; ++y) {
        int x1 = (int) round(left[y - top.y]);
        int x2 = (int) round(right[y - top.y]);
        std::vector<double> z_vals = linterp(x1, z_left[y - top.y], x2, z_right[y - top.y]);
        std::vector<double> x_norms = linterp(x1, norm_x_left[y - top.y], x2, norm_x_right[y - top.y]);
        std::vector<double> y_norms = linterp(x1, norm_y_left[y - top.y], x2, norm_y_right[y - top.y]);
        std::vector<double> z_norms = linterp(x1, norm_z_left[y - top.y], x2, norm_z_right[y - top.y]);

        for (int x = x1; x <= x2; ++x) {
            int i = x - x1;
            math3d::double3 p3D{x * z_vals[i] / camera.get_d(), y * z_vals[i] / camera.get_d(), z_vals[i]};
            math3d::double3 normal{x_norms[i], y_norms[i], z_norms[i]};
            math3d::double3 light_vec = point_light - p3D;
            double n_dot_l = math3d::dot(normal, light_vec);
            math3d::double3 reflection = 2 * normal * n_dot_l - light_vec;
            math3d::double3 cam_dist = p3D - camera.get_location();
            // double matte = n_dot_l / normal.length() / light_vec.length();
            double shiny = math3d::dot(reflection, cam_dist) / reflection.length() / cam_dist.length();
            double intensity = 0.5 + 0.5 * (shiny * shiny);
            if (intensity > 1.0) intensity = 1.0;
            put_pixel(x, y, z_vals[i], intensity * color);
        }
    }
}

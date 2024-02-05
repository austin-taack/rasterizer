//
// Created by Austin Taack on 8/6/23.
//

#ifndef RASTERIZER_COLORS_H
#define RASTERIZER_COLORS_H


#include <random>

#include "double3.h"

// Colors
math3d::color red {255, 0, 0};
math3d::color green {0, 255, 0};
math3d::color blue {0, 0, 255};
math3d::color yellow {255, 255, 0};
math3d::color magenta {255, 0, 255};
math3d::color cyan {0, 255, 255};

class rand_color {
private:
    std::mt19937 mt{};
    std::uniform_int_distribution<> color_range{50, 255};

public:
    math3d::color next() {
        return {(double) color_range(mt), (double) color_range(mt), (double) color_range(mt)};
    }
};

#endif //RASTERIZER_COLORS_H

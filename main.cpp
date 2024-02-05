#include <cmath>

#include "canvas.h"
#include "camera.h"
#include "renderable.h"


#define PI 3.14159

int main() {

    // Set up render object
    renderable teapot{"/Users/austintaack/CLionProjects/rasterizer/newell_teaset/teapot.obj"};
    teapot.transform(0.5, {
            {0, 0, -1},
            {0, 1, 0},
            {1, 0, 0}
    }, {0, -1, 4});

    // Initialize canvas
    const int w = 800;
    const int h = 600;
    canvas canvas {w, h};

    // Initialize camera
    camera camera {{0, 1.5, 0}, {
            {1, 0, 0},
            {0, cos(-PI/6), sin(-PI/6)},
            {0, -sin(-PI/6), cos(-PI/6)}
    }, 1, 1, 0.75};

    // Create scene
    std::vector<renderable> scene{teapot};

    // Draw objects to canvas
    for (auto &obj : scene)
        obj.to_image(camera, canvas);

    // Render cube image
    canvas.render();
}

cmake_minimum_required(VERSION 3.24)
project(rasterizer)

set(CMAKE_CXX_STANDARD 20)

find_package(SDL2 REQUIRED)

add_executable(rasterizer
        main.cpp
        double3.cpp
        canvas.cpp
        canvas.h
        camera.cpp
        camera.h
        point2.h
        renderable.h
        triangle.h
        colors.h
        matrix_3x3.cpp
        matrix_3x3.h
        renderable.cpp
        clipping_plane.h
        clipping_plane.cpp
        face_vert.h
)

include_directories(/Users/austintaack/CLionProjects)
target_link_libraries(rasterizer SDL2::SDL2)

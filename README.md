# CPU Rasterizer

This project is a simple renderer that uses rasterization on the CPU.
It was built from scratch using only the SDL2 library to display the synthesized images.
When the code is compiled as-is, it will generate a 3D image of a purple Utah teapot.
However, the project also includes other models to synthesize shown in the images below,
including the matching teacup and a spoon.

## Images

Here are some of the images I created with this rasterizer:

### Utah Teapot
![Render of the Utah Teapot](/sample_images/Utah_Teapot.png "Utah Teapot render")

### Teacup
![Render of a teacup](/sample_images/Teacup.png "Teacup render")

### Spoon
![Render of a spoon](/sample_images/Spoon.png "Spoon render")

## References

This project was created from a few different resources around the internet:

* [Computer Graphics from Scratch](https://gabrielgambetta.com/computer-graphics-from-scratch/) by Gabriel Gambetta 
  for the basics of rasterization algorithms.
* [Tiny Renderer](https://github.com/ssloy/tinyrenderer/wiki) by Dmitri V. Sokolov to fill in some extra gaps for 
  rasterization in C++.
* [3D Math Primer for Graphics and Game Development](https://gamemath.com/) by Fletcher Dunn and Ian Parberry for help 
  translating linear algebra into C++ code.
* [Beginning Game Programming with SDL v2.0](https://lazyfoo.net/tutorials/SDL/index.php) from Lazy Foo Productions for
  information on SDL2 windowing.

I greatly appreciate all of these authors for creating these projects!

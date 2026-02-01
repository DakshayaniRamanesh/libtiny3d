# 3D Wireframe Renderer with Lighting and Bézier Animation

This C-based project renders animated 3D wireframe models using anti-aliased lines, Lambertian lighting, and Bézier-curve-based animation. The output is a series of `.pgm` image frames that can be combined into a GIF or video. The ouptut of the first 2 tasks are ocnverted to .png format.

## ✨ Features

- Anti-aliased wireframe rendering
- Circular viewport clipping
- Lambert lighting (multiple light sources)
- Perspective projection (frustum matrix)
- Smooth animation via cubic Bézier paths
- OBJ file loading and edge extraction
- Support for multiple animated objects

---

## 📁 Directory Structure

 libtiny3d/
 |_include/
 | |_animation.h
 | |_canvas.h
 | |_math3d.h
 | |_renderer.h
 | |_lighting.h
 |_src/
 | |_canvas.c
 | |_math3d.c
 | |_renderer.c
 | |_lighting.c
 | |_animation.c
 |_tests/
 | |_main1.c
 | |_test_math.c
 | |_test_pipeline.c
 | |_ visual
 |   tests/
 |_demo/
 | |_main.c
 |_documentation/
 | |_GroupXX_reort.pdf
 |-README.md
 |-Makefile
 
for the first task the compilecode is
  gcc -Iinclude tests/main1.c src/canvas.c -lm -o task
  run- ./task
for the second task the compile code is
  gcc -Iinclude tests/test_math.c src/canvas.c src/math3d.c -o task2 -lm
  run- ./task2

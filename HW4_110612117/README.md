# NYCU Introduction to Computer Graphics 2023 Fall HW4 — Geometry Shader Explode

StudentID: 110612117  
Name: Chung-Yu Chang (張仲瑜)

## Introduction
This assignment experiments with geometry shaders by “exploding” a textured deer model. Two pipelines run in parallel: a baseline textured render and a geometry-shader variant that offsets triangles along their averaged face normal to create a pulsing explode effect. Animation speed and toggles are controlled at runtime via hotkeys.

## What I implemented
- Scene setup, animation loop, and key handling in [HW4/main.cpp](HW4/main.cpp): rotation toggle, explode toggle with bouncing distance (0–4), speed adjustments, and optional bypass of the geometry shader.
- Baseline textured pass using [HW4/shaders/deer.vert](HW4/shaders/deer.vert) and [HW4/shaders/deer.frag](HW4/shaders/deer.frag).
- Geometry-shader explode pass via [HW4/shaders/normal.vert](HW4/shaders/normal.vert), [HW4/shaders/normal.geom](HW4/shaders/normal.geom), and [HW4/shaders/normal.frag](HW4/shaders/normal.frag): accumulates a flat normal per triangle, offsets positions by `explodeFactor`, and samples the diffuse texture.
- Texture loading for `obj/Deer_diffuse.jpg`; VAO/VBO setup for positions, normals, and texcoords; depth testing enabled.

## Controls
- `R`: toggle model rotation (90°/s around Y). Prints "KEY R PRESSED".
- `E`: toggle explode animation (distance bounces between 0 and 4). Prints "KEY E PRESSED".
- `S`: toggle spin flag (currently unused in shader; prints "KEY S PRESSED").
- `G`: enable/disable the geometry-shader pass (shows only baseline when off). Prints "KEY G PRESSED".
- `P`: pause/resume explode distance update. Prints "KEY P PRESSED".
- `1`: increase explode speed by +1. Prints "KEY 1 PRESSED".
- `2`: decrease explode speed by −1. Prints "KEY 2 PRESSED".
- `Esc`: quit.

## How to run
- Environment: Windows + Visual Studio; GLAD/GLFW/GLM/stb headers and libs are bundled under `HW4/include` and `HW4/lib`.
- Open `HW4_110612117/HW4.sln` in Visual Studio.
- Build and run the `HW4` project (Debug x64 preconfigured). Execute from the `HW4` directory so `obj/Deer.obj` and `obj/Deer_diffuse.jpg` paths resolve.

## Notes
- Camera: `lookAt` from `(0, 4, 4)` toward the origin; perspective 45° FOV, near/far 1–100.
- Explode logic: `explodeFactor` animates with a bounce between 0 and 4; when explode is off, factor resets to 0.
- Geometry shader uses a single flat normal per triangle (average of inputs) for displacement; fragment shader performs texture fetch only (no lighting).
- The `S` spin flag updates an angle but is not currently consumed in the shader; can be extended for rotational explode patterns.
- See https://youtu.be/yfRX7Dna5uw for a demo video.

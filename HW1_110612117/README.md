# NYCU Introduction to Computer Graphics 2023 Fall HW1 — Transformations & Hierarchical Modeling

StudentID: 110612117  
Name: Chung-Yu Chang (張仲瑜)

## Introduction
This homework builds a simple 3D scene in OpenGL: a Big Ben–style clock tower with a spinning clock face plus a rabbit and a tortoise orbiting around it. Models are loaded from OBJ files, transformed hierarchically, and rendered with a basic diffuse shader.

## What I implemented
- Scene assembly and animation in [HW1/main.cpp](HW1/main.cpp): hierarchical transforms for tower base/body/roof, clock face, hands, and two orbiting animals; frame-time logging.
- Input handling: key callbacks to speed up, slow down, or toggle tower rotation.
- Simple per-fragment diffuse lighting in [HW1/fragmentShader.frag](HW1/fragmentShader.frag) with normal transformation in [HW1/vertexShader.vert](HW1/vertexShader.vert).
- Depth testing and back-face culling enabled for correct 3D rendering.

## Controls
- `1` / numpad `1`: double rotation speed of the clock hands and orbiting animals.
- `2` / numpad `2`: halve the rotation speed.
- `3` / numpad `3`: toggle tower body spinning.
- `Esc`: quit.

## How to run
- Environment: Windows + Visual Studio (vcpkg not required; GLAD/GLFW/GLM/stb are bundled under `HW1/include` and `HW1/lib`).
- Open `HW1_110612117/HW1.sln` in Visual Studio.
- Build and run the `HW1` project (Debug x64 is preconfigured). Assets under `HW1/obj` are loaded relative to the working directory; run the binary from the project folder to keep paths intact.

## Notes
- Camera: fixed look-at from `(0, 30, 50)` toward the origin; perspective 45° FOV.
- Lighting: single positional light at `(5, 10, 10)` with gray diffuse shading; no textures used even though texcoords are passed through.
- Animation speed is frame-rate independent (time accumulated per frame). If the scene runs too fast/slow, use keys `1`/`2` to adjust `SpinSpeed` on the fly.

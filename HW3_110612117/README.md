# NYCU Introduction to Computer Graphics 2023 Fall HW3 — Lighting Models & Stylized Shading

StudentID: 110612117  
Name: Chung-Yu Chang (張仲瑜)

## Introduction
This assignment renders a textured deer model with multiple shading styles: Blinn-Phong, Gouraud, Flat (with geometry shader), Toon, a view-dependent Border outline, and a horizontal Dissolve effect. The scene orbits the deer, applying shared material/light uniforms and switching shader programs at runtime via hotkeys.

## What I implemented
- Shading pipeline setup and runtime switching in [ICG_HW3/ICG_HW3/main.cpp](ICG_HW3/ICG_HW3/main.cpp): six shader programs (Blinn-Phong, Gouraud, Flat, Toon, Border, Dissolve), uniform uploads for material/light/camera matrices, and dissolve factor animation.
- Per-shader implementations:
  - Blinn-Phong lighting in [ICG_HW3/ICG_HW3/shaders/Blinn-Phong.vert](ICG_HW3/ICG_HW3/shaders/Blinn-Phong.vert) and [ICG_HW3/ICG_HW3/shaders/Blinn-Phong.frag](ICG_HW3/ICG_HW3/shaders/Blinn-Phong.frag).
  - Gouraud vertex lighting in [ICG_HW3/ICG_HW3/shaders/Gouraud.vert](ICG_HW3/ICG_HW3/shaders/Gouraud.vert) and accumulation in [ICG_HW3/ICG_HW3/shaders/Gouraud.frag](ICG_HW3/ICG_HW3/shaders/Gouraud.frag).
  - Flat shading via geometry normal aggregation in [ICG_HW3/ICG_HW3/shaders/Flat.vert](ICG_HW3/ICG_HW3/shaders/Flat.vert), [ICG_HW3/ICG_HW3/shaders/Flat.geom](ICG_HW3/ICG_HW3/shaders/Flat.geom), and [ICG_HW3/ICG_HW3/shaders/Flat.frag](ICG_HW3/ICG_HW3/shaders/Flat.frag).
  - Toon thresholds and palette mapping in [ICG_HW3/ICG_HW3/shaders/Toon.frag](ICG_HW3/ICG_HW3/shaders/Toon.frag).
  - Border outline using view-angle threshold in [ICG_HW3/ICG_HW3/shaders/Border.vert](ICG_HW3/ICG_HW3/shaders/Border.vert) and [ICG_HW3/ICG_HW3/shaders/Border.frag](ICG_HW3/ICG_HW3/shaders/Border.frag).
  - Dissolve cutoff along the x-axis with progressive factor in [ICG_HW3/ICG_HW3/shaders/Dissolve.vert](ICG_HW3/ICG_HW3/shaders/Dissolve.vert) and [ICG_HW3/ICG_HW3/shaders/Dissolve.frag](ICG_HW3/ICG_HW3/shaders/Dissolve.frag).
- Model/texture loading, VAO setup, and orbiting animation for the deer; depth testing and per-frame rotation control.

## Controls
- `SPACE`: toggle deer rotation (prints "KEY SPACE PRESSED").
- `1`–`4`: switch shading program — 1: Blinn-Phong, 2: Gouraud, 3: Flat, 4: Toon.
- `5`: Border outline effect.
- `6`: Dissolve effect (resets dissolveFactor and plays again).
- `Esc`: quit.

## How to run
- Environment: Windows + Visual Studio; GLAD/GLFW/GLM/stb headers and libs are bundled under `ICG_HW3/ICG_HW3/include` and `ICG_HW3/ICG_HW3/lib`.
- Open `ICG_HW3/ICG_HW3.sln` in Visual Studio.
- Build and run the `ICG_HW3` project (Debug x64 is preconfigured). Run from `ICG_HW3/ICG_HW3` so `obj/deer.obj` and `obj/deer_diffuse.jpg` paths resolve.

## Notes
- Camera: `lookAt` from `(0, 3.5, 3.5)` toward `(0, 0.5, 0)` with 45° perspective, near/far 1–100.
- Light/material: directional-like point at `(10, 10, 10)`; material defaults to white with specular 0.7 and shininess 10.5.
- Animation: deer spins at 45°/s when enabled; dissolveFactor ramps from -25 up to ~25 over time to wipe the mesh along +x.
- Border uses view-normal dot threshold to render a light outline; Toon uses hard thresholds for dark/mid/light bands and spec highlights.
- See [hw3_report_110612117.pdf](/hw3_report_110612117.pdf) for detailed explanations and screenshots.
- See [ICG_HW3_demo.mp4](/ICG_HW3_demo.mp4) for a demo video.
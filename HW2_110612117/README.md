# NYCU Introduction to Computer Graphics 2023 Fall HW2 — Texturing & Deformation

StudentID: 110612117  
Name: Chung-Yu Chang (張仲瑜)

## Introduction
This assignment textures and animates a penguin riding a surfboard. Both models are loaded from OBJ files, textured with diffuse maps, and animated with swing and squeeze motions. Custom shaders handle texture sampling, grayscale/white modes, and a simple vertex-space squeeze deformation.

## What I implemented
- Scene setup and animation in [ICG2023_HW2/HW2/Main.cpp](ICG2023_HW2/HW2/Main.cpp): swing motion for board and penguin(s), optional squeeze on penguin and board, togglable grayscale and white-penguin modes, and support for up to three penguins with offsets and scaling.
- Texture loading for penguin and surfboard diffuse maps; VAO/VBO setup per model.
- Vertex shader squeeze deformation in [ICG2023_HW2/HW2/vertexShader.vert](ICG2023_HW2/HW2/vertexShader.vert) including normal recomputation.
- Fragment shader color/grayscale/white output in [ICG2023_HW2/HW2/fragmentShader.frag](ICG2023_HW2/HW2/fragmentShader.frag).
- Depth testing enabled; camera and perspective configured for a front-facing view of the scene.

## Controls
- `S`: toggle penguin squeeze animation (prints "KEY S PRESSED").
- `G`: toggle grayscale rendering ("KEY G PRESSED").
- `M`: cycle penguin count between 1/2/3 with lateral offsets and size scaling ("KEY M PRESSED").
- `W`: toggle white-penguin mode overriding texture colors ("KEY W PRESSED").
- `B`: toggle surfboard squeeze animation ("KEY B PRESSED").
- `Esc`: quit.

## How to run
- Environment: Windows + Visual Studio (GLAD/GLFW/GLM/stb bundled under `ICG2023_HW2/HW2/include` and `ICG2023_HW2/HW2/lib`).
- Open `ICG2023_HW2/HW2.sln` in Visual Studio.
- Build and run the `HW2` project (Debug x64 preconfigured). Keep the working directory at `ICG2023_HW2/HW2` so model (`obj/*.obj`) and texture (`obj/*_diffuse.jpg`) paths resolve.

## Notes
- Camera: look-at from `(0, 5, 5)` toward `(0, 0.5, 0)`; perspective 45° FOV, near/far 1–100.
- Swing speeds: board/penguin yaw 20°/s, forward-back swing 1.0 units/s; squeeze rates 90°/s for penguin and 135°/s for board.
- Vertex squeeze skews positions along y/z for penguin and x/z for board; normals are recomputed via `transpose(inverse(M))`.
- Textures bound to separate units (board on texture unit 1, penguin on unit 2); grayscale uses luminance weights (0.299, 0.587, 0.114).
- See [HW2_report_110612117.pdf](/HW2_report_110612117.pdf) for detailed explanations and screenshots.

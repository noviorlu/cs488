# README for CS 488/688 Assignment 4: Ray Tracer

## Overview
This project is a ray tracing application for CS 488/688 at the University of Waterloo. It casts primary rays from a camera, intersects them with scene objects (spheres, cubes, triangle meshes), and computes pixel colors using a Phong illumination model with shadows. The ray tracer includes bounding volume hierarchies (BVH) for performance optimization and supports a non-trivial background.

## Features
- **Primary Ray Casting**: Generates rays from the camera for each pixel.
- **Blinn-Phong Illumination**: Supports ambient, diffuse, and specular lighting.
- **Shadows**: Casts shadow rays to calculate occlusion.
- **Bounding Volume Hierarchies**: Improves ray-mesh intersection efficiency.
- **Extra Feature**: MSAA

## Usage
Run the application with a Lua scene file as an argument:

```bash
./A4 <path_to_scene_file>
```

Example:

```bash
./A4 Assets/hier.lua
```

## Lua Scene Functions
- `gr.mesh(name, objfilepath)`: Loads an OBJ file mesh.
- `gr.nh_box` / `gr.nh_sphere`: Creates non-hierarchical box or sphere.
- `gr.cube` / `gr.sphere`: Creates hierarchical primitives.
- `gr.light`: Defines a point light with attenuation.
- `gr.render`: Renders the scene and saves it as a PNG.

## Sample Scenes
- `sample.lua`: Demonstrates all required primitives and features.
- `nonhier.lua`, `macho-cows.lua`, `simple-cows.lua`: Test complex scenes with bounding volumes.

## Deliverables
- **README**: This file.
- **screenshot.png**: Showcase of the ray tracer’s best output.
- **sample.lua** and **sample.png**: Lua script and output image demonstrating key features.
- **Bounding Volume Renderings**: Output image of bounding volumes in `hier.png` or `macho-cows.png`.

## Notes
- **Compile Optimization**: Use `-O2` for better performance.
- **Debugging**: "Epsilon" checks in intersections, optional bounding volume visualization with `#define RENDER_BOUNDING_VOLUMES`, check Macro `#define MSAA`

Enjoy exploring ray tracing with this tool!
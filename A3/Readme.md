# README for CS 488/688 Assignment 3: Puppet

## Overview
This project implements a hierarchical, articulated 3D humanoid model, or "puppet," for CS 488/688 (Introduction to Computer Graphics) at the University of Waterloo. The puppet consists of various body parts connected by joints, allowing interactive control of the puppet's movements. The puppet is constructed using simple geometric primitives and transformed instances, arranged in a hierarchical data structure.

The application also includes interactive lighting, materials, and a Deferred Shading pipeline that supports multiple light sources, making the 3D structure of the puppet clear and visible without a significant performance hit.

### Features:
- **Hierarchical Model**: The puppet is built with a parent-child relationship between body parts.
- **Articulated Joints**: You can control at least 15 degrees of freedom (DOFs) across different joints.
- **Interactive Controls**: Manipulate the puppet with mouse and keyboard input.
- **Deferred Shading Pipeline**: Supports multiple light sources without a drop in framerate.
- **Basic Reset Function**: One "Reset All" function resets position, orientation, and joints together.

## How to Use

### 1. Running the Application
The executable requires a command-line argument specifying the Lua file for the puppet model. Use the following command in the terminal to run the application:

```bash
./A3 <path_to_puppet_file>

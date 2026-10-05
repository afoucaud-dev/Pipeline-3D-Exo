# cpu-engine
CPU Engine is a retro 3D rendering engine that runs exclusively on the CPU using SIMD and multithreading.

It implements a classic fixed-function-style 3D pipeline with modern CPU optimizations such as multithreading, tile-based rendering, and SIMD-friendly math.

The engine uses a deterministic, frame-based execution model where worker threads are explicitly synchronized by the main thread, making rendering behavior predictable and easy to reason about.

## Overview

CPU Engine renders 3D scenes without relying on GPU rasterization or shaders.
All stages — transformation, lighting, triangle setup, rasterization, depth testing, and final presentation — are executed on the CPU.

Rendering is divided into screen-space tiles, which are dynamically distributed across worker threads each frame. This allows efficient load balancing while keeping strict control over synchronization and memory access patterns.

## Key Features

* **CPU-only 3D rendering**
  No GPU pipeline, no shaders, no hardware rasterization.

* **Tile-based multithreaded renderer**
  The screen is split into tiles processed in parallel by worker threads.

* **Frame-synchronous execution model**
  All worker threads start and finish rendering in lockstep with the main thread.

* **Software rasterization pipeline**
  Perspective projection, back-face culling, depth buffering, and triangle filling.

* **Fixed-function style lighting**
  Per-vertex lighting with directional light and ambient term.

* **Deterministic behavior**
  Explicit synchronization ensures predictable frame execution.

* **Retro-oriented design**
  Inspired by early 3D engines, with a focus on clarity and control rather than abstraction.

## Architecture Highlights

### Multithreading model

* Worker threads are created once and persist for the lifetime of the engine.
* Each frame:

  * the main thread signals all workers to start rendering,
  * workers atomically fetch tiles to process,
  * the main thread waits for all workers to finish before presenting the frame.

This model avoids job stealing frameworks and keeps synchronization explicit and understandable.

### Rendering pipeline

1. Entity update and sorting
2. View and projection setup
3. Tile assignment
4. Parallel tile rendering
5. Software rasterization and depth testing
6. CPU-side presentation to the window

## Goals

CPU Engine is not meant to replace modern GPU-based engines.
Its goals are to:

* explore classic 3D rendering techniques
* experiment with CPU-side optimizations
* provide a clear, low-level rendering architecture
* serve as a learning and experimentation platform for software rendering

## Status

CPU Engine is an experimental project under active development.
The API and internal systems may change as features and performance evolve.


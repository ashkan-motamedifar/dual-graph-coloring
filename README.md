# Mesh → Dual Graph Transformation

> A high-performance C implementation for converting 3D triangular meshes into their dual graph representation, with a comparative study of four edge-sorting algorithms and Dijkstra-based graph coloring.

---

## Overview

This project implements a full pipeline for transforming a 3D triangular mesh (`.obj` format) into its **dual graph**: a graph where each face of the original mesh becomes a node, and two nodes are connected if their corresponding faces share an edge.

The pipeline benchmarks four classical data structures and sorting strategies — **Selection Sort**, **Heap Sort**, **AVL Trees**, and **Hash Tables** — to identify adjacent faces efficiently. Once the dual graph is built, **Dijkstra's shortest-path algorithm** is applied to compute distances from a source node, and the result is written as a smooth RGB color gradient for intuitive visual inspection.

> 📄 Full technical report: [`docs/Rapport.pdf`](docs/Rapport.pdf)

---

## Table of Contents

- [Core Concepts](#core-concepts)
- [Pipeline](#pipeline)
- [Algorithms](#algorithms)
- [Mesh Gallery](#mesh-gallery)
- [Project Structure](#project-structure)
- [Getting Started](#getting-started)
- [Usage](#usage)
- [Performance](#performance)
- [License](#license)

---

## Core Concepts

**Dual Graph of a Mesh**

Given a triangulated 3D mesh, the dual graph *G\** is constructed by placing one node at the centroid of each triangular face, then connecting two nodes with an edge if and only if their corresponding faces share a mesh edge. This representation is fundamental in computational geometry, mesh analysis, and computer graphics.

**Centroid Calculation**

Each dual node is positioned at the arithmetic mean of its face's three vertex coordinates:

```
C = (V₀ + V₁ + V₂) / 3
```

**Graph Coloring via Dijkstra**

Once the dual graph is built, Dijkstra's algorithm computes the shortest hop-distance from a chosen source node to every other node. Nodes are then assigned an RGB color interpolated smoothly from the minimum to the maximum distance, producing a heatmap-style visualization that makes the graph's topology immediately readable.

---

## Pipeline

```
Input .obj mesh
       │
       ▼
  Parse vertices & faces
       │
       ▼
  Extract all mesh edges           ┌───────────────────────┐
  (+ reverse counterparts)  ──────►│  Sort edges using one  │
                                   │  of four strategies:   │
                                   │  · Selection Sort      │
                                   │  · Heap Sort           │
                                   │  · AVL Tree            │
                                   │  · Hash Table          │
                                   └──────────┬────────────┘
                                              │
                                              ▼
                              Identify shared edges → adjacency list
                                              │
                                              ▼
                              Compute face centroids → dual vertices
                                              │
                                              ▼
                              Run Dijkstra from source node
                                              │
                                              ▼
                              Assign RGB colors by hop-distance
                                              │
                                              ▼
                              Write output .obj with vertex colors
```

---

## Algorithms

### Edge Sorting Strategies

To identify adjacent faces, all mesh edges (including their reverse counterparts) are sorted so that matching edges between two faces can be detected efficiently. Four strategies are compared:

| Algorithm | Time Complexity | Space | Notes |
|-----------|:--------------:|:-----:|-------|
| **Selection Sort** | O(n²) | O(1) | Simple baseline; impractical for large meshes |
| **Heap Sort** | O(n log n) | O(1) | In-place; good cache behavior on medium meshes |
| **AVL Tree** | O(n log n) | O(n) | Self-balancing BST; in-order traversal yields sorted edges |
| **Hash Table** | O(n) avg | O(n) | Best asymptotic performance; adjacency via direct key lookup |

### Dijkstra's Algorithm

Runs on the dual graph (unweighted — each shared edge has cost 1) to compute the shortest hop-distances from a user-chosen source node. Results are stored as per-vertex RGB values in the output `.obj`, enabling direct visualization in any mesh viewer (MeshLab, Blender, etc.).

---

## Mesh Gallery

The meshes below are included in `assets/meshes/`. GitHub renders `.obj` files as interactive 3D viewers — click any link to explore the model directly in your browser.

> **Input meshes** (original geometry):

| Mesh | Faces | Interactive View |
|------|:-----:|:----------------:|
| Tetrahedron | 4 | [View 3D](assets/meshes/tetrahedron.obj) |
| Octahedron | 8 | [View 3D](assets/meshes/octahedron.obj) |
| Dodecahedron | 12 | [View 3D](assets/meshes/dodecahedron.obj) |
| Pumpkin | — | [View 3D](assets/meshes/pumpkin.obj) |
| Cow | ~5k | [View 3D](assets/meshes/cow.obj) |
| Humanoid | — | [View 3D](assets/meshes/humanoid_tri.obj) |
| Stanford Bunny (10k) | 10 000 | [View 3D](assets/meshes/bunny10k.obj) |

> **Output dual graphs** (generated by this program):

| Dual Graph | Interactive View |
|------------|:----------------:|
| Tetrahedron dual | [View 3D](assets/meshes/out_tetrahedron.obj) |
| Octahedron dual | [View 3D](assets/meshes/out_octahedron.obj) |
| Dodecahedron dual | [View 3D](assets/meshes/out_dodecahedron.obj) |
| Pumpkin dual | [View 3D](assets/meshes/out_pumpkin.obj) |
| Cow dual | [View 3D](assets/meshes/out_cow.obj) |
| Humanoid dual | [View 3D](assets/meshes/out_humanoid_tri.obj) |
| Bunny dual | [View 3D](assets/meshes/out_bunny10k.obj) |

> 💡 **Tip:** Add screenshots of your renders (from MeshLab, Blender, or the SDL2 output) to `docs/images/` to enrich this section further.

---

## Project Structure

```
.
├── assets/
│   └── meshes/                           # Input and output .obj meshes
│       ├── tetrahedron.obj
│       ├── octahedron.obj
│       ├── dodecahedron.obj
│       ├── pumpkin.obj
│       ├── cow.obj
│       ├── humanoid_tri.obj
│       ├── bunny10k.obj
│       └── out_*.obj                     # Generated dual graphs
├── docs/
│   └── Rapport.pdf                       # Full technical report
├── src/
│   ├── dual_graph_of_mesh_st1_final.c    # Main source (parser, algorithms, Dijkstra, writer)
│   └── vendor/
│       └── hashmap.h                     # Hash map implementation
├── Makefile
├── LICENSE
└── README.md
```

---

## Getting Started

### Prerequisites

- GCC (C99 or later) — `gcc --version`
- GNU Make — `make --version`

No external libraries are required for the core pipeline.

### Build

```bash
git clone https://github.com/ashkan-motamedifar/graph-dual-transformation.git
cd graph-dual-transformation
make build
```

This compiles `src/dual_graph_of_mesh_st1_final.c` with `-Wall -g` and produces the `exefile` binary.

```bash
make clean   # remove compiled binary
```

---

## Usage

```
./exefile <input.obj> <output.obj> <algorithm> <color>
```

| Argument | Accepted values | Description |
|----------|----------------|-------------|
| `input.obj` | any path | Input triangular mesh in Wavefront OBJ format |
| `output.obj` | any path | Output dual graph in OBJ format (vertex colors included) |
| `algorithm` | `selectionsort` · `heapsort` · `avltree` · `hashtable` | Edge-sorting strategy |
| `color` | `y` · `n` | Apply Dijkstra-based RGB coloring to the output |

### Examples

```bash
# Tetrahedron — AVL tree, with coloring
./exefile assets/meshes/tetrahedron.obj out_tetrahedron.obj avltree y

# Stanford Bunny (10k faces) — Hash table, no coloring
./exefile assets/meshes/bunny10k.obj out_bunny.obj hashtable n

# Cow mesh — Heap sort, with coloring
./exefile assets/meshes/cow.obj out_cow.obj heapsort y
```

Or use Make to build and run the default configuration in one step:

```bash
make
```

---

## Performance

The four sorting strategies are evaluated on meshes of increasing complexity. As expected, the hash-table approach dominates on large meshes (O(n) average), while selection sort becomes prohibitively slow beyond a few thousand faces.

| Mesh | Faces | Selection Sort | Heap Sort | AVL Tree | Hash Table |
|------|:-----:|:--------------:|:---------:|:--------:|:----------:|
| Tetrahedron | 4 | ✅ fast | ✅ fast | ✅ fast | ✅ fast |
| Dodecahedron | 12 | ✅ fast | ✅ fast | ✅ fast | ✅ fast |
| Cow | ~5k | 🟡 slow | ✅ | ✅ | ✅ fastest |
| Bunny | 10k | 🔴 very slow | ✅ | ✅ | ✅ fastest |

> Full benchmark results and algorithmic analysis are available in [`docs/Rapport.pdf`](docs/Rapport.pdf).

---

## License

This project is licensed under the **MIT License** — see [`LICENSE`](LICENSE) for full details.

You are free to use, modify, and distribute this software for any purpose, provided the original copyright notice and this license are retained in all copies or derivative works.

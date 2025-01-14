# Graph Dual Transformation

This project focuses on transforming a mesh into a dual graph. The transformation involves calculating centroids for each face in the mesh and connecting these centroids to form the edges of the dual graph. The project also evaluates various edge sorting algorithms, including selection sort, heap sort, AVL trees, and hash tables, to identify adjacent faces efficiently. The final step involves applying **Dijkstra's algorithm** to color the graph based on the shortest distances between nodes.

## Table of Contents

- [Introduction](#introduction)
- [Key Concepts](#key-concepts)
- [Project Workflow](#project-workflow)
- [Algorithms](#algorithms)
  - [Edge Sorting Algorithms](#edge-sorting-algorithms)
  - [Graph Coloring](#graph-coloring)
- [Installation](#installation)
- [Usage](#usage)
- [License](#license)

## Introduction

The project aims to demonstrate the process of transforming a mesh into a dual graph. It introduces key concepts like centroid calculation, edge formation, and graph coloring, all essential in understanding graph theory and mesh processing. The goal is to create an efficient method for generating and visualizing a dual graph based on a given mesh structure.

## Key Concepts

- **Mesh to Dual Graph Transformation**: The process of converting a 3D mesh into a dual graph by calculating centroids of mesh faces and connecting them based on adjacency.
- **Centroids**: A point that represents the average position of all vertices of a face in the mesh.
- **Graph Dual**: A graph formed by placing a vertex at the centroid of each face of the mesh and connecting vertices if the faces share an edge.
- **Graph Coloring**: Using an algorithm like Dijkstra’s to assign colors to nodes based on their shortest distance from a start node.

## Project Workflow

1. **Mesh to Dual Graph Transformation**:
   - Calculate the centroid for each face in the mesh.
   - Form edges in the dual graph by connecting centroids of adjacent faces.

2. **Edge Sorting to Find Adjacent Faces**:
   - Sort the edges of the mesh using various algorithms (Selection Sort, Heap Sort, AVL Tree, Hash Table).
   - Use sorted edges to identify adjacent faces and form the dual graph.

3. **Graph Coloring with Dijkstra's Algorithm**:
   - Apply Dijkstra's algorithm to calculate the shortest distance between nodes in the dual graph.
   - Color the graph based on the distances, enhancing visual clarity and understanding of the graph’s structure.

## Algorithms

### Edge Sorting Algorithms

The project compares four edge sorting algorithms to efficiently identify adjacent faces:

1. **Selection Sort**: A simple algorithm with O(n²) complexity.
2. **Heap Sort**: An optimized algorithm with O(n log n) complexity.
3. **AVL Trees**: Self-balancing binary search trees that offer efficient sorting with O(log n) complexity.
4. **Hash Tables**: Use a hash function for O(1) average time complexity in edge sorting.

### Graph Coloring

To visually represent the dual graph and the shortest distances between nodes, **Dijkstra’s algorithm** is used for graph coloring. The algorithm computes the shortest path from a source node to all other nodes in the graph, and nodes are colored based on their distance from the source.

## Installation

To install and run this project locally, follow these steps:

1. Clone the repository:
   ```bash
   git clone https://github.com/ashkan-motamedifar/graph-dual-transformation.git

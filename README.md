# Traveling Salesman Problem: Branch and Bound Analysis

## Project Description
This repository contains a technical implementation and comparative performance analysis of the **Branch and Bound (B&B)** algorithm applied to the **Traveling Salesman Problem (TSP)** and the **Asymmetric Traveling Salesman Problem (ATSP)**. The project evaluates how different tree traversal strategies and bounding techniques affect computational complexity, execution time, and memory management.

## Core Features
The solver implements three distinct tree traversal strategies to provide a comprehensive analysis of the search space:

* **Breadth-First Search (BFS):** Explores the tree layer by layer. This strategy serves as a baseline for measuring the impact of more advanced prioritization techniques.
* **Depth-First Search (DFS):** Prioritizes depth, significantly reducing the memory footprint compared to BFS, though often requiring more steps to find the optimal solution without strong pruning.
* **Best-First Search (Least Cost):** Uses a priority queue to explore nodes with the lowest **Lower Bound (LB)** first. This strategy is designed to minimize the number of visited nodes by focusing on the most promising paths.

## Optimization Techniques
To enhance performance and mitigate the combinatorial explosion inherent in NP-hard problems, the following optimizations were implemented:

1. **Upper Bound (UB) Integration:** An initial upper bound is established using the **Nearest Neighbor (NN)** heuristic. This allows the algorithm to prune non-promising branches immediately, preventing unnecessary exploration.
2. **Incremental Matrix Reduction:** Each node performs a cost matrix reduction to calculate a tight **Lower Bound (LB)**, which is critical for effective pruning.
3. **Symmetry Analysis:** The project examines how cost symmetry impacts bounding efficiency, noting that asymmetric matrices often allow for more aggressive pruning.

## Performance Results
The research conducted on both generated datasets and standard **TSPLIB** benchmarks yielded the following conclusions:

* **Strategy Comparison:** **Best-First Search** demonstrated superior efficiency in terms of nodes visited, particularly when combined with a tight Upper Bound.
* **Scaling and Limits:** The implementation successfully processed instances up to **n=24**. Beyond this threshold, the exponential growth of the search space led to significant computational overhead.
* **Resource Constraints:** The primary bottleneck for high-dimensional instances was identified as memory consumption during **deep-copying of reduction matrices**, rather than raw CPU processing power.
* **Heuristic Impact:** The introduction of the Upper Bound reduced the execution time of the BFS strategy by approximately **98%** for small-to-medium-sized instances.

## Technical Specifications
* **Programming Language:** [e.g., C++17]
* **Environment:** [e.g., Visual Studio 2022]
* **Benchmark Data:** TSPLIB (gr17, gr21, gr24, etc.)
* **Operating System:** Windows 10

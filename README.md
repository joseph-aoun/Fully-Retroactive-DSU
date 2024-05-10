
# Fully Retroactive Disjoint Set Union (DSU)

## Overview

This project implements a fully retroactive DSU, which allows dynamic connectivity queries not just in the current state but at any past time. It leverages a combination of link/cut trees and splay trees to manage connections and retroactive operations efficiently.

## Features

- **Union Operation:** Merge two sets and record the operation with a timestamp.
- **Erase Operation:** Remove a previously recorded operation by timestamp, effectively rolling back the DSU state to before that operation.
- **Connectivity Query:** Determine if two elements were in the same set at a given time.
- **Automatic Stress Testing:** Perform a series of random operations to validate the performance and correctness of the data structure under different scenarios.

## Components

- **Splay Tree:** A balanced binary search tree where access operations are frequently restructured to improve average access time.
- **Link/Cut Tree:** A data structure that dynamically represents a forest of trees, allowing for the quick union and cut operations along with path queries.
- **Retroactive DSU:** Extends traditional DSU functionalities to handle time-traveling queries and modifications.

## Compilation

This project is written in C++ and can be compiled with any standard C++ compiler supporting C++17 or later. For example, to compile the program with `g++`, you can use the following command:

```bash
g++ -std=c++17 -o retroactive_dsu main.cpp
```

## Running

After compilation, you can run the program directly from the command line:

```bash
./retroactive_dsu
```

## Testing

The source code includes two test functions:
- `test()`: Tests the efficiency of basic n*log(n) operations.
- `stress_test()`: Runs a series of operations defined in `num_ops` to evaluate performance under stress.

To enable testing, uncomment the respective function calls in the `main()` function before compiling.

## Output

The program outputs the results of user interactions and test runs to the console. To direct the output to a file, you can redirect it in the command line:

```bash
./retroactive_dsu > output.txt
```

## Closing

This implementation is part of an academic project exploring advanced data structures. Contributions and optimizations are welcome.


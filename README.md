# Fully Retroactive DSU

## Overview

This repository contains the implementation of a Fully Retroactive Disjoint Set Union (DSU) using persistent data structures. This innovative approach allows for inserting and deleting operations at any point in time and querying past states efficiently.

## Features

- **Make Set**: Initialize a new element as a new set.
- **Union**: Merge two sets at a given timestamp.
- **Delete**: Remove operations from the history, adjusting the data structure accordingly.
- **Query**: Check if two elements are in the same set as of a given time.

## Naive Time Complexity Analysis:
Please Note that we are using a map instead of an unorderd_map (hashtable) so we will incurr a log(n) overhead due to the nature of the map implementation in c++.
The log(n) overhead can be removed by replacing the map with a hashtable but now the analysis will be in O() expected time. 
- **Make Set**: O(log(n))
- **Union**: O(log(n))
- **Delete**: O(log(n))
- **Query**:
  - *Slow*: O(2^n)
  - *Fast*: O(nlog(m))
n: number of vertices.
m: number of edges (total number of operations).

## Potential Usage
- **Helpful in Analysis of Network Connectivity**
- **Undo and Redo operations in interactive systems**

## Compilation

To compile the project, use the following command:

```bash
g++ -std=c++17 main.cpp -o retroactive_dsu
```
Then run the executable file using:
```bash
./retroactive_dsu
```

## Contributions:
- **Joseph Aoun**
- **Hashem Khoder**
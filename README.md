# Fully Retroactive DSU

## Overview

This repository contains the implementation of a Fully Retroactive Disjoint Set Union (DSU) using persistent data structures. This innovative approach allows for inserting and deleting operations at any point in time and querying past states efficiently.

## Features

- **Make Set**: Initialize a new element as a new set.
- **Union**: Merge two sets at a given timestamp.
- **Delete**: Remove operations from the history, adjusting the data structure accordingly.
- **Query**: Check if two elements are in the same set as of a given time.

## Usage

### Compilation

To compile the project, use the following command:

```bash
g++ -std=c++17 main.cpp -o retroactive_dsu

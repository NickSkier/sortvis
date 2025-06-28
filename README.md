# SortVis - Sorting Algorithm Visualizer

A command-line application that provides a graphical visualization of common sorting algorithms using the ncurses library.

## Features

* Supported algorithms:
  * Bubble Sort (`-b`, `--bubble`)
  * Selection Sort (`-s`, `--selection`)
  * Double Selection Sort (`-ds`, `--double-selection`)
  * Insertion Sort (`-i`, `--insertion`)
* Dynamic chart size based on input argument.
* Adjustable animation speed (delay between steps).
* Real-time display of statistics: comparisons, swaps, and array accesses.
* Highlights elements currently being examined or moved during the sort.

## Building

To build and run this project, you need:

* A C++ compiler (supporting C++11 or later).
* The ncurses development library installed on your system.

Download `sortvis.cpp`.

```bash
curl -O https://raw.githubusercontent.com/NickSkier/sortvis/refs/heads/master/sortvis.cpp
```

Compile the code using your C++ compiler, linking the ncurses library:

```bash
g++ sortvis.cpp -o sortvis -lncurses
```

## Usage

Run the compiled executable from your terminal. The program requires at least one argument to specify the sorting algorithm.

```bash
./sortvis <algorithm> [size] [animation_delay]
```

* [algorithm]: Required. Specifies the sorting algorithm to visualize. Choose one of the following flags:
  * `-b`or `--bubble`
  * `-s`or `--selection`
  * `-ds`or `--double-selection`
  * `-i`or `--insertion`
* [size]: Optional. The number of elements in the array to sort. Must be a positive integer. Defaults to 20 if not specified.
* [animation_delay]: Optional. The delay in milliseconds between visualization steps, controls visualization speed. Must be a non-negative integer. Defaults to 0 (no delay) if not specified.

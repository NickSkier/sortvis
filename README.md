# SortVis - Sorting Algorithm Visualizer

A command-line application that provides a graphical visualization of common sorting algorithms using the ncurses library.

## Features

* Supported algorithms:
  * Bubble Sort (`bubble`)
  * Shaker Sort (`shaker`)
  * Selection Sort (`selection`)
  * Double Selection Sort (`double-selection`)
  * Insertion Sort (`insertion`)
* Dynamic chart size based on input argument.
* Adjustable animation speed (delay between steps).
* Abbility to set specific shuffle seed.
* Disable visualization to test how fast an algorithm can sort an array on your machine.
* Real-time display of statistics: comparisons, swaps, and array accesses.
* Highlights elements currently being examined or moved during the sort.

## Building

To build and run this project, you need:

* A C++ compiler (supporting C++17 or later).
* The ncurses and CLI11 development libraries installed on your system.

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
./sortvis <algorithm> --size [size] --delay [animation_delay] --seed [shuffle_seed] --no-vis
```

* [algorithm]: Required. Specifies the sorting algorithm to visualize. Choose one of the following flags:
  * `bubble`
  * `shaker`
  * `selection`
  * `double-selection`
  * `insertion`
* [size]: Optional. The number of elements in the array to sort. Must be a positive integer. Defaults to 20 if not specified.
* [animation_delay]: Optional. The delay in milliseconds between visualization steps, controls visualization speed. Must be a non-negative integer. Defaults to 0 (no delay) if not specified.
* [seed]: Optional. The seed, controls shuffle randomazer seed.
* `--no-vis` Optional. Disables visualization.

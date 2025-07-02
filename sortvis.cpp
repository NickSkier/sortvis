#include <algorithm>
#include <cstddef>
#include <functional>
#include <iostream>
#include <optional>
#include <string>
#include <vector>
#include <random>
#include <numeric>
#include <map>
#include <CLI/CLI.hpp>
#include <ncurses.h>

#define PROJECT_NAME "sortvis"
#define HIGHLIGHT_WHITE 7      // White bar
#define HIGHLIGHT_GREEN 2      // Green
#define HIGHLIGHT_BLUE 4       // Blue
#define HIGHLIGHT_RED 1        // Red
#define HIGHLIGHT_MAGENTA 5    // Magenta
#define HIGHLIGHT_YELLOW 11    // Yellow

void initNcurses();
void printChart(const std::vector<int> &vec, const size_t highlightGreen = -1, const size_t highlightBlue = -1, const size_t highlightRed = -1, const size_t highlightMagenta = -1, const size_t highlightYellow = -1, const size_t animDelay = 0);
void printNumberBar(const std::vector<int> &vec);
void printStats(const std::string &sortName, const size_t &comparisonsCounter, const size_t arrayAccessCounter, const size_t swapCounter);
void printFinalStats(const std::string &sortName, const size_t &comparisonsCounter, const size_t arrayAccessCounter, const size_t swapCounter);
std::vector<int> generateShuffledVector(const size_t &size, std::optional<int> seed = std::nullopt);
void selectionSort(std::vector<int> &vec);
void doubleSelectionSort(std::vector<int> &vec);
void bubbleSort(std::vector<int> &vec);
void shakerSort(std::vector<int> &vec);
void insertionSort(std::vector<int> &vec);

int main(int argc, char **argv) {
    CLI::App app{"SortVis: A command-line sorting algorithm visualizer."};
    app.footer("Example: " + std::string(PROJECT_NAME) + " bubble --size 50 --delay 10");
    app.fallthrough();

    size_t size = 20;
    size_t animationDelay = 0;
    std::optional<int> seed;

    std::map<std::string, std::function<void(std::vector<int>&)>> sortFunctions;
    sortFunctions["bubble"] = bubbleSort;
    sortFunctions["shaker"] = shakerSort;
    sortFunctions["selection"] = selectionSort;
    sortFunctions["double-selection"] = doubleSelectionSort;
    sortFunctions["insertion"] = insertionSort;

    app.require_subcommand(1);
    for (auto const& [name, func] : sortFunctions) {
        app.add_subcommand(name, "Use the " + name + " sort algorithm.");
    }

    app.add_option("-s,--size", size, "The number of elements to sort")->default_val(20);
    app.add_option("-d,--delay", animationDelay, "Animation delay in millisecons. Controls the animation speed")->default_val(0);
    app.add_option("--seed", seed, "[Optional] An integer seed for the elements array random shuffle");

    try {
        app.parse(argc, argv);
    }
    catch (const CLI::ParseError &e) {
        return app.exit(e);
    }

    std::vector<int> vec = generateShuffledVector(size, seed);

    initNcurses();

    printChart(vec, -1, -1, -1, -1, -1, animationDelay);

    CLI::App* selectedSortSubcommand = app.get_subcommands().front();
    auto sortFunction = sortFunctions.find(selectedSortSubcommand->get_name());

    if (sortFunction != sortFunctions.end()) {
        sortFunction->second(vec);
    }
    
    endwin();

    return 0;
}

void initNcurses() {
    initscr();
    keypad(stdscr, TRUE);
    noecho();
    curs_set(0);
    cbreak();
    halfdelay(1);
    start_color();

    init_pair(HIGHLIGHT_WHITE, COLOR_WHITE, COLOR_WHITE);
    init_pair(HIGHLIGHT_GREEN, HIGHLIGHT_GREEN, HIGHLIGHT_GREEN);
    init_pair(HIGHLIGHT_BLUE, HIGHLIGHT_BLUE, HIGHLIGHT_BLUE);
    init_pair(HIGHLIGHT_RED, HIGHLIGHT_RED, HIGHLIGHT_RED);
    init_pair(HIGHLIGHT_MAGENTA, HIGHLIGHT_MAGENTA, HIGHLIGHT_MAGENTA);
    init_pair(HIGHLIGHT_YELLOW, HIGHLIGHT_YELLOW, HIGHLIGHT_YELLOW);
}

void printChart(const std::vector<int> &vec, const size_t highlightGreen, const size_t highlightBlue, const size_t highlightRed, const size_t highlightMagenta, const size_t highlightYellow, const size_t animDelay) {
    static size_t animationDelay = animDelay;
    chtype highlightAttr;
    size_t vectorSize = vec.size();
    size_t barHeight, emptyHeight;
    printNumberBar(vec);
    for (size_t i = 0; i <  vectorSize; ++i) {
        if (highlightGreen == i) highlightAttr = COLOR_PAIR(HIGHLIGHT_GREEN);
        else if (highlightBlue == i) highlightAttr = COLOR_PAIR(HIGHLIGHT_BLUE);
        else if (highlightRed == i) highlightAttr = COLOR_PAIR(HIGHLIGHT_RED);
        else if (highlightMagenta == i) highlightAttr = COLOR_PAIR(HIGHLIGHT_MAGENTA);
        else if (highlightYellow == i) highlightAttr = COLOR_PAIR(HIGHLIGHT_YELLOW);
        else highlightAttr = COLOR_PAIR(HIGHLIGHT_WHITE);

        barHeight = vec[i];
        emptyHeight = vectorSize - barHeight;

        attrset(highlightAttr);
        for (size_t j = emptyHeight; j <= vectorSize; ++j) mvprintw(j, i*2, "[]");
        attrset(A_NORMAL);
        for (size_t j = 0; j < emptyHeight-1; ++j) mvprintw(j+1, i*2, "  ");
    }
    refresh();
    napms(animationDelay);
}

void printNumberBar(const std::vector<int> &vec) {
    size_t vectorSize = vec.size();
    attron(A_BOLD);
    for (size_t i = 0; i < vectorSize; ++i) {
        mvprintw(i+1, vectorSize*2+1, "%-*ld", 3, vectorSize-1-i);
    }
    attroff(A_BOLD);
}

void printStats(const std::string &sortName, const size_t &comparisonsCounter, const size_t arrayAccessCounter, const size_t swapCounter) {
    mvprintw(0, 0, "%s - %zu comparisons, %zu swaps, %zu array accesses", sortName.c_str(), comparisonsCounter, swapCounter, arrayAccessCounter);
}

void printFinalStats(const std::string &sortName, const size_t &comparisonsCounter, const size_t arrayAccessCounter, const size_t swapCounter) {
    endwin();
    std::cout << sortName << " - " << comparisonsCounter << " comparisons, " << swapCounter << " swaps, " << arrayAccessCounter <<  " array accesses" << std::endl;
}

std::vector<int> generateShuffledVector(const size_t &size, std::optional<int> seed) {
    std::vector<int> vec(size);
    std::iota(vec.begin(), vec.end(), 0);
    if (seed.has_value()) {
        std::mt19937 engine(seed.value());
        std::shuffle(vec.begin(), vec.end(), engine);
    }
    else {
        std::random_device rd;
        std::mt19937 engine(rd());
        std::shuffle(vec.begin(), vec.end(), engine);
    }
    return vec;
}

void bubbleSort(std::vector<int> &vec) {
    std::string sortName = "Bubble sort";
    size_t comparisonsCounter = 0;
    size_t swapCounter = 0;
    size_t arrayAccessCounter = 0;
    size_t vectorSize = vec.size();
    bool swapped = false;
    for (size_t i = 0; i < vectorSize-1; ++i) {
        swapped = false;
        for (size_t j = 0; j < vectorSize-1-i; ++j) {
            printChart(vec, j, i, j+1);
            printStats(sortName, comparisonsCounter, arrayAccessCounter, swapCounter);
            comparisonsCounter++;
            arrayAccessCounter += 2;
            if (vec[j] > vec[j+1]) {
                std::swap(vec[j], vec[j+1]);
                swapped = true;
                swapCounter++;
                arrayAccessCounter += 4;
            }
        }
        if (!swapped) break;
    }
    printChart(vec);
    printFinalStats(sortName, comparisonsCounter, arrayAccessCounter, swapCounter);
}

void shakerSort(std::vector<int> &vec) {
    std::string sortName = "Shaker sort";
    size_t comparisonsCounter = 0;
    size_t swapCounter = 0;
    size_t arrayAccessCounter = 0;
    size_t vectorSize = vec.size();
    bool swapped;
    size_t left = 0;
    size_t right = vectorSize - 1;
    while (left <= right) {
        swapped = false;
        for (size_t i = left; i < right; ++i) {
            printChart(vec, -1, i, i+1, -1, -1);
            printStats(sortName, comparisonsCounter, arrayAccessCounter, swapCounter);
            comparisonsCounter++;
            arrayAccessCounter += 2;
            if (vec[i] > vec[i+1]) {
                std::swap(vec[i], vec[i+1]);
                swapped = true;
                swapCounter++;
                arrayAccessCounter += 4;
            }
        }
        right--;
        if (!swapped) break;
        swapped = false;
        for (size_t i = right; i > left; --i) {
            printChart(vec, -1, i, i-1, -1, -1);
            printStats(sortName, comparisonsCounter, arrayAccessCounter, swapCounter);
            comparisonsCounter++;
            arrayAccessCounter += 2;
            if (vec[i] < vec[i-1]) {
                std::swap(vec[i], vec[i-1]);
                swapped = true;
                swapCounter++;
                arrayAccessCounter += 4;
            }
        }
        left++;
        if (!swapped) break;
    }
    printChart(vec);
    printFinalStats(sortName, comparisonsCounter, arrayAccessCounter, swapCounter);
}

void selectionSort(std::vector<int> &vec) {
    std::string sortName = "Selection sort";
    size_t comparisonsCounter = 0;
    size_t swapCounter = 0;
    size_t arrayAccessCounter = 0;
    size_t vectorSize = vec.size();
    size_t minIndex;
    size_t lastSwapedIndex = vectorSize;
    for (size_t i = 0; i < vectorSize-1; ++i) {
        minIndex = i;
        for (size_t j = i + 1; j < vectorSize; ++j) {
            printChart(vec, lastSwapedIndex, minIndex, j);
            printStats(sortName, comparisonsCounter, arrayAccessCounter, swapCounter);
            comparisonsCounter++;
            arrayAccessCounter += 2;
            if (vec[j] < vec[minIndex]) minIndex = j;
        }
        if (i != minIndex) {
            std::swap(vec[i], vec[minIndex]);
            lastSwapedIndex = i;
            swapCounter++;
            arrayAccessCounter += 4;
        }
    }
    printChart(vec);
    printFinalStats(sortName, comparisonsCounter, arrayAccessCounter, swapCounter);
}

void doubleSelectionSort(std::vector<int> &vec) {
    std::string sortName = "Double selection sort";
    size_t comparisonsCounter = 0;
    size_t swapCounter = 0;
    size_t arrayAccessCounter = 0;
    size_t vectorSize = vec.size();
    size_t minIndex, maxIndex;
    size_t lastSwapedMinIndex = vectorSize;
    size_t lastSwapedMaxIndex = vectorSize;
    std::string wasSwapedStr = "false";
    for (size_t i = 0; i < vectorSize/2+1; ++i) {
        minIndex = i;
        maxIndex = i;
        for (size_t j = i; j < vectorSize - i; ++j) {
            printChart(vec, lastSwapedMinIndex, minIndex, j, maxIndex, lastSwapedMaxIndex);
            printStats(sortName, comparisonsCounter, arrayAccessCounter, swapCounter);
            comparisonsCounter += 2;
            arrayAccessCounter += 4;
            if (vec[j] < vec[minIndex]) minIndex = j;
            if (vec[j] > vec[maxIndex]) maxIndex = j;
        }
        if (i != minIndex) {
            if (i == maxIndex) maxIndex = minIndex;
            std::swap(vec[i], vec[minIndex]);
            lastSwapedMinIndex = i;
            swapCounter++;
            arrayAccessCounter += 4;
        }
        if (vectorSize - i != maxIndex) {
            std::swap(vec[vectorSize-i-1], vec[maxIndex]);
            lastSwapedMaxIndex = vectorSize-i-1;
            swapCounter++;
            arrayAccessCounter += 4;
        }
    }
    printChart(vec);
    printFinalStats(sortName, comparisonsCounter, arrayAccessCounter, swapCounter);
}

void insertionSort(std::vector<int> &vec) {
    std::string sortName = "Insertion sort";
    size_t comparisonsCounter = 0;
    size_t swapCounter = 0;
    size_t arrayAccessCounter = 0;
    size_t vectorSize = vec.size();
    int key, j;
    for (size_t i = 1; i < vectorSize; ++i) {
        key = vec[i];
        arrayAccessCounter++;
        j = i - 1;
        while (j >= 0 && vec[j] > key) {
            comparisonsCounter++;
            arrayAccessCounter++;
            vec[j+1] = vec[j];
            arrayAccessCounter += 2;
            vec[j] = key; // arrayAccessCounter shouldn't be incremented as this is only used for visualization
            printChart(vec, j, i, j+1);
            printStats(sortName, comparisonsCounter, arrayAccessCounter, swapCounter);
            j--;
        }
        if (j >= 0) {
            comparisonsCounter++;
            arrayAccessCounter++;
        }
        vec[j+1] = key;
        arrayAccessCounter++;
    }
    printChart(vec);
    printFinalStats(sortName, comparisonsCounter, arrayAccessCounter, swapCounter);
}

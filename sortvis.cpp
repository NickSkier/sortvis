#include <algorithm>
#include <cstddef>
#include <iostream>
#include <string>
#include <vector>
#include <random>
#include <numeric>
#include <ncurses.h>

#define PROJECT_NAME "sortvis"
#define HIGHLIGHT_BAR 7  // White bar
#define HIGHLIGHT_1 2    // Green
#define HIGHLIGHT_2 4    // Blue
#define HIGHLIGHT_3 1    // Red

void printVector(std::vector<int> &vec);
void printChart(std::vector<int> &vec, size_t highlight_1 = -1, size_t highlight_2 = -1, size_t highlight_3 = -1, size_t argAnimDelay = 0);
void printNumberBar(std::vector<int> &vec);
void printStats(std::string &sortName, size_t &comparisonsCounter, size_t arrayAccessCounter, size_t swapCounter);
void printFinalStats(std::string &sortName, size_t &comparisonsCounter, size_t arrayAccessCounter, size_t swapCounter);
std::vector<int> generateShuffledVector(size_t size);
std::vector<int> selectionSort(std::vector<int> &vec);
std::vector<int> bubbleSort(std::vector<int> &vec);
std::vector<int> insertionSort(std::vector<int> &vec);

int main(int argc, char **argv) {
    initscr();
    keypad(stdscr, TRUE);
    noecho();
    curs_set(0);
    cbreak();
    halfdelay(1);
    start_color();

    init_pair(HIGHLIGHT_BAR, COLOR_WHITE, COLOR_WHITE);
    init_pair(HIGHLIGHT_1, HIGHLIGHT_1, HIGHLIGHT_1);
    init_pair(HIGHLIGHT_2, HIGHLIGHT_2, HIGHLIGHT_2);
    init_pair(HIGHLIGHT_3, HIGHLIGHT_3, HIGHLIGHT_3);

    size_t size = 20;
    if (argc >= 3) size = std::atoi(argv[2]);
    std::vector<int> vec = generateShuffledVector(size);
    size_t angAnimDelay = 0;
    if (argc == 4) angAnimDelay = std::atoi(argv[3]);
    printChart(vec, -1, -1, -1, angAnimDelay);

    std::string argSortType = argv[1];
    if (argSortType == "--selection" || argSortType == "-s") {
        selectionSort(vec);
    }
    else if (argSortType == "--bubble" || argSortType == "-b") {
        bubbleSort(vec);
    }
    else if (argSortType == "--insertion" || argSortType == "-i") {
        insertionSort(vec);
    }

    endwin();

    return 0;
}

void printVector(std::vector<int> &vec) {
    for (int &val : vec) {   
        printw("%d", val);
    }
    refresh();
}

void printChart(std::vector<int> &vec, size_t highlight_1, size_t highlight_2, size_t highlight_3, size_t argAnimDelay) {
    static size_t animationDelay = argAnimDelay;
    chtype highlightAttr;
    size_t vectorSize = vec.size();
    size_t barHeight, emptyHeight;
    printNumberBar(vec);
    for (size_t i = 0; i <  vectorSize; ++i) {
        if (highlight_1 == i) highlightAttr = COLOR_PAIR(HIGHLIGHT_1);
        else if (highlight_2 == i) highlightAttr = COLOR_PAIR(HIGHLIGHT_2);
        else if (highlight_3 == i) highlightAttr = COLOR_PAIR(HIGHLIGHT_3);
        else highlightAttr = COLOR_PAIR(HIGHLIGHT_BAR);

        barHeight = vec[i];
        emptyHeight = vectorSize - barHeight;

        attrset(A_NORMAL);
        for (size_t j = 0; j < emptyHeight; ++j) mvprintw(j+1, i*2, "  ");
        attrset(highlightAttr);
        for (size_t j = emptyHeight; j < vectorSize; ++j) mvprintw(j+1, i*2, "[]");
    }
    attrset(A_NORMAL);
    refresh();
    napms(animationDelay);
}

void printNumberBar(std::vector<int> &vec) {
    size_t vectorSize = vec.size();
    attron(A_BOLD);
    for (size_t i = 0; i < vectorSize; ++i) {
        mvprintw(i+1, vectorSize*2+1, "%-*ld", 3, vectorSize-1-i);
    }
    attroff(A_BOLD);
}

void printStats(std::string &sortName, size_t &comparisonsCounter, size_t arrayAccessCounter, size_t swapCounter) {
    mvprintw(0, 0, "%s - %zu comparisons, %zu swaps, %zu array accesses", sortName.c_str(), comparisonsCounter, swapCounter, arrayAccessCounter);
}

void printFinalStats(std::string &sortName, size_t &comparisonsCounter, size_t arrayAccessCounter, size_t swapCounter) {
    endwin();
    std::cout << sortName << " - " << comparisonsCounter << " comparisons, " << swapCounter << " swaps, " << arrayAccessCounter <<  " array accesses" << std::endl;
}

std::vector<int> generateShuffledVector(size_t size) {
    std::vector<int> vec(size);

    std::iota(vec.begin(), vec.end(), 0);

    std::random_device rd;
    std::mt19937 engine(rd());
    std::shuffle(vec.begin(), vec.end(), engine);

    return vec;
}

std::vector<int> bubbleSort(std::vector<int> &vec) {
    std::string sortName = "Bubble sort";
    size_t comparisonsCounter = 0;
    size_t swapCounter = 0;
    size_t arrayAccessCounter = 0;
    size_t vectorSize = vec.size();
    for (size_t i = 0; i < vectorSize-1; ++i) {
        for (size_t j = 0; j < vectorSize-1-i; ++j) {
            comparisonsCounter++;
            arrayAccessCounter += 2;
            if (vec[j] > vec[j+1]) {
                std::swap(vec[j], vec[j+1]);
                swapCounter++;
                arrayAccessCounter += 4;
            }
            printChart(vec, j, i, j+1);
            printStats(sortName, comparisonsCounter, arrayAccessCounter, swapCounter);
        }
    }
    printChart(vec);
    printFinalStats(sortName, comparisonsCounter, arrayAccessCounter, swapCounter);
    return vec;
}

std::vector<int> selectionSort(std::vector<int> &vec) {
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
            comparisonsCounter++;
            arrayAccessCounter += 2;
            if (vec[j] < vec[minIndex]) minIndex = j;
            printChart(vec, lastSwapedIndex, minIndex, j);
            printStats(sortName, comparisonsCounter, arrayAccessCounter, swapCounter);
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
    return vec;
}

std::vector<int> insertionSort(std::vector<int> &vec) {
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
    return vec;
}

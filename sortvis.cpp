#include <algorithm>
#include <cstddef>
#include <iostream>
#include <string>
#include <vector>
#include <random>
#include <numeric>
#include <ncurses.h>

#define PROJECT_NAME "sortvis"

void printVector(std::vector<int> &vec);
void printChart(std::vector<int> &vec, size_t argAnimDelay = 0);
void printNumberBar(std::vector<int> &vec);
void printStats(std::string &sortName, size_t &comparisonsCounter, size_t arrayAccessCounter, size_t swapCounter);
void printFinalStats(std::string &sortName, size_t &comparisonsCounter, size_t arrayAccessCounter, size_t swapCounter);
std::vector<int> generateShuffledVector(size_t size);
std::vector<int> selectionSort(std::vector<int> &vec);
std::vector<int> bubbleSort(std::vector<int> &vec);

int main(int argc, char **argv) {
    initscr();
    keypad(stdscr, TRUE);
    noecho();
    curs_set(0);
    cbreak();
    halfdelay(1);

    size_t size = 20;
    if (argc >= 3) size = std::atoi(argv[2]);
    std::vector<int> vec = generateShuffledVector(size);
    size_t angAnimDelay = 0;
    if (argc == 4) angAnimDelay = std::atoi(argv[3]);
    printChart(vec, angAnimDelay);

    std::string argSortType = argv[1];
    if (argSortType == "--selection" || argSortType == "-s") {
        selectionSort(vec);
    }
    else if (argSortType == "--bubble" || argSortType == "-b") {
        bubbleSort(vec);
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

void printChart(std::vector<int> &vec, size_t argAnimDelay) {
    static size_t animationDelay = argAnimDelay;
    std::string fullChartBlock = "[]";
    std::string emptyChartBlock = "  ";
    size_t vectorSize = vec.size();
    printNumberBar(vec);
    for (size_t i = 0; i <  vectorSize; ++i) {
        for (size_t j = 0; j < vectorSize; ++j) {
            if (j+1 >= vectorSize - vec[i]) mvprintw(j+1, i*2, "%s", fullChartBlock.c_str());
            else mvprintw(j+1, i*2, "%s", emptyChartBlock.c_str());

        }
    }
    napms(animationDelay);
    refresh();
}

void printNumberBar(std::vector<int> &vec) {
    size_t vectorSize = vec.size();
        for (size_t i = 0; i < vectorSize; ++i) {
        mvprintw(i+1, vectorSize*2+1, "%-*ld", 3, vectorSize-1-i);
    }
    refresh();
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
            printChart(vec);
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
    for (size_t i = 0; i < vectorSize-1; ++i) {
        minIndex = i;
        for (size_t j = i + 1; j < vectorSize; ++j) {
            comparisonsCounter++;
            arrayAccessCounter += 2;
            if (vec[j] < vec[minIndex]) minIndex = j;
            printChart(vec);
            printStats(sortName, comparisonsCounter, arrayAccessCounter, swapCounter);
        }
        if (i != minIndex) {
            std::swap(vec[i], vec[minIndex]);
            swapCounter++;
            arrayAccessCounter += 4;
        }
    }
    printChart(vec);
    printFinalStats(sortName, comparisonsCounter, arrayAccessCounter, swapCounter);
    return vec;
}

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
void printChart(std::vector<int> &vec);
void printNumberBar(std::vector<int> &vec);
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
    if (argc == 3) size = std::atoi(argv[2]);
    std::vector<int> vec = generateShuffledVector(size);
    printChart(vec);

    std::string argSortType = argv[1];
    if (argSortType == "--selection" || argSortType == "-s") {
        mvprintw(0, 0, "Selection sort");
        selectionSort(vec);
    }
    else if (argSortType == "--bubble" || argSortType == "-b") {
        mvprintw(0, 0, "Bubble sort");
        bubbleSort(vec);
    }
    printChart(vec);

    timeout(-1);
    getch();
    endwin();

    return 0;
}

void printVector(std::vector<int> &vec) {
    for (int &val : vec) {   
        printw("%d", val);
    }
    refresh();
}

void printChart(std::vector<int> &vec) {
    std::string fullChartBlock = "[]";
    std::string emptyChartBlock = "  ";
    size_t vectorSize = vec.size();
    printNumberBar(vec);
    for (size_t i = 0; i <  vectorSize; ++i) {
        for (size_t j = 0; j < vectorSize; ++j) {
            if (j+1 <= vec[vectorSize-1-i]) mvprintw(j+1, i*2, "%s", emptyChartBlock.c_str());
            else mvprintw(j+1, i*2, "%s", fullChartBlock.c_str());
        }
    }
    napms(40);
    refresh();
}

void printNumberBar(std::vector<int> &vec) {
    size_t vectorSize = vec.size();
        for (size_t i = 0; i < vectorSize; ++i) {
        mvprintw(i+1, vectorSize*2+1, "%-*ld", 3, vectorSize-1-i);
    }
    refresh();
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
    for (size_t i = 0; i < vec.size()-1; ++i) {
        for (size_t j = 0; j < vec.size()-1-i; ++j) {
            if (vec[j] > vec[j+1]) std::swap(vec[j], vec[j+1]);
            printChart(vec);
        }
    }
    return vec;
}

std::vector<int> selectionSort(std::vector<int> &vec) {
    size_t minIndex;
    for (size_t i = 0; i < vec.size()-1; ++i) {
        minIndex = i;
        for (size_t j = i + 1; j < vec.size(); ++j) {
            if (vec[j] < vec[minIndex]) minIndex = j;
            printChart(vec);
        }
        std::swap(vec[i], vec[minIndex]);
    }
    return vec;
}

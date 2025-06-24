#include <iostream>
#include <string>
#include <vector>
#include <ncurses.h>

#define PROJECT_NAME "sortvis"

void printVector(std::vector<int> &vec);
void printChart(std::vector<int> &vec);
void printNumberBar(std::vector<int> &vec);
std::vector<int> selectionSort(std::vector<int> &vec);
std::vector<int> bubbleSort(std::vector<int> &vec);

int main(int argc, char **argv) {
    if (argc == 1) {
        std::cout << argv[0] <<  " takes arguments.\n";
        return 1;
    }
    initscr();
    keypad(stdscr, TRUE);
    noecho();
    curs_set(0);
    cbreak();
    nodelay(stdscr, TRUE);

    std::vector<int> vec = {4, 3, 10, 2, 0, 9, 5, 1, 8, 6, 7};

    std::string argSortType = argv[1];
    if (argSortType == "--selection" || argSortType == "-s") {
        printw( "Selection sort");
        selectionSort(vec);
    }
    else if (argSortType == "--bubble" || argSortType == "-b") {
        printw("Bubble sort");
        bubbleSort(vec);
    }
    else if (argSortType == "--chart" || argSortType == "-c") {
        printChart(vec);
    }

    timeout(-1);
    getch();
    endwin();
    
    return 0;
}

void printVector(std::vector<int> &vec) {
    for (int &val : vec) {   
        std::cout << val;
    }
    std::cout << "\n";
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
    refresh();
}

void printNumberBar(std::vector<int> &vec) {
    size_t vectorSize = vec.size();
        for (size_t i = 0; i < vectorSize; ++i) {
        mvprintw(i+1, vectorSize*2+1, "%-*ld", 3, vectorSize-1-i);
    }
    refresh();
}
    
std::vector<int> bubbleSort(std::vector<int> &vec) {
    for (size_t i = 0; i < vec.size()-1; ++i) {
        for (size_t j = 0; j < vec.size()-1-i; ++j) {
            printChart(vec);
            napms(100);
            if (vec[j] > vec[j+1]) {
                std::swap(vec[j], vec[j+1]);
            }
        }
    }
    return vec;
}

std::vector<int> selectionSort(std::vector<int> &vec) {
    size_t minIndex;
    for (size_t i = 0; i < vec.size()-1; ++i) {
        minIndex = i;
        for (size_t j = i + 1; j < vec.size(); ++j) {
            printChart(vec);
            napms(100);
            if (vec[j] < vec[minIndex]) minIndex = j;
        }
        std::swap(vec[i], vec[minIndex]);
    }
    return vec;
}

#include <algorithm>
#include <cctype>
#include <cstddef>
#include <iostream>
#include <string>
#include <vector>
#include <random>
#include <numeric>
#include <ncurses.h>

#define PROJECT_NAME "sortvis"
#define HIGHLIGHT_WHITE 7      // White bar
#define HIGHLIGHT_GREEN 2      // Green
#define HIGHLIGHT_BLUE 4       // Blue
#define HIGHLIGHT_RED 1        // Red
#define HIGHLIGHT_MAGENTA 5    // Magenta
#define HIGHLIGHT_YELLOW 11    // Yellow

void initNcurses();
bool isValidNumericArgument(const char* argStr);
void printChart(const std::vector<int> &vec, const size_t highlightGreen = -1, const size_t highlightBlue = -1, const size_t highlightRed = -1, const size_t highlightMagenta = -1, const size_t highlightYellow = -1, const size_t argAnimDelay = 0);
void printNumberBar(const std::vector<int> &vec);
void printStats(const std::string &sortName, const size_t &comparisonsCounter, const size_t arrayAccessCounter, const size_t swapCounter);
void printFinalStats(const std::string &sortName, const size_t &comparisonsCounter, const size_t arrayAccessCounter, const size_t swapCounter);
std::vector<int> generateShuffledVector(const size_t size);
void selectionSort(std::vector<int> &vec);
void doubleSelectionSort(std::vector<int> &vec);
void bubbleSort(std::vector<int> &vec);
void insertionSort(std::vector<int> &vec);

int main(int argc, char **argv) {
    size_t size = 20;
    size_t argAnimationDelay = 0;

    if (argc == 1) {
        std::cout << PROJECT_NAME << ": You must specify at least one argument\n";
        return 1;
    }
    if (argc >= 3) {
        if (isValidNumericArgument(argv[2])) size = std::stoi(argv[2]);
        else {
            std::cout << PROJECT_NAME << ": must be a numeric argument -- '" << argv[2] << "'\n";
            return 1;
        }
    }
    if (argc >= 4) {
        if (isValidNumericArgument(argv[3])) argAnimationDelay = std::stoi(argv[3]);
        else {
            std::cout << PROJECT_NAME << ": must be a numeric argument -- '" << argv[3] << "'\n";
            return 1;
        }
    }

    std::vector<int> vec = generateShuffledVector(size);

    initNcurses();

    printChart(vec, -1, -1, -1, -1, -1, argAnimationDelay);

    std::string argSortType = argv[1];
    if (argSortType == "--bubble" || argSortType == "-b") {
        bubbleSort(vec);
    }
    else if (argSortType == "--selection" || argSortType == "-s") {
        selectionSort(vec);
    }
    else if (argSortType == "--double-selection" || argSortType == "-ds") {
        doubleSelectionSort(vec);
    }
    else if (argSortType == "--insertion" || argSortType == "-i") {
        insertionSort(vec);
    }
    else {
        endwin();
        std::cout << PROJECT_NAME << ": invalid option '" << argv[1] << "'\n";
        return 1;
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

bool isValidNumericArgument(const char* argStr) {
    if (argStr == nullptr || *argStr == '\0') return false;

    char* endptr;
    strtol(argStr, &endptr, 10);

    if (endptr == argStr) return false;
    while (*endptr != '\0') {
        if (!std::isspace(static_cast<unsigned char>(*endptr))) return false;
        endptr++;
    }
    
    return true;
}

void printChart(const std::vector<int> &vec, const size_t highlightGreen, const size_t highlightBlue, const size_t highlightRed, const size_t highlightMagenta, const size_t highlightYellow, const size_t argAnimDelay) {
    static size_t animationDelay = argAnimDelay;
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

std::vector<int> generateShuffledVector(const size_t size) {
    std::vector<int> vec(size);

    std::iota(vec.begin(), vec.end(), 0);

    std::random_device rd;
    std::mt19937 engine(rd());
    std::shuffle(vec.begin(), vec.end(), engine);

    return vec;
}

void bubbleSort(std::vector<int> &vec) {
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

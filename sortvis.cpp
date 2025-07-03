#include <algorithm>
#include <cctype>
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

class ProgressReporter {
private:
    std::string sortName;
    size_t comparisons = 0;
    size_t accesses = 0;
    size_t swaps = 0;
    bool noVis;
    size_t animationDelay;
    size_t vectorSize = 0;

    void printNumberBar() {
        attron(A_BOLD);
        for (size_t i = 0; i < vectorSize; ++i) {
            mvprintw(i+1, vectorSize*2+1, "%-*ld", 3, vectorSize-1-i);
        }
        attroff(A_BOLD);
    }
public:
    ProgressReporter(bool noVis = false, size_t delay = 20) : noVis(noVis), animationDelay(delay) {
        if (!this->noVis) {
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
    }
    ~ProgressReporter() {
        if (!this->noVis) endwin();
        std::cout << sortName << " sort - " << comparisons << " comparisons, " << swaps << " swaps, " << accesses <<  " array accesses" << std::endl;
    }

    void setSortName(std::string name = "[Unnamed]") {
        name[0] = toupper(name[0]);
        sortName = name;
    }
    void addComparison(size_t count = 1) { comparisons += count; }
    void addArrayAccess(size_t count = 1) { accesses += count; }
    void addSwap(size_t count = 1) { swaps += count; }

    void printProgress(const std::vector<int> &vec, const std::optional<size_t> green = std::nullopt, const std::optional<size_t> blue = std::nullopt, const std::optional<size_t> red = std::nullopt, const std::optional<size_t> magenta = std::nullopt, const std::optional<size_t> yellow = std::nullopt) {
        if (!this->noVis) {
            if (vectorSize == 0) vectorSize = vec.size();
            mvprintw(0, 0, "%s sort - %zu comparisons, %zu swaps, %zu array accesses", sortName.c_str(), comparisons, swaps, accesses);
            printNumberBar();
            chtype highlightAttr;
            size_t barHeight, emptyHeight;
            for (size_t i = 0; i <  vectorSize; ++i) {
                if (green == i) highlightAttr = COLOR_PAIR(HIGHLIGHT_GREEN);
                else if (blue == i) highlightAttr = COLOR_PAIR(HIGHLIGHT_BLUE);
                else if (red == i) highlightAttr = COLOR_PAIR(HIGHLIGHT_RED);
                else if (magenta == i) highlightAttr = COLOR_PAIR(HIGHLIGHT_MAGENTA);
                else if (yellow == i) highlightAttr = COLOR_PAIR(HIGHLIGHT_YELLOW);
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
    }
};

std::vector<int> generateShuffledVector(size_t size, std::optional<int> seed = std::nullopt);
void bubbleSort(std::vector<int> &vec, ProgressReporter& reporter);
void shakerSort(std::vector<int> &vec, ProgressReporter& reporter);
void selectionSort(std::vector<int> &vec, ProgressReporter& reporter);
void doubleSelectionSort(std::vector<int> &vec, ProgressReporter& reporter);
void insertionSort(std::vector<int> &vec, ProgressReporter& reporter);

int main(int argc, char **argv) {
    CLI::App app{"SortVis: A command-line sorting algorithm visualizer."};
    app.footer("Example: " + std::string(PROJECT_NAME) + " bubble --size 50 --delay 10");
    app.fallthrough();

    size_t size = 20;
    size_t animationDelay = 0;
    std::optional<int> seed;
    bool visualizationDisabled = false;

    std::map<std::string, std::function<void(std::vector<int>&, ProgressReporter&)>> sortFunctions;
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

    app.add_flag("--no-vis", visualizationDisabled, "Disable ncurses visualization")->default_val(false);

    try {
        app.parse(argc, argv);
    }
    catch (const CLI::ParseError &e) {
        return app.exit(e);
    }

    std::vector<int> vec = generateShuffledVector(size, seed);

    ProgressReporter reporter(visualizationDisabled, animationDelay);

    CLI::App* selectedSortSubcommand = app.get_subcommands().front();
    std::string sortName = selectedSortSubcommand->get_name();
    auto sortFunction = sortFunctions.find(sortName);

    reporter.setSortName(sortName);

    if (sortFunction != sortFunctions.end()) {
        sortFunction->second(vec, reporter);
    }
    
    return 0;
}

std::vector<int> generateShuffledVector(size_t size, std::optional<int> seed) {
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

void bubbleSort(std::vector<int> &vec, ProgressReporter& reporter) {
    size_t vectorSize = vec.size();
    bool swapped = false;
    for (size_t i = 0; i < vectorSize-1; ++i) {
        swapped = false;
        for (size_t j = 0; j < vectorSize-1-i; ++j) {
            reporter.printProgress(vec, j, i, j+1);
            reporter.addComparison();
            reporter.addArrayAccess(2);
            if (vec[j] > vec[j+1]) {
                std::swap(vec[j], vec[j+1]);
                swapped = true;
                reporter.addSwap();
                reporter.addArrayAccess(4);
            }
        }
        if (!swapped) break;
    }
    reporter.printProgress(vec);
}

void shakerSort(std::vector<int> &vec, ProgressReporter& reporter) {
    size_t vectorSize = vec.size();
    bool swapped;
    size_t left = 0;
    size_t right = vectorSize - 1;
    while (left <= right) {
        swapped = false;
        for (size_t i = left; i < right; ++i) {
            reporter.printProgress(vec, -1, i, i+1, -1, -1);
            reporter.addComparison();
            reporter.addArrayAccess(2);
            if (vec[i] > vec[i+1]) {
                std::swap(vec[i], vec[i+1]);
                swapped = true;
                reporter.addSwap();
                reporter.addArrayAccess(4);
            }
        }
        right--;
        if (!swapped) break;
        swapped = false;
        for (size_t i = right; i > left; --i) {
            reporter.printProgress(vec, -1, i, i-1, -1, -1);
            reporter.addComparison();
            reporter.addArrayAccess(2);
            if (vec[i] < vec[i-1]) {
                std::swap(vec[i], vec[i-1]);
                swapped = true;
                reporter.addSwap();
                reporter.addArrayAccess(4);
            }
        }
        left++;
        if (!swapped) break;
    }
    reporter.printProgress(vec);
}

void selectionSort(std::vector<int> &vec, ProgressReporter& reporter) {
    size_t vectorSize = vec.size();
    size_t minIndex;
    size_t lastSwapedIndex = vectorSize;
    for (size_t i = 0; i < vectorSize-1; ++i) {
        minIndex = i;
        for (size_t j = i + 1; j < vectorSize; ++j) {
            reporter.printProgress(vec, lastSwapedIndex, minIndex, j);
            reporter.addComparison();
            reporter.addArrayAccess(2);
            if (vec[j] < vec[minIndex]) minIndex = j;
        }
        if (i != minIndex) {
            std::swap(vec[i], vec[minIndex]);
            lastSwapedIndex = i;
            reporter.addSwap();
            reporter.addArrayAccess(4);
        }
    }
    reporter.printProgress(vec);
}

void doubleSelectionSort(std::vector<int> &vec, ProgressReporter& reporter) {
    size_t vectorSize = vec.size();
    size_t minIndex, maxIndex;
    size_t lastSwapedMinIndex = vectorSize;
    size_t lastSwapedMaxIndex = vectorSize;
    for (size_t i = 0; i < vectorSize/2+1; ++i) {
        minIndex = i;
        maxIndex = i;
        for (size_t j = i; j < vectorSize - i; ++j) {
            reporter.printProgress(vec, lastSwapedMinIndex, minIndex, j, maxIndex, lastSwapedMaxIndex);
            reporter.addComparison(2);
            reporter.addArrayAccess(4);
            if (vec[j] < vec[minIndex]) minIndex = j;
            if (vec[j] > vec[maxIndex]) maxIndex = j;
        }
        if (i != minIndex) {
            if (i == maxIndex) maxIndex = minIndex;
            std::swap(vec[i], vec[minIndex]);
            lastSwapedMinIndex = i;
            reporter.addSwap();
            reporter.addArrayAccess(4);
        }
        if (vectorSize - i != maxIndex) {
            std::swap(vec[vectorSize-i-1], vec[maxIndex]);
            lastSwapedMaxIndex = vectorSize-i-1;
            reporter.addSwap();
            reporter.addArrayAccess(4);
        }
    }
    reporter.printProgress(vec);
}

void insertionSort(std::vector<int> &vec, ProgressReporter& reporter) {
    size_t vectorSize = vec.size();
    int key, j;
    for (size_t i = 1; i < vectorSize; ++i) {
        key = vec[i];
        reporter.addArrayAccess();
        j = i - 1;
        while (j >= 0 && vec[j] > key) {
            reporter.addComparison();
            reporter.addArrayAccess();
            vec[j+1] = vec[j];
            reporter.addArrayAccess(2);
            vec[j] = key; // reporter.addArrayAccess() shouldn't be incremented as this is only used for visualization
            reporter.printProgress(vec, j, i, j+1);
            j--;
        }
        if (j >= 0) {
            reporter.addComparison();
            reporter.addArrayAccess();
        }
        vec[j+1] = key;
        reporter.addArrayAccess();
    }
    reporter.printProgress(vec);
}

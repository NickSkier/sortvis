#include <iostream>
#include <string>
#include <vector>

#define PROJECT_NAME "sortvis"

void printVector(std::vector<int> &vec);
std::vector<int> selectionSort(std::vector<int> &vec);
std::vector<int> bubbleSort(std::vector<int> &vec);

int main(int argc, char **argv) {
    if (argc == 1) {
        std::cout << argv[0] <<  " takes arguments.\n";
        return 1;
    }

    std::vector<int> vec = {4, 3, 2, 5, 1, 8, 6, 7};

    std::cout << "Initial vector: ";
    printVector(vec);

    std::string argSortType = argv[1];
    if (argSortType == "--selection" || argSortType == "-s") {
        std::cout << "Sorting using selection sort...\n";
        selectionSort(vec);
    }
    else if (argSortType == "--bubble" || argSortType == "-b")
    {
        std::cout << "Sorting using bubble sort...\n";
        bubbleSort(vec);
    }

    std::cout << "Sorted vector: ";
    printVector(vec);

    
    return 0;
}

void printVector(std::vector<int> &vec) {
    for (int &val : vec) {   
        std::cout << val;
    }
    std::cout << "\n";
}
    
std::vector<int> bubbleSort(std::vector<int> &vec) {
    for (size_t i = 0; i < vec.size()-1; ++i) {
        for (size_t j = 0; j < vec.size()-1; ++j) {
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
        for (size_t j = i + 1; j < vec.size()-1; ++j)
            if (vec[j] < vec[minIndex]) minIndex = j;
        std::swap(vec[i], vec[minIndex]);
    }
    return vec;
}

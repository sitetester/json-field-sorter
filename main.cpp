#include "token_sorter.h"

#include <iostream>
#include <string>
#include <chrono>

static std::string getArgValue(const int argc, char* argv[], const std::string& flag) {
    const std::string prefix = "--" + flag + "=";
    for (int i = 1; i < argc; ++i) {
        if (std::string arg = argv[i]; arg.rfind(prefix, 0) == 0) {
            return arg.substr(prefix.size());
        }
    }
    return "";
}

int main(int argc, char* argv[]) {
    std::string inPath = "./data/in";
    std::string outPath = "./data/out";
    std::string sort = "name";

    std::string val = getArgValue(argc, argv, "in-path");
    if (!val.empty()) {
        inPath = val;
    }

    val = getArgValue(argc, argv, "out-path");
    if (!val.empty()) {
        outPath = val;
    }

    val = getArgValue(argc, argv, "sort");
    if (!val.empty()) {
        sort = val;
    }

    if (!TokenSorter::isValidSortField(sort)) {
        std::cerr << "Only `name` or `address` could be used for sorting\n";
        return 1;
    }

    TokenSorter sorter(inPath, outPath, sort);

    std::cout << "Parsing file: " << inPath << " using sort field: " << sort << "\n";
    auto startTime = std::chrono::high_resolution_clock::now();
    sorter.sort();
    auto endTime = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime).count();
    std::cout << "Sorting finished successfully in " << duration << " us.\n";

    return 0;
}

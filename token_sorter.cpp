#include "token_sorter.h"

#include <algorithm>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

namespace {
constexpr auto SORT_FIELD_NAME = "name";
constexpr auto SORT_FIELD_ADDRESS = "address";

std::string extractJsonValue(const std::string& json, const std::string& key) {
    auto keyPos = json.find("\"" + key + "\":");
    if (keyPos == std::string::npos) {
        return "";
    }
    keyPos += key.size() + 3;
    auto valueStart = json.find('"', keyPos);
    if (valueStart == std::string::npos) {
        return "";
    }
    valueStart++;
    auto valueEnd = json.find('"', valueStart);
    if (valueEnd == std::string::npos) {
        return "";
    }
    return json.substr(valueStart, valueEnd - valueStart);
}
}

TokenSorter::TokenSorter(std::string inPath, std::string outPath, std::string sortField)
    : inPath(std::move(inPath))
    , outPath(std::move(outPath))
    , sortField(std::move(sortField)) {}

bool TokenSorter::isValidSortField(const std::string& field) {
    return field == SORT_FIELD_NAME || field == SORT_FIELD_ADDRESS;
}

bool TokenSorter::parseLine(const std::string& line, NameAndAddress& out) {
    std::string name = extractJsonValue(line, "name");
    std::string address = extractJsonValue(line, "address");
    if (name.empty() && address.empty()) {
        return false;
    }
    out.name = name;
    out.address = address;
    return true;
}

std::string TokenSorter::toJson(const NameAndAddress& record) {
    std::ostringstream oss;
    oss << R"({"name":")" << record.name << R"(","address":")" << record.address << "\"}";
    return oss.str();
}

void TokenSorter::sort() const {
    std::ifstream inFile(inPath);
    if (!inFile.is_open()) {
        std::cerr << "Failed to open input file: " << inPath << "\n";
        return;
    }

    std::vector<NameAndAddress> records;
    std::string line;
    while (std::getline(inFile, line)) {
        if (line.empty()) {
            continue;
        }
        NameAndAddress record;
        if (parseLine(line, record)) {
            records.push_back(record);
        }
    }
    inFile.close();

    if (sortField == SORT_FIELD_NAME) {
        std::ranges::sort(records, [](const NameAndAddress& a, const NameAndAddress& b) {
            return a.name < b.name;
        });
    } else if (sortField == SORT_FIELD_ADDRESS) {
        std::ranges::sort(records, [](const NameAndAddress& a, const NameAndAddress& b) {
            return a.address < b.address;
        });
    }

    std::ostringstream data;
    for (size_t i = 0; i < records.size(); ++i) {
        data << toJson(records[i]);
        if (i + 1 < records.size()) {
            data << '\n';
        }
    }

    std::ofstream outFile(outPath, std::ios::trunc);
    if (!outFile.is_open()) {
        std::cerr << "Failed to open output file: " << outPath << "\n";
        return;
    }
    outFile << data.str();
    outFile.close();
}

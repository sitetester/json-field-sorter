#pragma once

#include <string>

struct NameAndAddress {
    std::string name;
    std::string address;
};

class TokenSorter {
public:
    TokenSorter(std::string inPath, std::string outPath, std::string sortField);

    static bool isValidSortField(const std::string& field);
    static bool parseLine(const std::string& line, NameAndAddress& out);
    static std::string toJson(const NameAndAddress& record);

    void sort() const;

    [[nodiscard]] const std::string& getInPath() const { return inPath; }
    [[nodiscard]] const std::string& getOutPath() const { return outPath; }
    [[nodiscard]] const std::string& getField() const { return sortField; }

private:
    std::string inPath;
    std::string outPath;
    std::string sortField;
};

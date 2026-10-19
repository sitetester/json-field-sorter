#include "token_sorter.h"

#include <fstream>
#include <gtest/gtest.h>
#include <string>
#include <filesystem>

static size_t countLines(const std::string& path) {
    std::ifstream file(path);
    std::string line;
    size_t count = 0;
    while (std::getline(file, line)) {
        count++;
    }
    return count;
}

TEST(TokenSorterTest, IsValidSortField) {
    EXPECT_FALSE(TokenSorter::isValidSortField("blah"));
    EXPECT_TRUE(TokenSorter::isValidSortField("name"));
    EXPECT_TRUE(TokenSorter::isValidSortField("address"));
}

TEST(TokenSorterTest, ParseLine) {
    NameAndAddress na;
    EXPECT_TRUE(TokenSorter::parseLine(R"({"name":"Amp","address":"0xabc"})", na));
    EXPECT_EQ(na.name, "Amp");
    EXPECT_EQ(na.address, "0xabc");
}

TEST(TokenSorterTest, ParseLineEmpty) {
    NameAndAddress na;
    EXPECT_FALSE(TokenSorter::parseLine("", na));
}

TEST(TokenSorterTest, SortByName) {
    const std::string OUT_PATH = "./data/test_out_sort_by_name";

    TokenSorter sorter("./data/in", OUT_PATH, "name");
    sorter.sort();

    size_t count = countLines(OUT_PATH);
    ASSERT_GT(count, 0);

    std::ifstream file(OUT_PATH);
    std::string line;
    size_t i = 0;
    while (std::getline(file, line)) {
        if (i == 0) {
            NameAndAddress na;
            TokenSorter::parseLine(line, na);
            EXPECT_EQ(na.name, "Amp");
            EXPECT_EQ(na.address, "0xfF20817765cB7f73d4bde2e66e067E58D11095C2");
        } else if (i == count - 1) {
            NameAndAddress na;
            TokenSorter::parseLine(line, na);
            EXPECT_EQ(na.name, "hoge.finance");
            EXPECT_EQ(na.address, "0xfAd45E47083e4607302aa43c65fB3106F1cd7607");
        }
        i++;
    }

    file.close();
    std::filesystem::remove(OUT_PATH);
}

TEST(TokenSorterTest, SortByAddress) {
    const std::string OUT_PATH = "./data/test_out_sort_by_address";

    TokenSorter sorter("./data/in", OUT_PATH, "address");
    sorter.sort();

    size_t count = countLines(OUT_PATH);
    ASSERT_GT(count, 0);

    std::ifstream file(OUT_PATH);
    std::string line;
    size_t i = 0;
    while (std::getline(file, line)) {
        if (i == 0) {
            NameAndAddress na;
            TokenSorter::parseLine(line, na);
            EXPECT_EQ(na.name, "Reef.finance");
            EXPECT_EQ(na.address, "0xFE3E6a25e6b192A42a44ecDDCd13796471735ACf");
        } else if (i == count - 1) {
            NameAndAddress na;
            TokenSorter::parseLine(line, na);
            EXPECT_EQ(na.name, "FalconSwap Token");
            EXPECT_EQ(na.address, "0xfffffffFf15AbF397dA76f1dcc1A1604F45126DB");
        }
        i++;
    }

    file.close();
    std::filesystem::remove(OUT_PATH);
}

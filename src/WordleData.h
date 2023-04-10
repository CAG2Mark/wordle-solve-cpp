#ifndef WORLDEDATA_H
#define WORLDEDATA_H

#include <string>
#include <unordered_map>
#include <vector>

class WordleData {
private:
    int words_size;
    int guess_words_size;
    std::vector<std::vector<int>> guess_table;

    std::unordered_map<std::string, int> wtoi_map;
    std::vector<std::string> itow_map;

public:
    WordleData(std::string guess_words_file, std::string all_words_file,
        std::string guess_table_file);

    int size() const;
    int size_guess() const;

    int get_guess(int guess, int actual) const;
    const std::vector<std::vector<int>> &get_guess_table() const;
    int get_guess(const std::string &guess, const std::string &actual) const;

    std::string itow(int i) const;
    int wtoi(const std::string &word) const;
};

#endif
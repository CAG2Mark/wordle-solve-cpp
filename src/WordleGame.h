#ifndef WORLDEGAME_H
#define WORLDEGAME_H

#include "WordleData.h"
#include <set>
#include <string>
#include <vector>

class WordleGame {
private:
    std::vector<std::set<int>> boards;
    const WordleData *data;

public:
    WordleGame(int num_boards, const WordleData *data);

    std::string get_best_word() const;
    std::string get_solved_word();
    void filter_words(std::string word, const std::vector<int> &results);
    double compute_entropy(int word) const;
};

#endif
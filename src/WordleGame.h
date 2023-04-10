#ifndef WORLDEGAME_H
#define WORLDEGAME_H

#include "WordleData.h"
#include <set>
#include <string>
#include <vector>

class WordleGame {
private:
    std::vector<std::set<int>> boards;
    std::vector<bool> solved;
    const WordleData *data;
    bool first = true;

public:
    WordleGame(int num_boards, const WordleData *data);

    int get_best_word();
    int get_solved_word();
    void filter_words(std::string word, const std::vector<int> &results);
    double compute_entropy(int word) const;
    double compute_entropy(int word, int board) const;

    std::vector<int> get_definite_results(int guess);
    
    std::vector<bool> get_solved() const;

    bool is_invalid() const;
};

#endif
#ifndef WORLDEUI_H
#define WORLDEUI_H

#include "WordleData.h"
#include "WordleGame.h"
#include <iostream>
#include <string>
#include <vector>

class WordleUI {
    WordleGame game;
    const WordleData *data;

    std::vector<std::string> guesses;
    std::vector<bool> autofilled;
    std::vector<std::vector<std::string>> results_log;

    std::vector<int> solved_turn;

    int last_height = 0;
    bool cleared = true;

    size_t cols;

    size_t boards;

    void clear_board();

    void print_board(size_t highlighted = -1);

public:
    WordleUI(int boards, int cols, const WordleData *data);

    void run();
};

#endif
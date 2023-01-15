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
    std::vector<std::vector<std::string>> results;

    int last_height = 0;
    bool cleared = false;

    size_t boards;

    void clear_board();

    void print_board(size_t highlighted = -1);

public:
    WordleUI(int boards, const WordleData *data) : game(boards, data) {
        this->data = data;
        this->boards = boards;
    }

    void start_game();
};

#endif
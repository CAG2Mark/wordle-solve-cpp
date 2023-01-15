#include "WordleGame.h"
#include <algorithm>
#include <iostream>
#include <math.h>

const std::string BEST_START = "slate";
using namespace std;

WordleGame::WordleGame(int num_boards, const WordleData *data) :
    solved(num_boards, false) {
    this->data = data;

    boards.resize(num_boards);
    for (int i = 0; i < data->size_guess(); ++i) {
        for (auto &s : boards) {
            s.insert(i);
        }
    }
}

double WordleGame::compute_entropy(int word) const {
    auto &guess_table = data->get_guess_table();

    int arrangs[243] = { 0 };
    int total = 0;
    for (const auto &s : boards) {
        for (int ans : s) {
            int res = guess_table[word][ans];
            if (res >= 243) {
                cout << "ERROR!\n";
            }
            ++arrangs[res];
        }
        total += s.size();
    }

    double entropy = 0;
    for (int i = 0; i < 243; ++i) {
        if (arrangs[i] == 0)
            continue;
        double p = (double)arrangs[i] / total;
        entropy -= p * log2(p);
    }
    return entropy;
}

string WordleGame::get_best_word() const {
    if (first) {
        return BEST_START;
    }

    double max_entropy = -1;
    int best_word = -1;

    // Prefer words that are in the possible words list
    set<int> merged = boards[0];
    for (size_t i = 1; i < boards.size(); ++i) {
        merged.insert(boards[i].begin(), boards[i].end());
    }

    for (int i : merged) {
        double entropy = compute_entropy(i);

        if (entropy <= max_entropy)
            continue;

        max_entropy = entropy;
        best_word = i;
    }

    for (int i = 0; i < data->size(); ++i) {
        if (merged.find(i) != merged.end())
            continue;

        double entropy = compute_entropy(i);

        if (entropy <= max_entropy)
            continue;

        max_entropy = entropy;
        best_word = i;
    }

    return data->itow(best_word);
}

string WordleGame::get_solved_word() {
    for (size_t i = 0; i < boards.size(); ++i) {
        auto &s = boards[i];

        if (s.size() != 1)
            continue;
        if (solved[i])
            continue;

        solved[i] = true;

        string ret = data->itow(*s.begin());

        return ret;
    }
    return "";
}

void WordleGame::filter_words(std::string word, const vector<int> &results) {
    first = false;

    int w = data->wtoi(word);
    for (size_t i = 0; i < boards.size(); ++i) {
        if (solved[i])
            continue;

        auto &s = boards[i];

        if (results[i] == 242) solved[i] = true;

        for (auto it = s.begin(); it != s.end();) {
            int w_ = *it;
            if (data->get_guess_table()[w][w_] != results[i])
                s.erase(it++);
            else
                it++;
        }
    }
}

bool WordleGame::is_invalid() const {
    vector<bool> ret;
    for (const auto &s : boards)
        if (s.size() == 0)
            return false;

    return false;
}

std::vector<bool> WordleGame::get_solved() const { return solved; }
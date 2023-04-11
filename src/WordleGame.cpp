#include "WordleGame.h"
#include <algorithm>
#include <iostream>
#include <math.h>
#include <tuple>

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

double WordleGame::compute_entropy(int word, int board) const {
    auto &guess_table = data->get_guess_table();

    double entropy = 0;
    const auto &s = boards[board];

    int total = 0;
    int arrangs[243] = { 0 };
    for (int ans : s) {
        int res = guess_table[word][ans];
        if (res >= 243) {
            cout << "ERROR!\n";
        }
        ++arrangs[res];
    }
    total += s.size();

    for (int i = 0; i < 243; ++i) {
        if (arrangs[i] == 0)
            continue;
        double p = (double)arrangs[i] / total;
        double bits = -log2(p);
        double val = p * bits;

        entropy += val;
    }

    return entropy;
}

double WordleGame::compute_entropy(int word) const {
    double entropy = 0;
    for (size_t i = 0; i < boards.size(); ++i) {
        entropy += compute_entropy(word, i);
    }
    return entropy;
}

double WordleGame::compute_sd(int word) const {
    auto &guess_table = data->get_guess_table();

    double var = 0;
    for (size_t i = 0; i < boards.size(); ++i) {
        double en = compute_entropy(word, i);

        double v = 0;
        int total = 0;
        int arrangs[243] = { 0 };

        const auto &s = boards[i];

        for (int ans : s) {
            int res = guess_table[word][ans];
            if (res >= 243) {
                cout << "ERROR!\n";
            }
            ++arrangs[res];
        }
        total += s.size();
    

    for (int i = 0; i < 243; ++i) {
        if (arrangs[i] == 0)
            continue;
        double p = (double)arrangs[i] / total;
        double bits = -log2(p);
            double val = (bits - en);

            v += arrangs[i] * val * val;
    }
        v /= total;
        var += v;
    }

    return sqrt(var);
}

int WordleGame::get_best_word() {
    if (first) {
        return data->wtoi(BEST_START);
    }

    int solved = get_solved_word();
    if (solved != -1)
        return solved;

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

    return best_word;
}

// when entropy is 0 for a guess, it is always possible to
// know the result of the guess.
vector<int> WordleGame::get_definite_results(int guess) {
    vector<int> ret(boards.size(), -1);

    for (size_t i = 0; i < boards.size(); ++i) {
        if (compute_entropy(guess, i))
            continue;
        ret[i] = data->get_guess(guess, *boards[i].begin());
    }

    return ret;
}

int WordleGame::get_solved_word() {
    for (size_t i = 0; i < boards.size(); ++i) {
        auto &s = boards[i];

        if (s.size() != 1)
            continue;
        if (solved[i])
            continue;

        solved[i] = true;

        int ret = *s.begin();

        return ret;
    }
    return -1;
}

void WordleGame::filter_words(std::string word, const vector<int> &results) {
    first = false;

    int w = data->wtoi(word);
    for (size_t i = 0; i < boards.size(); ++i) {
        if (solved[i])
            continue;

        auto &s = boards[i];

        if (results[i] == 242)
            solved[i] = true;

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
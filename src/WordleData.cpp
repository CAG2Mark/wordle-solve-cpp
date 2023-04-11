#include "WordleData.h"

#include <fstream>
#include <sstream>
#include <iostream>
#include <string>

using namespace std;

int guess_to_int(const string &guess) {
    int ret = 0;
    int pow = 1;
    for (int i = guess.size() - 1; i >= 0; --i) {
        ret += (guess[i] - '0') * pow;
        pow *= 3;
    }
    return ret;
}

string guess_to_string(int guess) {
    string ret = "";
    for (int i = 0; i < 5; ++i) {
        ret = to_string(guess % 3) + ret;
        guess /= 3;
    }
    return ret;
}

WordleData::WordleData(string guess_words_file, string all_words_file, string guess_table_file) {
    ifstream guess_words_f(guess_words_file);
    ifstream all_words_f(all_words_file);

    string line;
    int i = 0;
    while (getline(guess_words_f, line)) {
        itow_map.push_back(line);
        wtoi_map[line] = i;
        ++i;
    }

    guess_words_size = i;

    // Ensure guess words are at the front
    while (getline(all_words_f, line)) {
        if (wtoi_map.find(line) != wtoi_map.end()) continue;
        itow_map.push_back(line);
        wtoi_map[line] = i;
        ++i;
    }

    words_size = i;

    ifstream guess_table_f(guess_table_file);
    vector<vector<string>> vals;

    while (getline(guess_table_f, line)) {
        string val;
        stringstream l(line);
        vals.push_back(vector<string>());
        while (getline(l, val, ',')) {
            vals.back().push_back(val);
        };
    }

    guess_table.resize(vals.size() - 1, vector<int>(vals[0].size() - 1));

    for (size_t i = 1; i < vals.size(); ++i) {
        const vector<string> &row = vals[i];
        int cur = wtoi_map[row[0]];
        for (size_t j = 1; j < row.size(); ++j) {
            int col_word = wtoi_map[vals[0][j]];
            guess_table[cur][col_word] = guess_to_int(row[j]);
        }
    }
}

int WordleData::size() const {
    return words_size;
}

int WordleData::size_guess() const {
    return guess_words_size;
}

int WordleData::get_guess(int guess, int actual) const {
    return guess_table[guess][actual];
}

const vector<vector<int>> &WordleData::get_guess_table() const {
    return guess_table;
}

std::string WordleData::itow(int i) const {
    return itow_map[i];
}

int WordleData::wtoi(const string &word) const {
    return get<1>(*wtoi_map.find(word));
}

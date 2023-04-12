#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <array>

using namespace std;

vector<string> get_lines(const string &filename) {
    ifstream f(filename);
    string line;

    vector<string> ret;

    while (getline(f, line)) {
        ret.push_back(line);
    }

    return ret;
}

template <typename T, std::size_t N>
std::ostream &operator<<(std::ostream &os, const std::array<T, N> &container) {
    os << '[';
    for (int i = 0; i < N; ++i) {
        if (i != 0)
            os << ", ";
        os << container[i];
    }
    os << ']';
    return os;
}

array<int, 5> guess_word(const string &guess, const string &answer) {
    array<int, 5> ret = {0};
    array<int, 26> d = {0};

    for (char c : answer) {
        ++d[c - 'a'];
    }

    for (size_t i = 0; i < answer.size(); ++i) {
        char c = answer[i];
        if (guess[i] != c) continue;
        
        ret[i] = 2;
        --d[c - 'a'];
    }

    for (size_t i = 0; i < guess.size(); ++i) {
        char c = guess[i];

        if (ret[i] == 2) 
            continue;
        
        ret[i] = 0 <= --d[c - 'a'];
    }

    return ret;
}

int main() {
    vector<string> words_possible = get_lines("guess_words");
    vector<string> all_words=  get_lines("all_words");

    ofstream f("guess_table");
    f << "     ";
    for (const string &w : words_possible) {
        f << "," << w;
    }
    f << "\n";
    int cnt = 0;
    for (const string &g : all_words) {
        if (cnt % 100 == 0) {
            cout << cnt << " " << all_words.size() << "\n";
        }
        f << g;
        for (const string &w : words_possible) {
            f << ",";
            array<int, 5> res = guess_word(g, w);
            for (int i : res)
                f << i; 
        }
        f << "\n";
        ++cnt;
    }
}
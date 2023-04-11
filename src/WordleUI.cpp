#include "WordleUI.h"
#include <algorithm>
#include <string>
using namespace std;

const size_t MIN_BOARD_HEIGHT = 6;

const size_t PAD_X = 2;
const size_t PAD_Y = 1;

extern int guess_to_int(const string &guess);
extern string guess_to_string(int guess);

enum FOREGROUND {
    FG_BLACK = 30,
    FG_RED = 31,
    FG_GREEN = 32,
    FG_YELLOW = 33,
    FG_BLUE = 34,
    FG_FG_MAGNETA = 35,
    FG_CYAN = 36,
    FG_WHITE = 37
};

enum BACKGROUND {
    BG_BLACK = 40,
    BG_RED = 41,
    BG_GREEN = 42,
    BG_YELLOW = 43,
    BG_BLUE = 44,
    BG_MAGNETA = 45,
    BG_CYAN = 46,
    BG_WHITE = 47
};

WordleUI::WordleUI(int boards, int cols, const WordleData *data) :
    game(boards, data),
    results_log(boards, std::vector<std::string>()),
    solved_turn(boards, -1) {
    this->data = data;
    this->boards = boards;
    this->cols = cols;
}

string colorize(string inp, FOREGROUND fg, bool bold = false) {
    string ret;
    ret += "\033[";
    ret += to_string((int)fg);
    if (bold)
        ret += ";1";
    ret += "m" + inp + "\033[0m";
    return ret;
}

string colorize(string inp, BACKGROUND bg, bool bold = false) {
    string ret;
    ret += "\033[";
    ret += to_string((int)bg);
    if (bold)
        ret += ";1";
    ret += "m" + inp + "\033[0m";
    return ret;
}

string colorize(string inp, FOREGROUND fg, BACKGROUND bg, bool bold = false) {
    string ret;
    ret += "\033[";
    ret += to_string(fg);
    ret += ";" + to_string(bg);
    if (bold)
        ret += ";1";
    ret += "m" + inp + "\033[0m";
    return ret;
}

void WordleUI::clear_board() {
    if (cleared)
        return;

    for (int i = 0; i < last_height + 1; ++i) {
        cout << "\x1b[A";
        cout << "\r";
    }

    cleared = true;

    last_height = 0;
}

void WordleUI::print_board(size_t highlighted) {
    size_t rows = (boards + cols - 1) / cols;

    size_t board_height = max(MIN_BOARD_HEIGHT, guesses.size() + 1);
    size_t lines = (board_height + 2 * PAD_Y) * rows;

    size_t width = (5 + 2 * PAD_X) * cols;
    width = max((size_t)100, width);

    vector<vector<string>> out(lines, vector<string>(width, " "));

    for (size_t i = 0; i < rows; ++i) {
        for (size_t j = 0; j < cols && i * cols + j < boards; ++j) {
            size_t board = i * cols + j;

            size_t corner_x = PAD_X + (5 + 2 * PAD_X) * j;
            size_t corner_y = PAD_Y + (board_height + 2 * PAD_Y) * i;

            for (size_t k = 0; k < guesses.size(); ++k) {
                if (k > (size_t)solved_turn[board])
                    break;

                const string &guess = guesses[k];
                auto &log = results_log[board];
                for (size_t l = 0; l < guess.size(); ++l) {
                    if (board == highlighted && k == guesses.size() - 1) {
                        out[corner_y + k][corner_x + l] =
                            colorize({ guess[l] }, FG_BLACK, BG_WHITE, true);
                    }
                    else if (k == (size_t) solved_turn[board]) {
                        out[corner_y + k][corner_x + l] =
                            colorize({ guess[l] }, BACKGROUND::BG_GREEN, true);
                    } else if (log.size() > k && log[k][l] - '0') {
                        out[corner_y + k][corner_x + l] = colorize({ guess[l] },
                            log[k][l] == '1' ? BACKGROUND::BG_YELLOW
                                             : BACKGROUND::BG_GREEN,
                            true);
                    } else {
                        out[corner_y + k][corner_x + l] = guess[l];
                    }
                }
            }
        }
    }

    for (const auto &v : out) {
        for (const auto &s : v) {
            cout << s;
        }
        cout << "\n";
    }

    last_height += lines;
    cleared = false;
}

void print_spaces(int length) {
    for (int i = 0; i < length; ++i)
        cout << " ";
}

void clear_input_line(int length) {
    cout << "\x1b[A";
    cout << "\r";
    print_spaces(length);
    cout << "\n";
}

bool valid_res(string inp) {
    if (inp.size() != 5)
        return false;

    for (size_t i = 0; i < inp.size(); ++i) {
        if (inp[i] < '0' || inp[i] > '2')
            return false;
    }
    return true;
}

void WordleUI::run() {
    size_t turn = 0;
    while (true) {
        if (game.is_invalid()) {
            cout << "Game is invalid. Exiting.\n";
            return;
        }

        string word = data->itow(game.get_best_word());

        vector<bool> solved = game.get_solved();

        for (size_t i = 0; i < solved.size(); ++i) {
            bool b = solved[i];
            if (!b || solved_turn[i] != -1)
                continue;

            solved_turn[i] = turn;
        }

        guesses.push_back(word);

        vector<int> results(boards);

        vector<bool> entered = solved;

        // get known guesses
        vector<int> definite_i = game.get_definite_results(data->wtoi(word));
        // vector<string> definite(definite_i.size(), "");
        for (size_t i = 0; i < definite_i.size(); ++i) {
            if (definite_i[i] == -1)
                continue;

            results[i] = definite_i[i];
            results_log[i].push_back(guess_to_string(definite_i[i]));
            entered[i] = true;
        }

        size_t i = 0;

        while (true) {
            i %= boards;

            for (size_t j = 0; j < boards && definite_i[i] != -1;
                 i = (i + 1) % boards, ++j) { };

            clear_board();
            cout << "\nGUESS: " << word
                 << " (Entropy: " << game.compute_entropy(data->wtoi(word))
                 << " bits, SD: " << game.compute_sd(data->wtoi(word))
                 << ")";
            print_spaces(20);
            cout << "\n";
            last_height += 2;
            print_board(i);
            string res;
            cout << "Inputting result for board " << i
                 << ". Use 'a' and 'd' to go left/right and enter nothing to "
                    "confirm.\n";
            ++last_height;
            cout << "INPUT: ";
            getline(cin, res);
            clear_input_line(res.length() + 7);

            if (res == "a") {
                --i;
                for (int j = 0; j < boards && i < boards && definite_i[i] != -1; i = (i - 1) % boards, ++j)
                    ;
                continue;
            }

            if (res == "d") {
                ++i;
                for (int j = 0; j < boards && i < boards && definite_i[i] != -1; i = (i + 1) % boards, ++j)
                    ;
                continue;
            }

            if (res == "") {
                if (any_of(entered.begin(), entered.end(),
                        [](bool b) { return !b; })) {
                    continue;
                }
                i = 0;
                break;
            }

            if (!valid_res(res)) {
                continue;
            }

            results[i] = guess_to_int(res);

            auto &log = results_log[i];

            if (log.size() <= turn)
                log.push_back(res);
            else
                log.back() = res;

            entered[i] = true;
            ++i;
        }

        game.filter_words(word, results);

        for (size_t i = 0; i < results.size(); ++i) {
            if (results[i] == 242)
                solved_turn[i] = turn;
        }

        ++turn;

        if (!any_of(solved.begin(), solved.end(), [](bool b) { return !b; })) {
            break;
        }
    }

    print_board();
}
#include "WordleUI.h"
#include <string>
using namespace std;

const size_t MIN_BOARD_HEIGHT = 6;

const size_t COLS = 2;

const size_t PAD_X = 2;
const size_t PAD_Y = 1;

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

string colorize(string inp, FOREGROUND fg, bool bold = false) {
    string ret;
    ret += "\033[";
    ret += to_string((int)fg);
    if (bold) ret += ";1";
    ret += "m" + inp + "\033[0m";
    return ret;
}

string colorize(string inp, BACKGROUND bg, bool bold = false) {
    string ret;
    ret += "\033[";
    ret += to_string((int)bg);
    if (bold) ret += ";1";
    ret += "m" + inp + "\033[0m";
    return ret;
}

string colorize(string inp, FOREGROUND fg, BACKGROUND bg, bool bold = false) {
    string ret;
    ret += "\033[";
    ret += to_string(fg);
    ret += ";" + to_string(bg);
    if (bold) ret += ";1";
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
    clear_board();

    size_t ROWS = (boards + COLS - 1) / COLS;

    size_t BOARD_HEIGHT = max(MIN_BOARD_HEIGHT, guesses.size() + 1);
    size_t LINES = (BOARD_HEIGHT + 2 * PAD_Y) * ROWS;

    size_t WIDTH = (5 + 2 * PAD_X) * COLS;
    WIDTH = max((size_t) 80, WIDTH);

    vector<vector<string>> out(LINES, vector<string>(WIDTH, " "));

    for (size_t i = 0; i < ROWS; ++i) {
        for (size_t j = 0; j < COLS && i * ROWS + j < boards; ++j) {
            size_t board = i * ROWS + j;

            size_t corner_x = PAD_X + (5 + 2 * PAD_X) * j;
            size_t corner_y = PAD_Y + (BOARD_HEIGHT + 2 * PAD_Y) * i;

            for (size_t k = 0; k < guesses.size(); ++k) {
                const string &guess = guesses[k];
                for (size_t l = 0; l < guess.size(); ++l) {
                    if (board == highlighted) {
                        out[corner_y + k][corner_x + l] = colorize({guess[l]}, FG_BLACK, BG_WHITE, true);
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

    last_height += LINES;
    cleared = false;
}

void print_spaces(int length) {
    for (int i = 0; i < length; ++i) cout << " ";
}


void clear_input_line(int length) {
    cout << "\x1b[A";
    cout << "\r";
    print_spaces(length);
    cout << "\n";
}

void WordleUI::start_game() {
    while (true) {
        string word = game.get_best_word();
        guesses.push_back(word);

        for (size_t i = 0; i < boards; ++i) {
            print_board(i);
            string res;
            cout << "Please input result for board " << i << ".\n";
            ++last_height;
            cout << "INPUT: ";
            cin >> res;
            clear_input_line(res.length() + 7);
        }
    }
}
#include "WordleData.h"
#include "WordleGame.h"
#include "WordleUI.h"
#include <iostream>
using namespace std;

extern int guess_to_int(const string &guess);

int main() {
    cout << "Importing data - please wait...\n\n";
    WordleData a("guess_words", "all_words", "guess_table");

    WordleUI ui(3, &a);
    ui.start_game();

}
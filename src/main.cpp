#include "WordleData.h"
#include "WordleGame.h"
#include "WordleUI.h"
#include <iostream>
using namespace std;

extern int guess_to_int(const string &guess);

int main() {
    cout << "Importing data - please wait...\n\n";
    WordleData a("guess_words", "all_words", "guess_table");

    bool play = true;
    while (play) {
        int boards;
        int cols;
        cout << "Enter the number of boards: ";
        cin >> boards;
        cout << "Enter the number of columns: ";
        cin >> cols;
        string dummy;
        getline(cin, dummy);
        cout << "\n\n";

        WordleUI ui(boards, cols, &a);
        ui.run();
        cout << "\n\n";
        char yn;
        cout << "Play again? (Y/n): ";
        cin >> yn;
        if (yn == 'n') break;
    }
}
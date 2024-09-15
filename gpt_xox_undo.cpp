#include <iostream>
#include <cstdlib> // for rand() and srand()
#include <ctime>   // for time()
#include <fstream> // for file operations
using namespace std;

#define ROWS 3
#define COLS 3

char table[ROWS][COLS];
int numberOfMoves = ROWS * COLS;
const string movesFile = "moves.txt";

bool control(int i, int j) {
    if (table[i][j] == ' ')
        return true;
    else
        return false;
}

void saveMove(int row, int col) {
    ofstream ofs(movesFile, ios::app);
    if (ofs.is_open()) {
        ofs << row << " " << col << endl;
        ofs.close();
    }
}

void deleteLastTwoMoves() {
    ifstream ifs(movesFile);
    ofstream ofs("temp.txt");
    string line;
    int moveCount = 0;

    // Count the number of lines/moves
    if (ifs.is_open()) {
        while (getline(ifs, line)) {
            moveCount++;
        }
        ifs.close();
    }

    if (moveCount > 1) { // Ensure there are at least two moves to delete
        ifs.open(movesFile);
        if (ifs.is_open() && ofs.is_open()) {
            int currentLine = 0;
            while (getline(ifs, line)) {
                if (currentLine < moveCount - 2) { // Skip the last two lines
                    ofs << line << endl;
                }
                currentLine++;
            }
            ifs.close();
            ofs.close();
        }

        remove(movesFile.c_str());
        rename("temp.txt", movesFile.c_str());
    }
}

void printTable() {
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            cout << table[i][j];
            if (j < COLS - 1) cout << " | ";
        }
        cout << endl;
        if (i < ROWS - 1) cout << "----------" << endl;
    }
}

void initializeTable() {
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            table[i][j] = ' ';
        }
    }
}

void userX() {
    char userRow;
    int userCol;
    cout << "Enter the row and col (A1-C3) or U to undo: " << endl;
    cin >> userRow;

    if (userRow == 'U' || userRow == 'u') {
        if (numberOfMoves < ROWS * COLS) {
            deleteLastTwoMoves();
            ifstream ifs(movesFile);
            initializeTable();
            numberOfMoves = ROWS * COLS;

            if (ifs.is_open()) {
                int row, col;
                while (ifs >> row >> col) {
                    table[row][col] = (numberOfMoves % 2 == 0) ? 'O' : 'X';
                    numberOfMoves--;
                }
                ifs.close();
            }

            cout << "Last two moves undone.\n";
            printTable();
            userX(); // Allow the user to make their move again
        } else {
            cout << "No moves to undo.\n";
            userX();
        }
        return;
    }

    cin >> userCol;

    if (control(userRow - 'A', userCol - 1)) {
        table[userRow - 'A'][userCol - 1] = 'X';
        saveMove(userRow - 'A', userCol - 1);
    } else {
        cout << "Invalid move. Try again.\n";
        userX();
    }
    numberOfMoves--;
}

void opponentO() {
    srand(static_cast<unsigned int>(time(nullptr)));

    int randRow, randCol;
    do {
        randRow = rand() % 3;
        randCol = rand() % 3;
    } while (!control(randRow, randCol));

    table[randRow][randCol] = 'O';
    saveMove(randRow, randCol);

    numberOfMoves--;
}

char winnerControl() {
    //horizontal
    for (int i = 0; i < 3; i++) {
        if (table[i][0] == table[i][1] && table[i][0] == table[i][2] && table[i][0] != ' ')
            return table[i][0];
    }

    //vertical
    for (int i = 0; i < 3; i++) {
        if (table[0][i] == table[1][i] && table[0][i] == table[2][i] && table[0][i] != ' ')
            return table[0][i];
    }

    //diagonals
    if (table[0][0] == table[1][1] && table[0][0] == table[2][2] && table[0][0] != ' ')
        return table[0][0];

    if (table[0][2] == table[1][1] && table[0][2] == table[2][0] && table[0][2] != ' ')
        return table[0][2];

    return ' ';
}

int main() {
    initializeTable();
    ofstream ofs(movesFile, ofstream::trunc); // Clear the file at the start
    ofs.close();

    do {
        userX();
        if (winnerControl() != ' ' || numberOfMoves == 0) break; // Stop if there's a winner or no moves left
        opponentO();
        printTable();
    } while (winnerControl() == ' ' && numberOfMoves != 0);

    if (winnerControl() == 'X')
        cout << "You are the winner!";
    else if (numberOfMoves == 0)
        cout << "Draw";
    else
        cout << "You lose.";

    return 0;
}
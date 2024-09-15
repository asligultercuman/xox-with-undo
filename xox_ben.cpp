#include <iostream>
#include <cstdlib> // for rand() and srand()
#include <ctime>   // for time()
#include <cctype>  // for toupper
using namespace std;

#define ROWS 3
#define COLS 3

char table[ROWS][COLS];
int numberOfMoves = ROWS*COLS;

bool control(int i, int j){
    if(table[i][j] == ' ')
        return true;
    else
        return false;
}

void userX(){
    char userRow;
    int userCol;
    cout << "Enter the row and col (A1-C3): " << endl;
    cin >> userRow >> userCol;
    userRow = toupper(userRow);

    if(control(userRow - 'A', userCol - 1))
        table[userRow - 'A'][userCol - 1] = 'X';
    else{
        cout << "Invalid move. Try again.\n";
        userX();
    }    
    numberOfMoves--;
}

void opponentO(){

    srand(static_cast<unsigned int>(time(nullptr)));

    int randRow, randCol;
    do {
        randRow = rand() % 3;
        randCol = rand() % 3;
    } while (!control(randRow, randCol));

    table[randRow][randCol] = 'O';

    numberOfMoves--;
}

char winnerControl(){
    //horizontal
    for(int i=0; i<3; i++){
        if(table[i][0] == table[i][1] && table[i][0] == table[i][2] && table[i][0] != ' ')
            return table[i][0];
    }

    //vertical
    for(int i=0; i<3; i++){
        if(table[0][i] == table[1][i] && table[0][i] == table[2][i] && table[0][i] != ' ')
            return table[0][i];
    }

    //diagonals
    if(table[0][0] == table[1][1] && table[0][0] == table[2][2] && table[0][0] != ' ')
        return table[0][0];

    if(table[0][2] == table[1][1] && table[0][2] == table[2][0] && table[0][2] != ' ')
        return table[0][2];

    return ' ';
}

void printTable(){
    for(int i=0; i<ROWS; i++){
        for(int j=0; j<COLS; j++){
            cout << table[i][j] << " | ";
        }
        cout << endl << "-----------" << endl;
    }
}

void initializeTable(){
    for(int i=0; i<ROWS; i++){
        for(int j=0; j<COLS; j++){
            table[i][j] = ' ';
        }
    }
}

int main(){

    initializeTable();
    do{
        userX();
        opponentO();
        printTable();
    }while( winnerControl() == ' ' && numberOfMoves != 0);
    
    if(winnerControl() == 'X')
        cout << "You are the winner!";
    else if(numberOfMoves == 0)
        cout << "Draw";
    else    
        cout << "You loose.";
        
    return 0;
}
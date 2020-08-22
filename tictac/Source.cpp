  

#include <fstream> 
#include <iostream> 
#include <string> 
#include <sstream>

using namespace std;

const int N = 3;
const int M = 8;
int currentCoord = 0;
char board[N][N] = { {'_', '_', '_'}, {'_', '_', '_'} , {'_', '_', '_'} };
int aiCounter[M] = { 0, 0, 0, 0, 0, 0, 0, 0 };
int humanCounter[M]{ 0, 0, 0, 0, 0, 0, 0, 0 };

void menu(char board[N][N], int aiCounter[M], int humanCounter[M]);
void runGame(char board[N][N], int aiCounter[], int humanCounter[]);
void humanMarkBoard(char board[N][N]);
void aiMarkBoard(char board[N][N], int aiCounter[], int humanCounter[]);
int  aiPickIdx(char board[N][N], int aiCounter[], int humanCounter[]);
void updateArrCounter(int coord, int aiCounter[], int humanCounter[]);
char checkForWin(int aiCounter[], int humanCounter[M]);
void resetArr(int ai[], int human[]);
void resetGame();
bool isBoardFilled();
void printDirections();
void printBoard(char board[N][N]);



int main(int argc, char* argv)
{
    menu(board, aiCounter, humanCounter);
    return 0;
};

void menu(char board[N][N], int aiCounter[M], int humanCounter[M]) {
    char switch_on = ' ';
    cout << "Welcome to Tic Tac Toe! " << endl;
    cout << "Press n to start new game. " << endl;
    cout << "Press c to continue game." << endl;
    cout << "Press d for directions. " << endl;
    cout << "Press e to end program. " << endl;
    cout << "Option: ";
    cin >> switch_on;

    switch (switch_on)
    {
    case 'n':
        resetGame();
        runGame(board, aiCounter, humanCounter);
    case 'c':
        runGame(board, aiCounter, humanCounter);
    case 'd':
        printDirections();
        menu(board, aiCounter, humanCounter);
    case 'e': exit(0);

    default: menu(board, aiCounter, humanCounter);
    }
}

void runGame(char board[N][N], int aiCounter[], int humanCounter[]) {
    printDirections();
    cout << "Let's play tic tac toe! " << endl;
    printBoard(board);
    char winner = ' ';
    while (true) {
        humanMarkBoard(board); 
        updateArrCounter(currentCoord, aiCounter, humanCounter);
        winner = checkForWin(aiCounter, humanCounter);
        if (winner != ' ') {
            cout << "Game Over!" << endl;
            cout << winner << " won!" << endl;
            resetGame();
            menu(board, aiCounter, humanCounter);
        }
        if (isBoardFilled()) {
            break;
        }
        aiMarkBoard(board, aiCounter, humanCounter);
        printBoard(board);
        updateArrCounter(currentCoord, aiCounter, humanCounter);
        winner = checkForWin(aiCounter, humanCounter);
        if (winner != ' ') {
            cout << "Game Over!" << endl;
            cout << winner << " won!" << endl;
            resetGame();
            menu(board, aiCounter, humanCounter);
        }
        if (isBoardFilled()) {
            break;
        }
    }
    printBoard(board);
    cout << "Game Over.\nIt's a tie." << endl;
    resetGame();
    menu(board, aiCounter, humanCounter);
}

void humanMarkBoard(char board[N][N]) {
    int input;
    string testinput; 
    int coord;
    int r = 0;
    int c = 0;
    bool needInput = true;
    string promptMsg = "Enter 0 to go back to menu \nOr enter a position from 1 to 9 to mark board.\n"; 
    cout << promptMsg; 

    while (needInput) {
        
        try {
            getline(cin, testinput);
            input = stoi(testinput); // throws exception if cannot convert to int
            if (input == 0) {
                menu(board, aiCounter, humanCounter);
            }
            else {
                input--; // make input 0-indexed to calc row & col 
                r = input / 3;
                c = input % 3;
                if (board[r][c] == '_' && input >= 0 && input < 9) {
                    board[r][c] = 'X';
                    currentCoord = r * N + c;
                    needInput = false;
                    break;
                }
                if (input < 0 || input > 9) {
                    cout << "Out of bound position. " << endl;
                    cout << promptMsg; 
                    continue; 
                }
                if ((input >= 0 || input < 9 )&& board[r][c] != ' ') {
                    cout << "Position occupied. " << endl;
                    cout << promptMsg; 
                }
            }
        }
        catch (invalid_argument & e) {
            if (testinput.empty()) {
                continue; 
            }
            else {
                cout << promptMsg;
                continue;
            }
        }
    }

}

void aiMarkBoard(char board[N][N], int aiCounter[], int humanCounter[]) {
    int coord = aiPickIdx(board, aiCounter, humanCounter);
    int r = coord / 3;
    int c = coord % 3;
    board[r][c] = 'O';
}
int aiPickIdx(char board[N][N], int aiCounter[], int humanCounter[]) {
    //check if 2 in a line, complete line and win 
    //check if human has 2 in a row  
    //if they do, loop through and find empty spot 
    //else, place piece close to other piece to make 3 in a line

    int winCoord = -1;  // the ai's winning move coordinate
    int offCoord = -1;  // offence coordinate to block human's 3 in a line
    int defCoord = -1;  // defence coordinate to make 3 in a line for ai win
    int randCoord = 0;  // rand coordinate, when no off or def moves possible (beginning of game)
    int calcCoord = -1; // save coord as single digit here

    for (int r = 0; r < N; r++) {
        for (int c = 0; c < N; c++) {
            if (board[r][c] == '_') {
                calcCoord = r * 3 + c;
                if (aiCounter[r] == 2 || aiCounter[c + N] == 2) { //row and col 
                    winCoord = calcCoord;
                    currentCoord = winCoord;
                    return winCoord;
                }
                if (r == c && aiCounter[M - 1] == 2) { //left diag
                    winCoord = calcCoord;
                    currentCoord = winCoord;
                    return winCoord;
                }
                if (aiCounter[M - 2] == 2 && calcCoord % 2 == 0 && calcCoord > 0 && calcCoord < M) { //right diag
                    winCoord = calcCoord;
                    currentCoord = winCoord;
                    return winCoord;
                }
                if (offCoord == -1) {
                    if (humanCounter[r] == 2) { //row
                        offCoord = calcCoord;

                    }
                    if (humanCounter[c + N] == 2) { //col 
                        offCoord = calcCoord;

                    }
                    if (r == c && humanCounter[M - 1] == 2) { //left diag
                        offCoord = calcCoord;

                    }
                    if (humanCounter[M - 2] == 2 && calcCoord % 2 == 0 && calcCoord > 0 && calcCoord < 8) { //right diag
                        offCoord = calcCoord;
                    }
                }
                if (defCoord == -1) {
                    if (aiCounter[r] == 1 && (aiCounter[r] + humanCounter[r]) < 2) { //row
                        defCoord = calcCoord;
                    }
                    if (aiCounter[c + N] == 1 && (aiCounter[r + N] + humanCounter[r + N]) < 2) { //col 
                        defCoord = calcCoord;
                    }
                    if (r == c && (aiCounter[M - 1] + humanCounter[M - 1]) < 2) { //left diag

                        defCoord = calcCoord;
                    }
                    if ((aiCounter[M - 2] + humanCounter[M - 2]) < 2 && calcCoord % 2 == 0 && calcCoord > 0 && calcCoord < 8) { //right diag

                        defCoord = calcCoord;
                    }
                }
                if (randCoord == 0) {
                    if (calcCoord % 2 == 0 && (humanCounter[r] > 0 || humanCounter[c + N])) { // choose corner or center position 
                        randCoord = calcCoord;

                    }
                }

            }
        }
    }


    if (offCoord != -1) {
        currentCoord = offCoord;
        return offCoord;
    }
    if (defCoord != -1) {
        currentCoord = defCoord;
        return defCoord;
    }
    currentCoord = randCoord;
    return randCoord;
}

void updateArrCounter(int coord, int aiCounter[], int humanCounter[]) {
    //AI IS O
    int r = coord / 3;
    int c = coord % 3;
    if (board[r][c] == 'O') {
        if (r == c) {
            //left diagonal
            aiCounter[M - 1]++; //last index   
        }
        if (coord % 2 == 0 && coord > 0 && coord < 8) {
            //right diag
            aiCounter[M - 2]++; //second last index   
        }
        aiCounter[r]++;
        aiCounter[c + N]++; // Col

    }
    if (board[r][c] == 'X') {
        if (r == c) {
            //left diagonal
            humanCounter[M - 1]++; //last index
        }
        if (coord % 2 == 0 && coord > 0 && coord < 8) {
            ///right diag
            humanCounter[M - 2]++; //second last index   
        }
        humanCounter[r]++;
        humanCounter[c + N]++; // COL

    }

}

char checkForWin(int aiCounter[], int humanCounter[M]) {

    for (int i = 0; i < M; i++) {
        if (aiCounter[i] == 3) {
            return 'O';

        }
        if (humanCounter[i] == 3) {
            return 'X';
        }

    }

    return ' ';

}

void resetArr(int ai[], int human[]) {
    for (int i = 0; i < M; i++) {
        ai[i] = 0;
        human[i] = 0;
    }
}

void resetGame() {
    resetArr(aiCounter, humanCounter);
    for (int r = 0; r < N; r++) {
        for (int c = 0; c < N; c++) {
            board[r][c] = '_';
        }
    }
    currentCoord = 0;
}

bool isBoardFilled() {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (board[i][j] == '_') {
                return false;
            }
        }
    }
    return true;
}

void printDirections() {
    ifstream dirFile;
    string dirOutput = "";
    dirFile.open("directions.txt");

    while (getline(dirFile, dirOutput)) {
        //returns true after each line of .txt
        cout << dirOutput << "\n";

    }

    dirFile.close();

}

void  printBoard(char board[N][N]) {
    for (int r = 0; r < N; r++) {
        for (int c = 0; c < N; c++) {
            cout << board[r][c];
            cout << " ";
        }
        cout << endl;
    }
}










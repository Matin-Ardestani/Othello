// ==============================================Importing Libraries==============================
#include <iostream>
#include <string>
#include <conio.h> // inputs
#include <cstdlib> // random
#include <ctime> // time

using namespace std;


//==============================================Structures & Global variables=======================================
const int board_size = 8;
struct Game{
    int board[board_size][board_size];
    int turn;
    string player1name;
    string player2name;
    bool isSinglePlayerMode;
};

struct CellState{
    const int empty = 0, black = 1, white = 2;
};

Game game;
CellState cell_state;

//=============================================Function prototypes===============================
void ShowBoard(int curserI, int curserJ);
void ShowMenu();
void GameMode(); // single player mode or two player mode
void PlayGame();


//=============================================Main Function=====================================
int main(){

    system("chcp 65001"); // for showing the unicode characters in terminal


    // preparing board for the start of the game
    for(int i = 0; i < board_size; i++){
        for(int j = 0; j < board_size; j++){
            game.board[i][j] = cell_state.empty;
        }
    }
    game.board[3][3] = cell_state.white;
    game.board[4][4] = cell_state.white;
    game.board[3][4] = cell_state.black;
    game.board[4][3] = cell_state.black;
    

    ShowMenu();


    return 0;
}



//============================================Functions=========================================

// showing the board and the state of it
void PlayGame(){
    bool running = true;
    int curserI = 0, curserJ = 0;
    int counter = 0;

    while(running){
        ShowBoard(curserI, curserJ);
        counter = 0;
        char pressed_key = getch();
        switch(pressed_key)
        {
        case 'w': case 'W': // UP
            if(curserI - 1 >= 0){
                int temp = curserI;
                while(game.board[curserI - 1][curserJ] != cell_state.empty){
                    curserI--;
                }
                if(curserI < 0){
                    curserI = temp;
                    break;
                }
                curserI--;
            }
            break;
        case 's': case 'S': // DOWN
            if(curserI + 1 < board_size){
                int temp = curserI;
                while(game.board[curserI + 1][curserJ] != cell_state.empty){
                    curserI++;
                }
                if(curserI < 0){
                    curserI = temp;
                    break;
                }
                curserI++;
            }
            break;
        case 'd': case 'D': // RIGHT
            if(curserJ + 1 < board_size){
                int temp = curserJ;
                while(game.board[curserI][curserJ + 1] != cell_state.empty){
                    curserJ++;
                }
                if(curserJ < 0){
                    curserJ = temp;
                    break;
                }
                curserJ++;
            }
            break;
        case 'a': case 'A': // LEFT
            if(curserJ - 1 >= 0){
                int temp = curserJ;
                while(game.board[curserI][curserJ - 1] != cell_state.empty){
                    curserJ--;
                }
                if(curserJ < 0){
                    curserJ = temp;
                    break;
                }
                curserJ--;
            }
            break;
        case 'm': // temparary
            running = false;
            break;
        }

    }
    ShowMenu();
}

void ShowBoard(int curserI = -1, int curserJ = -1){ // curser is the selected cell by keyboard
    system("cls");
    for(int i = 0; i < board_size; i++){
        for(int j = 0; j < board_size; j++){
            if(curserI == i && curserJ == j){
                cout << u8"\u25A0";
                continue;
            }
            if(game.board[i][j] == cell_state.empty) cout << u8"\u25A1";
            else if(game.board[i][j] == cell_state.black) cout << u8"\u25CF";  
            else if(game.board[i][j] == cell_state.white) cout << u8"\u25CB";
        }
        cout << endl;
    }

    // showing turn


}

// menu
void ShowMenu(){

    system("cls");

    cout << "Main Menu:\n" 
         << "1. New Game\n"
         << "2. Help\n"
         << "3. Game History\n"
         << "4. Exit\n";

    char choice = getch();
    char temp;
    
    switch(choice)
    {
    case '1': // new game
        GameMode();
        break;
    case '2': // help
        system("cls");
        cout << "Othello is a two-player strategy board game played on an 8×8 grid using black and white discs. Players take turns placing one of their discs on an empty square, with black always going first. A move is only legal if the placed disc brackets one or more of the opponent’s discs in a straight line (horizontal, vertical, or diagonal) between the new disc and another disc of the same color; all such bracketed discs are then flipped to the current player’s color. If a player has no valid moves, their turn is skipped. The game continues until neither player can make a move, usually when the board is full. The winner is the player with the most discs of their color on the board at the end of the game.";
        cout << "\n\nPress any key to go to menu.";
        
        temp = getch();
        ShowMenu();
        break;
    case '3':
        // History
        break;
    case '4': // exit
        return; // closing terminal
        break;
    
    default:
        cout << "Invalid Input!\tTry again.";
        ShowMenu();
        break;
    }

}

// choosing which game mode we are gonna have
void GameMode(){
    system("cls");

    cout << "Choose Game Mode:\n" 
         << "1. Single Player Mode\n"
         << "2. Two Player Mode\n";

    char choice = getch();

    switch(choice)
    {
    case '1':
        game.isSinglePlayerMode = true;
        PlayGame();
        break;
    case '2':
        game.isSinglePlayerMode = false;
        break;
    default:
        cout << "Invalid Input!\tTry again.";
        GameMode();
        break;
    }


    ShowBoard(0, 0);
}
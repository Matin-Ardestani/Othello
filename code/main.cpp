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
void ShowBoard();
void ShowMenu();
void GameMode(); // single player mode or two player mode


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
void ShowBoard(){
    for(int i = 0; i < board_size; i++){
        for(int j = 0; j < board_size; j++){
            if(game.board[i][j] == cell_state.empty) cout << u8"\u25A1";
            else if(game.board[i][j] == cell_state.black) cout << u8"\u25CF";  
            else if(game.board[i][j] == cell_state.white) cout << u8"\u25CB";  
        }
        cout << endl;
    }
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
    ShowBoard();
}
// ==============================================Importing Libraries==============================
#include <iostream>
#include <string>
#include <iomanip>
#include <conio.h> // inputs
#include <cstdlib> // random
#include <ctime> // time
#include <fstream>

using namespace std;


//==============================================Structures & Global variables=======================================
const int board_size = 8;

struct ColorChanger{
    int startI, startJ;
    int endI, endJ;
    int directionX, directionY;
};

struct Game{
    int board[board_size][board_size];
    int turn;
    string player1name;
    string player2name;
    bool isSinglePlayerMode;
    int black_count;
    int white_count;
    ColorChanger color_changer;
};

struct CellState{
    const int empty = 0, black = 1, white = 2;
};

Game game;
CellState cell_state;
int curserI, curserJ;

fstream history("C:\\Users\\LOQ\\Desktop\\Othello\\code\\history.txt", ios::in | ios::out | ios::app);
/*
    history file format:
    player1name,player1discs,player2name,player2discs,winner,gamedate
*/

// crowling through a cell's nieghbors
int IndX[8] = {-1, 0, 1, -1, 1, -1, 0, 1};
int IndY[8] = {-1, -1, -1, 0, 0, 1, 1, 1};

//=============================================Function prototypes===============================
void ShowBoard(int i, int j);
void ShowMenu();
void GameMode(); // single player mode or two player mode
void PlayGame();
void PlaceCell(int i, int j);
void SwitchTurn();
bool CheckValidTurn(int turn);
bool CheckValidMove(int i, int j, bool change_color);
void ChangeColor();
void ShowHistory();


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

// THE GAME
void PlayGame(){

    // recieving names and declaring turns
    if(!game.isSinglePlayerMode){
        cout << "Enter Player 1 Name: ";
        cin >> game.player1name;
        cout << "Enter Player 2 Name: ";
        cin >> game.player2name;
        game.turn = 1;
    }
    else{
        cout << "Enter Player 1 Name: ";
        cin >> game.player1name;
        game.player2name = "Bot";
        game.turn = 1;
    }


    // moving through the board
    bool running = true;
    int opposite_turn;
    
    while(running){
        ShowBoard(curserI, curserJ);
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
                while(game.board[curserI][curserJ - 1] == cell_state.black || game.board[curserI][curserJ - 1] == cell_state.white){
                    curserJ--;
                }
                if(curserJ < 0){
                    curserJ = temp;
                    break;
                }
                curserJ--;
            }
            break;

        case '\r': // selecting the cell
            if(CheckValidMove(curserI, curserJ, true)){
                PlaceCell(curserI, curserJ);
                SwitchTurn();
            }
            else{
                cout << "Cant Place there!";
            }
            break;
        case ' ': // closing game
            running = false;
            break;
        }

        opposite_turn = (game.turn == 1 ? 2 : 1);
        if(!CheckValidTurn(game.turn) && CheckValidTurn(opposite_turn)){
            SwitchTurn();
        }
        // end of game
        else if(!CheckValidTurn(game.turn) && !CheckValidTurn(opposite_turn)){
            ShowBoard(-1, -1);
            time_t now = time(0);
            if(game.black_count > game.white_count){
                cout << endl << game.player1name << " WON!";
                // saving game info
                history << endl
                        << game.player1name << "," << game.black_count << ","
                        << game.player2name << "," << game.white_count << ","
                        << game.player1name << ","
                        << ctime(&now);
            }
            else if(game.black_count < game.white_count){
                cout << endl << game.player2name << " WON!";
                // saving game info
                history << endl
                        << game.player1name << "," << game.black_count << ","
                        << game.player2name << "," << game.white_count << ","
                        << game.player2name << ","
                        << ctime(&now);
            }
            else{
                cout << endl << "TIE GAME!";
                // saving game info
                history << endl 
                        << game.player1name << "," << game.black_count << ","
                        << game.player2name << "," << game.white_count << ","
                        << "tie" << ","
                        << ctime(&now);
            }
            
            char temp = getch(); // keeping the page alive to see the result
            running = false;
        }

    }
    ShowMenu();
}

// drawing the game board
void ShowBoard(int i = -1, int j = -1){ // curser is the selected cell by keyboard
    system("cls");
    game.black_count = 0;
    game.white_count = 0;
    for(int i = 0; i < board_size; i++){
        for(int j = 0; j < board_size; j++){
            if(curserI == i && curserJ == j){
                cout << u8"\u25A0";
                continue;
            }
            if(game.board[i][j] == cell_state.empty) cout << u8"\u25A1";
            else if(game.board[i][j] == cell_state.black){
                cout << u8"\u25CF";  
                game.black_count++;
            }
            else if(game.board[i][j] == cell_state.white){
                cout << u8"\u25CB";
                game.white_count++;
            }
        }
        cout << endl;
    }

    // showing whose turn it is
    if(game.turn == 1) cout << "Turn: " << game.player1name << u8" \u25CF";
    else if(game.turn == 2 && !game.isSinglePlayerMode) cout << "Turn: " << game.player2name << u8" \u25CB";
    else cout << "Turn: Bot " << u8" \u25CB";

    // showing each players discs
    cout << "\n" << game.player1name << "\'s discs:" << game.black_count << "\t" << game.player2name << "\'s discs:" << game.white_count;


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
    case '3': // History
        ShowHistory();
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
    case '1': // Single player mode
        game.isSinglePlayerMode = true;
        PlayGame();
        break;
    case '2': // Two player mode
        game.isSinglePlayerMode = false;
        PlayGame();
        break;
    default:
        cout << "Invalid Input!\tTry again.";
        GameMode();
        break;
    }


    ShowBoard(0, 0);
}


// changing the selected cell
void PlaceCell(int i, int j){
    if(game.turn == 1){
        game.board[i][j] = cell_state.black;
    }
    else{
        game.board[i][j] = cell_state.white;
    }

    // finding the first cell that is empty to place the curser
    for(int i = board_size - 1; i >= 0; i--){
        for(int j = board_size - 1; j >= 0; j--){
            if(game.board[i][j] == cell_state.empty){
                curserI = i;
                curserJ = j;
            }
        }
    }


}


// switchin turn in game
void SwitchTurn(){
    if(game.turn == 1) game.turn = 2;
    else game.turn = 1;
}

// checking if the cell is available for the player
bool CheckValidMove(int curserI, int curserJ, bool change_color){
    bool validMove = false;
    if(game.board[curserI][curserJ] != cell_state.empty) {
        return false;
    }

    int opponent;
    int myColor;
    if(game.turn == 1){
        myColor = cell_state.black;
        opponent = cell_state.white;
    }
    else{
        myColor = cell_state.white;
        opponent = cell_state.black;
    }

    for(int k = 0; k < 8; k++){
        int dx = IndX[k];
        int dy = IndY[k];
        int x = curserI + dx;
        int y = curserJ + dy;

        if(x >= 0 && x < board_size && y >= 0 && y < board_size && game.board[x][y] == opponent){
            
            // going on in the same direction
            while(true){
                x += dx;
                y += dy;

                if(x < 0 || x >= board_size || y < 0 || y >= board_size) {
                    break;
                }
                if(game.board[x][y] == cell_state.empty){
                    break;
                }
                if(game.board[x][y] == myColor){

                    game.color_changer.startI = curserI;
                    game.color_changer.startJ = curserJ;
                    game.color_changer.endI = x;
                    game.color_changer.endJ = y;
                    game.color_changer.directionX = dx;
                    game.color_changer.directionY = dy;
                    
                    if(change_color) ChangeColor(); // if we are checking the cell for a player move then we change the colors int the same process

                    validMove = true;
                }
            }
        }
    }
    return validMove;

}

// checking if the player can play a move
bool CheckValidTurn(int turn){
    for(int i = 0; i < board_size; i++){
        for(int j = 0; j < board_size; j++){
            if(game.board[i][j] == cell_state.empty && CheckValidMove(i, j, false))
                return true;
        }
    }
    return false;
}

// changin cells after a valid move
void ChangeColor(){
    int opposite_color = (game.turn == 1 ? cell_state.black : cell_state.white);
    int x = game.color_changer.startI;
    int y = game.color_changer.startJ;
    while(true){
        x += game.color_changer.directionX;
        y += game.color_changer.directionY;

        if(game.board[x][y] != opposite_color){
            game.board[x][y] = opposite_color;
        }
        else{
            break;
        }

        
    }
}

// showing history (crawling through history.txt file which is considered a database for this game)
void ShowHistory(){
    system("cls");

    cout << left
         << setw(10) << "P1"
         << setw(8)  << "D1"
         << setw(10) << "P2"
         << setw(8)  << "D2"
         << setw(10) << "Winner"
         << setw(10) << "Date"
         << endl;

    cout << string(56, '-') << endl;


    string line;
    string player1, player2, winner, date;
    string player1discs, player2discs;
    int splitter[5];
    while(getline(history, line)){
        for(int i = 0; i < 5; i++){
            if(i == 0)
                splitter[i] = line.find(",");
            else
                splitter[i] = line.find(",", splitter[i - 1] + 1);
        }

        player1 = line.substr(0, splitter[0]);
        player1discs = line.substr(splitter[0] + 1, splitter[1] - splitter[0] - 1);
        player2 = line.substr(splitter[1] + 1, splitter[2] - splitter[1] - 1);
        player2discs = line.substr(splitter[2] + 1, splitter[3] - splitter[2] - 1);
        winner = line.substr(splitter[3] + 1, splitter[4] - splitter[3] - 1);
        date = line.substr(splitter[4] + 1);

        cout << left
            << setw(10) << player1
            << setw(8)  << player1discs
            << setw(10) << player2
            << setw(8)  << player2discs
            << setw(10) << winner
            << setw(10) << date
            << endl;

    }


    cout << "\n\nPress any key to go to menu.";
    char temp = getch();
    ShowMenu();

}


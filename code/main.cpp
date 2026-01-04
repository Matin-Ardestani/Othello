// ==============================================Importing Libraries==============================
#include <iostream>
#include <string>
#include <iomanip> // cout
#include <conio.h> // inputs
#include <cstdlib> // random
#include <ctime> // time
#include <fstream> // file
#include <windows.h> // delay

using namespace std;


//==============================================Structures & Global variables=======================================
const int board_size = 8;

// --- ANSI Color Codes ---
const string RESET = "\033[0m";
const string BLACK_TXT = "\033[30m";  // black text
const string RED_TXT = "\033[31m";    // red text
const string GREEN_TXT = "\033[32m";  // green text
const string YELLOW_TXT = "\033[33m"; // yellow text
const string WHITE_TXT = "\033[97m";  // white text

const string BG_GREEN = "\033[42m";   // green background
const string BG_YELLOW = "\033[43m";  // yellow background
const string BG_DEFAULT = "\033[49m"; // default background

const string CYAN_TXT = "\033[36m";
const string MAGENTA_TXT = "\033[35m";
const string BOLD = "\033[1m";
const string MARGIN = "      ";

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
void BotMove();


//=============================================Main Function=====================================
int main(){

    system("chcp 65001"); // for showing the unicode characters in terminal
    srand(static_cast<unsigned int>(time(nullptr)));


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
    system("cls");
    cout << "\n\n";
    cout << MARGIN << BOLD << CYAN_TXT << u8"╔══════════════════════════════════╗" << RESET << endl;
    cout << MARGIN << BOLD << CYAN_TXT << u8"║            GAME SETUP            ║" << RESET << endl;
    cout << MARGIN << BOLD << CYAN_TXT << u8"╚══════════════════════════════════╝" << RESET << endl;
    cout << endl;
    if(!game.isSinglePlayerMode){ // two player mode
        cout << MARGIN << YELLOW_TXT << "Player 1 Name (Black): " << CYAN_TXT;
        cin >> game.player1name;
        cout << RESET << "\n";

        cout << MARGIN << YELLOW_TXT << "Player 2 Name (White): " << CYAN_TXT;
        cin >> game.player2name;
        cout << RESET;
    }
    else{
        cout << MARGIN << YELLOW_TXT << "Enter Your Name: " << CYAN_TXT;
        cin >> game.player1name;
        cout << RESET;
        game.player2name = "Bot";
    }
    game.turn = 1;


    // moving through the board
    bool running = true;
    int opposite_turn;
    
    while(running){
        ShowBoard(curserI, curserJ);

        if(game.isSinglePlayerMode && game.turn == 2){
            Sleep(500); // a delay for palyer to see bot's move
            BotMove();
            SwitchTurn();
        }
        else {
            char pressed_key = getch();
            switch(pressed_key)
            {
            case 'w': case 'W': 
                if(curserI > 0) curserI--;
                break;
            case 's': case 'S': 
                if(curserI < board_size - 1) curserI++;
                break;
            case 'd': case 'D': 
                if(curserJ < board_size - 1) curserJ++;
                break;
            case 'a': case 'A': 
                if(curserJ > 0) curserJ--;
                break;

            case '\r': // Enter
                if(CheckValidMove(curserI, curserJ, true)){
                    PlaceCell(curserI, curserJ);
                    SwitchTurn();
                }
                else{
                    cout << "\a";
                }
                break;
            case ' ': // emergency exit
                running = false;
                break;
            }
        }

        // end of game or switch turn because of no valid move
        opposite_turn = (game.turn == 1 ? 2 : 1);
        bool currentHasMove = CheckValidTurn(game.turn);
        bool opponentHasMove = CheckValidTurn(opposite_turn);

        if(!currentHasMove && opponentHasMove){
            cout << "\nNo Valid Moves for " << (game.turn == 1 ? game.player1name : game.player2name) << "! Passing turn...";
            getch();
            SwitchTurn();
        }
        else if(!currentHasMove && !opponentHasMove){ // end of game
            ShowBoard(-1, -1);
            time_t now = time(0);
            string winnerName = "Tie";

            if(game.black_count > game.white_count) winnerName = game.player1name;
            else if(game.black_count < game.white_count) winnerName = game.player2name;

            cout << "\n========================\n";
            if (winnerName == "Tie") cout << "       TIE GAME!";
            else cout << "   WINNER: " << winnerName;
            cout << "\n========================\n";

            ofstream history("history.txt", ios::app);
            if(history.is_open()){

                // AI generated
                char* dt = ctime(&now); // converting datetime to string
                // deleting \n from the end of the time string
                string dateStr = string(dt); 
                if (!dateStr.empty() && dateStr[dateStr.length()-1] == '\n') {
                    dateStr.erase(dateStr.length()-1);
                }
                //...

                history << game.player1name << "," << game.black_count << ","
                         << game.player2name << "," << game.white_count << ","
                         << winnerName << ","
                         << dateStr << endl;
                
                history.close();
                cout << "Game saved to history." << endl;
            } else {
                cout << "Error saving history!" << endl;
            }
            
            cout << "Press any key to exit...";
            getch(); 
            running = false;

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


        }
    }
    ShowMenu();
}

// drawing the game board
void ShowBoard(int i = -1, int j = -1){ // curser is the selected cell by keyboard
    system("cls");
    game.black_count = 0;
    game.white_count = 0;

    bool show_hint = (!game.isSinglePlayerMode) || (game.turn == 1);

    cout << "\n      A  B  C  D  E  F  G  H\n";
    for(int i = 0; i < board_size; i++){
        cout << "   " << (i + 1) << " ";
        for(int j = 0; j < board_size; j++){
            if(curserI == i && curserJ == j){
                cout << BG_YELLOW << BLACK_TXT << u8" \u25A0 " << RESET; // full square
                continue;
            }
            if(game.board[i][j] == cell_state.empty && !CheckValidMove(i, j, false)){
                cout << BG_GREEN << BLACK_TXT << u8" \u25A1 " << RESET; // empty square
            }
            else if(game.board[i][j] == cell_state.black){
                cout << BG_GREEN << BLACK_TXT << u8" \u25CF " << RESET;  // black circle
                game.black_count++;
            }
            else if(game.board[i][j] == cell_state.white){
                cout << BG_GREEN << WHITE_TXT << u8" \u25CF " << RESET; // white circle
                game.white_count++;
            }

            else if(show_hint && game.board[i][j] == cell_state.empty && CheckValidMove(i, j, false)){
                cout << BG_GREEN << RED_TXT << " * " << RESET;
            }
        }
        cout << RESET << endl;
    }

    cout << RESET << "\n";
    cout << MARGIN << CYAN_TXT << u8"══════════════════════════════════" << RESET << endl;
    string p2name = (game.isSinglePlayerMode ? "Bot" : game.player2name);
    cout << MARGIN << BLACK_TXT << RESET << game.player1name << ": " 
         << BOLD <<  game.black_count << RESET;
    cout << "          "; 
    cout << WHITE_TXT << RESET << p2name << ": " 
         << BOLD << game.white_count << RESET << endl;
    cout << endl;
    cout << MARGIN << " Turn: ";
    
    if(game.turn == 1) {
        cout << BG_YELLOW << BLACK_TXT << "  " << u8"\u25CF " << game.player1name << "  " << RESET;
    } 
    else {
        cout << BG_YELLOW << BLACK_TXT << "  " << u8"\u25CB " << p2name << "  " << RESET;
    }
    
    cout << endl;


}

// menu
void ShowMenu(){

    system("cls");

    cout << "\n\n";
    cout << MARGIN << BOLD << CYAN_TXT << u8"╔══════════════════════════════════╗" << RESET << endl;
    cout << MARGIN << BOLD << CYAN_TXT << u8"║           OTHELLO GAME           ║" << RESET << endl;
    cout << MARGIN << BOLD << CYAN_TXT << u8"╚══════════════════════════════════╝" << RESET << endl;
    cout << endl;

    cout << MARGIN << YELLOW_TXT << "[1]" << WHITE_TXT << " Start New Game" << endl;
    cout << MARGIN << "    " << GREEN_TXT << "Play vs Bot or Friend" << RESET << "\n\n";

    cout << MARGIN << YELLOW_TXT << "[2]" << WHITE_TXT << " How to Play (Help)" << endl;
    cout << MARGIN << "    " << GREEN_TXT << "Learn the rules" << RESET << "\n\n";

    cout << MARGIN << YELLOW_TXT << "[3]" << WHITE_TXT << " Game History" << endl;
    cout << MARGIN << "    " << GREEN_TXT << "View past match results" << RESET << "\n\n";

    cout << MARGIN << YELLOW_TXT << "[4]" << WHITE_TXT << " Exit" << endl;
    
    cout << endl << MARGIN << "Select an option: ";

    char choice = getch();
    char temp;
    
    switch(choice)
    {
    case '1': // new game
        GameMode();
        break;
    case '2': // help
        system("cls");
        cout << "\n\n";
        cout << MARGIN << BOLD << MAGENTA_TXT << "=== HOW TO PLAY OTHELLO ===" << RESET << "\n\n";
        cout << MARGIN << "Othello is a strategy board game for two players." << endl;
        cout << MARGIN << "Played on an 8x8 uncheckered board." << endl << endl;
        cout << MARGIN << YELLOW_TXT << "1. Objective:" << RESET << endl;
        cout << MARGIN << "   Have the majority of discs in your color at the end." << endl << endl;
        cout << MARGIN << YELLOW_TXT << "2. The Rules:" << RESET << endl;
        cout << MARGIN << "   - Black always moves first." << endl;
        cout << MARGIN << "   - Place a disc to 'bracket' opponent's discs." << endl;
        cout << MARGIN << "   - Bracketing can be horizontal, vertical, or diagonal." << endl;
        cout << MARGIN << "   - Bracketed discs are flipped to your color." << endl << endl;
        cout << MARGIN << YELLOW_TXT << "3. Winning:" << RESET << endl;
        cout << MARGIN << "   The game ends when the board is full or no valid moves remain." << endl;
        cout << MARGIN << "   The player with the most discs wins." << endl;
        cout << "\n\n" << MARGIN << BG_GREEN << BLACK_TXT << " Press any key to return... " << RESET;
        
        getch();
        ShowMenu();
        break;
    case '3': // History
        ShowHistory();
        break;
    case '4': // exit
        return; // closing terminal
        break;
    
    default:
        cout << "\a";
        ShowMenu();
        break;
    }

}

// choosing which game mode we are gonna have
void GameMode(){
    system("cls");

    cout << "\n\n";
    cout << MARGIN << BOLD << CYAN_TXT << u8"╔══════════════════════════════════╗" << RESET << endl;
    cout << MARGIN << BOLD << CYAN_TXT << u8"║         SELECT GAME MODE         ║" << RESET << endl;
    cout << MARGIN << BOLD << CYAN_TXT << u8"╚══════════════════════════════════╝" << RESET << endl;
    cout << endl;

    cout << MARGIN << YELLOW_TXT << "[1]" << WHITE_TXT << " Single Player" << endl;
    cout << MARGIN << "    " << GREEN_TXT << "Challenge the AI Bot" << RESET << "\n\n";
    cout << MARGIN << YELLOW_TXT << "[2]" << WHITE_TXT << " Two Players" << endl;
    cout << MARGIN << "    " << GREEN_TXT << "Play with a friend on this device" << RESET << "\n\n";
    cout << MARGIN << YELLOW_TXT << "[3]" << WHITE_TXT << " Back to Main Menu" << endl;

    cout << endl << MARGIN << "Select an option: ";

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
    case '3':
        ShowMenu();
        break;
    default:
        cout << '\a';
        GameMode();
        break;
    }

}


// changing the selected cell
void PlaceCell(int i, int j){
    if(game.turn == 1){
        game.board[i][j] = cell_state.black;
    }
    else{
        game.board[i][j] = cell_state.white;
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

    cout << "\n\n";
    cout << MARGIN << BOLD << CYAN_TXT << u8"╔══════════════════════════════════════════════════════════════╗" << RESET << endl;
    cout << MARGIN << BOLD << CYAN_TXT << u8"║                   GAME HISTORY ARCHIVE                       ║" << RESET << endl;
    cout << MARGIN << BOLD << CYAN_TXT << u8"╚══════════════════════════════════════════════════════════════╝" << RESET << endl;
    cout << endl;

    ifstream history("history.txt");
    if(!history.is_open()){
        cout << MARGIN << RED_TXT << "No history found or could not open file." << RESET << endl;
        cout << "\n" << MARGIN << "Press any key to return...";
        getch();
        ShowMenu();
        return;
    }

    cout << MARGIN << YELLOW_TXT << left
         << setw(12) << "Player 1"
         << setw(8)  << "Score"
         << setw(12) << "Player 2"
         << setw(8)  << "Score"
         << setw(12) << "Winner"
         << setw(20) << "Date"
         << RESET << endl;
    cout << MARGIN << CYAN_TXT << u8"──────────────────────────────────────────────────────────────────────" << RESET << endl;



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

        cout << MARGIN << left
            << CYAN_TXT   << setw(12) << player1        
            << BOLD << WHITE_TXT  << setw(8)  << player1discs   
            << RESET << CYAN_TXT   << setw(12) << player2        
            << BOLD << WHITE_TXT  << setw(8)  << player2discs   
            << RESET << GREEN_TXT  << setw(12) << winner         
            << RESET      << setw(20) << date           
            << endl;

    }


    history.close();
    cout << "\n\n" << MARGIN << BG_GREEN << BLACK_TXT << " Press any key to return... " << RESET;
    getch();
    ShowMenu();

}

void BotMove(){

    if(CheckValidTurn(game.turn)){

        // allocating an array for bot's available moves
        int **availabe_moves = new int*[board_size*board_size];
        for(int i = 0; i < (board_size*board_size); i++){
            availabe_moves[i] = new int[2];
        }



        // adding available moves to the array
        int counter = 0;
        for(int i = 0; i < board_size; i++){
            for(int j = 0; j < board_size; j++){
                if(CheckValidMove(i, j, false)){
                    availabe_moves[counter][0] = i;
                    availabe_moves[counter][1] = j;
                    counter++;
                }
            }
        }


        if (counter > 0) {
            int randomIndex = rand() % counter; 

            curserI = availabe_moves[randomIndex][0];
            curserJ = availabe_moves[randomIndex][1];

            CheckValidMove(curserI, curserJ, true);
            PlaceCell(curserI, curserJ);
        }

        
        // deallocating availabe moves array
        for(int i = 0; i < board_size*board_size; i++){
            delete[] availabe_moves[i];
        }
        delete[] availabe_moves;

    }
    else{
        SwitchTurn();
    }


}
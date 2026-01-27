#include <iostream>
#include <termios.h>
#include <unistd.h>
#include <cstdlib> 
#include <ctime>  // for seeding random number generator
#include <string>
// Function to print the current state of the board
void print_board(char board[3][3]) {
    system("clear");
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            std::cout << board[i][j];
        }
        std::cout << "\n";
    }
}

// Function to handle player's move, numeric keypad input 1-9
void player_move(char board[3][3]){
    while (true) {
        int  move;
        int row, col;
        std::cout << "Enter your move (1-9). Numeric Numpad: ";
        std::cin >> move;
        row = 2-(move-1)/3;
        col = (move-1)%3;
        if (board[row][col] != '-'){
            std::cout << "Invalid move. Try again.\n";
        }
        else{
            board[row][col] = 'X';
            break;
        }
    }
}

// Function for computer's move, random selection
void computer_move(char board[3][3]){
    while (true){
        int move = rand() % 9 + 1;
        int row = 2-(move-1)/3;
        int col = (move-1)%3;
        if (board[row][col] == '-'){
            board[row][col] = 'O';
            break;
        }
    }
}

// Function to check for a win condition
std::string check_win(char board[3][3], char player){
    // Check Columns
    for (int i = 0; i < 3; ++i){
        if (board[0][i] == player && board[1][i] == player && board[2][i] == player){
            std::cout << player << " wins!\n";
            exit(0);
        }
    }

    // Check Rows
    for (int j = 0; j < 3; ++j){
        if (board[j][0] == player && board[j][1] == player && board[j][2] == player){
            std::cout << player << " wins!\n";
            exit(0);
        }
    }

    // Check Diagonals
    if (board[0][0] == player && board[1][1] == player && board[2][2] == player){
        std::cout << player << " wins!\n";
        exit(0);
    }
    if (board[0][2] == player && board[1][1] == player && board[2][0] == player){
        std::cout << player << " wins!\n";
        exit(0);
    }
    return "no-one";
}

int main() {
    srand(time(0));  // Seed the random number generator
    // Initialize empty board
    char my_board[3][3] = { {'-', '-', '-'},
                         {'-', '-', '-'},
                         {'-', '-', '-'} };
    
    std::string won = "no-one";
    int moves = 0;
    // Initial board print
    print_board(my_board);
    
    std::cout << "Moves = "<< moves <<"\n";

    while (true){
        // Player's turn
        player_move(my_board);
        print_board(my_board);
        won = check_win(my_board, 'X');
        moves++;
        if (moves == 9){
            std::cout << "It's a draw!\n";
            break;
        }
        // Computer's turn
        computer_move(my_board);
        print_board(my_board);
        won = check_win(my_board, 'O');
        moves++;
        std::cout << "Moves = "<< moves <<"\n";

    }



    return 0;
}
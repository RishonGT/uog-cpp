#include <iostream>
#include <termios.h>
#include <unistd.h>
#include <cstdlib> 
#include <ctime>  // for seeding random number generator
#include <string>
#include <limits> // for std::numeric_limits

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

// Function to handle player's move, Computer keypad input 1-9
void player_move(char board[3][3]){
    while (true) {
        int  move;
        int row, col;

        // Prompt the user for input
        std::cout << "Enter your move (1-9). Computer Numpad: ";
        std::cin >> move;

        // Check to make sure the input is valid, if not, clear the error state and ignore the input
        if (std::cin.fail()) {
            std::cin.clear(); // Clear the error state
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Discard invalid input
            std::cout << "Invalid input. Please enter a number between 1 and 9.\n";
            continue;
        }

        // Check if the move is within the valid range
        if (move < 1 || move > 9){
            std::cout << "Invalid input. Please enter a number between 1 and 9.\n";
            continue;
        }
        
        // Convert the move to board coordinates
        row = 2-(move-1)/3;
        col = (move-1)%3;

        // Check if the selected cell is already occupied, if not, place the player's mark
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
        // Generate a random move between 1 and 9
        // Convert the move to board coordinates
        // Check if the selected cell is already occupied, if not, place the computer's mark
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
int check_win(char board[3][3], char player){
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
    return 0;
}

int main() {
    srand(time(0));  // Seed the random number generator
    // Initialize empty board
    char my_board[3][3] = { {'-', '-', '-'},
                         {'-', '-', '-'},
                         {'-', '-', '-'} };
    
    int won = 0;
    int moves = 0;
    // Initial board print
    print_board(my_board);
    
    std::cout << "Moves = "<< moves <<"\n";

    // Game loop
    // Alternate turns between player and computer until a win or draw condition is met
    while (true){
        // Player's turn
        player_move(my_board);
        print_board(my_board);
        won = check_win(my_board, 'X');
        moves++;
        if (won == 1){
            break;
        }
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
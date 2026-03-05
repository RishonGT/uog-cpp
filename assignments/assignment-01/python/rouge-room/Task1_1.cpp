#include <iostream>
#include <termios.h>
#include <unistd.h>
#include <cstdlib> 

// w, h, x, y -> width, height, player x, player y 
void draw_room(int w, int h, int x, int y){
    // Print the top wall
    std::cout << "+" << std::string(w, '-') << "+" << std::endl;

    // Print the rows in the middle of the room
    for (int i = 0; i < h; ++i) {
        
        // If the row being printed has the player inside
        if (i == y) {
            std::cout << "|" << std::string(x, '.')<< '@' << std::string(w - x - 1, '.') << "|" << std::endl;
        }
        // Empty row
        else {
            std::cout << "|" << std::string(w,'.') << "|" << std::endl;
        }
    }

    // Print the bottom wall
    std::cout << "+" << std::string(w, '-') << "+" << std::endl;
}

// Function to get a single character input without waiting for Enter key
char getch() {
    struct termios oldt, newt;  // Use termios.h to change terminal settings temporarily
    char ch;

    tcgetattr(STDIN_FILENO, &oldt); // Get current terminal attributes and store in oldt
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO); // Disable canonical mode and echo

    tcsetattr(STDIN_FILENO, TCSANOW, &newt); // Apply new attributes
    ch = getchar(); // Read a single character
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt); // Restore old attributes

    return ch;
}

// This class is responsible for drawing and updating the room
class Room {
    public:

        // Initialises the room with width and height
        Room(int w, int h) : width(w), height(h) {}

        // Draw the room with the player at (x, y)
        void draw(int x, int y) {
            draw_room(width, height, x, y);
        }
        
        // Used to find the current width and height of the players
        int getWidth() const {
            return width;
        }
        int getHeight() const {
            return height;
        }

    // Private members to store the dimensions of the room
    private:
        int width;
        int height;
    };

int main() {
    // Initialize a 10x5 room with player at the top right
    int width = 10;
    int height = 5;
    int x = 0; 
    int y = 0;
    
    // Create a Room object
    Room room(width, height);

    char input;
    while (true){

        // Clear the console so the room is redrawn fresh
        system("clear");

        room.draw(x, y);
        std::cout << "Use WASD to move, X to quit: " << std::endl;
        input = getch();  // Get user input
        

        // Update player position based on input, disallow moving out of bounds
        if (input == 'w' && y > 0) {
            y--;
        } 
        else if (input == 's' && y < room.getHeight() - 1) {
            y++;
        } 
        else if (input == 'a' && x > 0) {
            x--;
        } 
        else if (input == 'd' && x < room.getWidth() - 1) {
            x++;
        } 
        else if (input == 'x') {
            break;
        }


    }
    return 0;
}
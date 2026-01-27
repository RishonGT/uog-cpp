#include <iostream>
#include <termios.h>
#include <unistd.h>
#include <cstdlib> 

// w, h, x, y -> width, height, player x, player y
void draw_room(int w, int h, int x, int y){
    // Print the top wall
    std::cout << "+" << std::string(w, '-') << "+" << std::endl;

    // Print the sides
    for (int i = 0; i < h; ++i) {
        if (i == y) {
            std::cout << "|" << std::string(x, '.')<< '@' << std::string(w - x - 1, '.') << "|" << std::endl;
        }
        else {
            std::cout << "|" << std::string(w,'.') << "|" << std::endl;
        }
    }

    // Print the bottom wall
    std::cout << "+" << std::string(w, '-') << "+" << std::endl;
}

class Room {
// This can be accessed by other parts of the program
public:
    // Program draws a room of given width and height
    Room(int width, int height) : width(width), height(height) {}
    // Draw the room with the player at (x, y)
    void draw(int x, int y) {
        draw_room(width, height, x, y);
    }
    // Getters for width and height
    int getWidth() const {
        return width;
    }
    int getHeight() const {
        return height;
    }

// This can only be accessed by member functions of Room
private:
    int width;
    int height;
};

// Function to get a single character input without waiting for Enter key
char getch() {
    struct termios oldt, newt;
    char ch;
    tcgetattr(STDIN_FILENO, &oldt); // Get current terminal attributes
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO); // Disable canonical mode and echo
    tcsetattr(STDIN_FILENO, TCSANOW, &newt); // Apply new attributes
    ch = getchar(); // Read a single character
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt); // Restore old attributes
    return ch;
}

int main() {
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
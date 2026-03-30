#include<iostream>
#include<ncurses.h>

#include "drawroom.h"

int main()
{
    //function to start curses window
    initscr();

    //User Input for Room Size
    echo(); //Show user input text
    int roomwidth{};
    int roomheight{};

    printw("Enter width: ");
    scanw("%d", &roomwidth);
    printw("Enter height: ");
    scanw("%d", &roomheight);
    clear(); //Clear screen for game
    noecho(); //Disables user input text

    //Construct the Room
    drawfullroom(roomheight,roomwidth);
    
    curs_set(0); //hide cursor
    //Player while loop
    
    //Initial player position (1,1). Top left of any room.
    int playerx {1};
    int playery {1};
    mvaddch(playery, playerx, '@'); // MOVE cursor and ADDCHaracter

    int keypress {}; //Store keypress
    keypad(stdscr, TRUE); //Enable arrowkeys 

    // A move is deleting @, replacing dirt and overwriting to the moved location.
    while(1)
    {
        keypress = getch();
        switch(keypress)
        {
            case KEY_UP:
                if((playery - 1) == 0) //Top wall check
                    break;
                else
                {
                    mvaddch(playery, playerx, '.'); //Replace the dirt in old location, overwriting the player
                    --playery;
                    break;
                }
            
            case KEY_DOWN:
                if((playery + 1) > roomheight) //Bottom wall check
                    break;
                else
                {
                    mvaddch(playery, playerx, '.');
                    ++playery;
                    break;
                }
            case KEY_LEFT:
                if((playerx - 1) == 0) //LEFT wall check
                    break;
                else
                {
                    mvaddch(playery, playerx, '.'); //Replace the dirt in old location, overwriting the player
                    --playerx;
                    break;
                }
            
            case KEY_RIGHT:
                if((playerx + 1) > roomwidth) //Bottom wall check
                    break;
                else
                {
                    mvaddch(playery, playerx, '.');
                    ++playerx;
                    break;
                }
        }
        mvaddch(playery,playerx, '@'); //Redraw the player character with the new location
        refresh();
        if(keypress == 'q')
            break;
    }

    //end curses window
    endwin();
    
    return 0;
}

